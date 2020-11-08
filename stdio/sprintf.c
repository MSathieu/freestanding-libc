// Taken from https://github.com/nothings/stb/blob/master/stb_sprintf.h (314d0a6f9af5af27e585336eecea333e95c5a2d8)

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#define STB_SPRINTF_MIN 512
#define STBSP__LEFTJUST 1
#define STBSP__LEADINGPLUS 2
#define STBSP__LEADINGSPACE 4
#define STBSP__LEADING_0X 8
#define STBSP__LEADINGZERO 16
#define STBSP__INTMAX 32
#define STBSP__TRIPLET_COMMA 64
#define STBSP__NEGATIVE 128
#define STBSP__METRIC_SUFFIX 256
#define STBSP__HALFWIDTH 512
#define STBSP__METRIC_NOSPACE 1024
#define STBSP__METRIC_1024 2048
#define STBSP__METRIC_JEDEC 4096

typedef char* STBSP_SPRINTFCB(const char* buf, void* user, int len);

static char stbsp__comma = ',';
static struct
{
  short temp; // force next field to be 2-byte aligned
  char pair[201];
} stbsp__digitpair =
    {
        0,
        "00010203040506070809101112131415161718192021222324"
        "25262728293031323334353637383940414243444546474849"
        "50515253545556575859606162636465666768697071727374"
        "75767778798081828384858687888990919293949596979899"};

static void stbsp__lead_sign(uint32_t fl, char* sign) {
  sign[0] = 0;
  if (fl & STBSP__NEGATIVE) {
    sign[0] = 1;
    sign[1] = '-';
  } else if (fl & STBSP__LEADINGSPACE) {
    sign[0] = 1;
    sign[1] = ' ';
  } else if (fl & STBSP__LEADINGPLUS) {
    sign[0] = 1;
    sign[1] = '+';
  }
}

static int vsprintfcb(STBSP_SPRINTFCB* callback, void* user, char* buf, char const* fmt, va_list va) {
  static char hex[] = "0123456789abcdefxp";
  static char hexu[] = "0123456789ABCDEFXP";
  char* bf;
  char const* f;
  int tlen = 0;

  bf = buf;
  f = fmt;
  for (;;) {
    int32_t fw, pr, tz;
    uint32_t fl;

// macros for the callback buffer stuff
#define stbsp__chk_cb_bufL(bytes)                     \
  {                                                   \
    int len = (int) (bf - buf);                       \
    if ((len + (bytes)) >= STB_SPRINTF_MIN) {         \
      tlen += len;                                    \
      if (0 == (bf = buf = callback(buf, user, len))) \
        goto done;                                    \
    }                                                 \
  }
#define stbsp__chk_cb_buf(bytes) \
  {                              \
    if (callback) {              \
      stbsp__chk_cb_bufL(bytes); \
    }                            \
  }
#define stbsp__flush_cb()                    \
  {                                          \
    stbsp__chk_cb_bufL(STB_SPRINTF_MIN - 1); \
  } // flush if there is even one byte in the buffer
#define stbsp__cb_buf_clamp(cl, v)               \
  cl = v;                                        \
  if (callback) {                                \
    int lg = STB_SPRINTF_MIN - (int) (bf - buf); \
    if (cl > lg)                                 \
      cl = lg;                                   \
  }

    // fast copy everything up to the next % (or end of string)
    for (;;) {
      while (((uintptr_t) f) & 3) {
      schk1:
        if (f[0] == '%')
          goto scandd;
      schk2:
        if (f[0] == 0)
          goto endfmt;
        stbsp__chk_cb_buf(1);
        *bf++ = f[0];
        ++f;
      }
      for (;;) {
        // Check if the next 4 bytes contain %(0x25) or end of string.
        // Using the 'hasless' trick:
        // https://graphics.stanford.edu/~seander/bithacks.html#HasLessInWord
        uint32_t v, c;
        v = *(uint32_t*) f;
        c = (~v) & 0x80808080;
        if (((v ^ 0x25252525) - 0x01010101) & c)
          goto schk1;
        if ((v - 0x01010101) & c)
          goto schk2;
        if (callback)
          if ((STB_SPRINTF_MIN - (int) (bf - buf)) < 4)
            goto schk1;
        {
          *(uint32_t*) bf = v;
        }
        bf += 4;
        f += 4;
      }
    }
  scandd:

    ++f;

    // ok, we have a percent, read the modifiers first
    fw = 0;
    pr = -1;
    fl = 0;
    tz = 0;

    // flags
    for (;;) {
      switch (f[0]) {
      // if we have left justify
      case '-':
        fl |= STBSP__LEFTJUST;
        ++f;
        continue;
      // if we have leading plus
      case '+':
        fl |= STBSP__LEADINGPLUS;
        ++f;
        continue;
      // if we have leading space
      case ' ':
        fl |= STBSP__LEADINGSPACE;
        ++f;
        continue;
      // if we have leading 0x
      case '#':
        fl |= STBSP__LEADING_0X;
        ++f;
        continue;
      // if we have thousand commas
      case '\'':
        fl |= STBSP__TRIPLET_COMMA;
        ++f;
        continue;
      // if we have kilo marker (none->kilo->kibi->jedec)
      case '$':
        if (fl & STBSP__METRIC_SUFFIX) {
          if (fl & STBSP__METRIC_1024) {
            fl |= STBSP__METRIC_JEDEC;
          } else {
            fl |= STBSP__METRIC_1024;
          }
        } else {
          fl |= STBSP__METRIC_SUFFIX;
        }
        ++f;
        continue;
      // if we don't want space between metric suffix and number
      case '_':
        fl |= STBSP__METRIC_NOSPACE;
        ++f;
        continue;
      // if we have leading zero
      case '0':
        fl |= STBSP__LEADINGZERO;
        ++f;
        goto flags_done;
      default:
        goto flags_done;
      }
    }
  flags_done:

    // get the field width
    if (f[0] == '*') {
      fw = va_arg(va, uint32_t);
      ++f;
    } else {
      while ((f[0] >= '0') && (f[0] <= '9')) {
        fw = fw * 10 + f[0] - '0';
        f++;
      }
    }
    // get the precision
    if (f[0] == '.') {
      ++f;
      if (f[0] == '*') {
        pr = va_arg(va, uint32_t);
        ++f;
      } else {
        pr = 0;
        while ((f[0] >= '0') && (f[0] <= '9')) {
          pr = pr * 10 + f[0] - '0';
          f++;
        }
      }
    }

    // handle integer size overrides
    switch (f[0]) {
    // are we halfwidth?
    case 'h':
      fl |= STBSP__HALFWIDTH;
      ++f;
      if (f[0] == 'h')
        ++f; // QUARTERWIDTH
      break;
    // are we 64-bit (unix style)
    case 'l':
      fl |= ((sizeof(long) == 8) ? STBSP__INTMAX : 0);
      ++f;
      if (f[0] == 'l') {
        fl |= STBSP__INTMAX;
        ++f;
      }
      break;
    // are we 64-bit on intmax? (c99)
    case 'j':
      fl |= (sizeof(size_t) == 8) ? STBSP__INTMAX : 0;
      ++f;
      break;
    // are we 64-bit on size_t or ptrdiff_t? (c99)
    case 'z':
      fl |= (sizeof(ptrdiff_t) == 8) ? STBSP__INTMAX : 0;
      ++f;
      break;
    case 't':
      fl |= (sizeof(ptrdiff_t) == 8) ? STBSP__INTMAX : 0;
      ++f;
      break;
    // are we 64-bit (msft style)
    case 'I':
      if ((f[1] == '6') && (f[2] == '4')) {
        fl |= STBSP__INTMAX;
        f += 3;
      } else if ((f[1] == '3') && (f[2] == '2')) {
        f += 3;
      } else {
        fl |= ((sizeof(void*) == 8) ? STBSP__INTMAX : 0);
        ++f;
      }
      break;
    default:
      break;
    }

    // handle each replacement
    switch (f[0]) {
#define STBSP__NUMSZ 512 // big enough for e308 (with commas) or e-307
      char num[STBSP__NUMSZ];
      char lead[8];
      char tail[8];
      char* s;
      char const* h;
      uint32_t l, n, cs;
      uint64_t n64;
      char const* sn;

    case 's':
      // get the string
      s = va_arg(va, char*);
      if (s == 0)
        s = (char*) "null";
      // get the length
      sn = s;
      for (;;) {
        if ((((uintptr_t) sn) & 3) == 0)
          break;
      lchk:
        if (sn[0] == 0)
          goto ld;
        ++sn;
      }
      n = 0xffffffff;
      if (pr >= 0) {
        n = (uint32_t)(sn - s);
        if (n >= (uint32_t) pr)
          goto ld;
        n = ((uint32_t)(pr - n)) >> 2;
      }
      while (n) {
        uint32_t v = *(uint32_t*) sn;
        if ((v - 0x01010101) & (~v) & 0x80808080UL)
          goto lchk;
        sn += 4;
        --n;
      }
      goto lchk;
    ld:

      l = (uint32_t)(sn - s);
      // clamp to precision
      if (l > (uint32_t) pr)
        l = pr;
      lead[0] = 0;
      tail[0] = 0;
      pr = 0;
      cs = 0;
      // copy the string in
      goto scopy;

    case 'c': // char
      // get the character
      s = num + STBSP__NUMSZ - 1;
      *s = (char) va_arg(va, int);
      l = 1;
      lead[0] = 0;
      tail[0] = 0;
      pr = 0;
      cs = 0;
      goto scopy;

    case 'n': // weird write-bytes specifier
    {
      int* d = va_arg(va, int*);
      *d = tlen + (int) (bf - buf);
    } break;

    case 'A': // float
    case 'a': // hex float
    case 'G': // float
    case 'g': // float
    case 'E': // float
    case 'e': // float
    case 'f': // float
    case 'B': // upper binary
    case 'b': // lower binary
      h = (f[0] == 'B') ? hexu : hex;
      lead[0] = 0;
      if (fl & STBSP__LEADING_0X) {
        lead[0] = 2;
        lead[1] = '0';
        lead[2] = h[0xb];
      }
      l = (8 << 4) | (1 << 8);
      goto radixnum;

    case 'o': // octal
      h = hexu;
      lead[0] = 0;
      if (fl & STBSP__LEADING_0X) {
        lead[0] = 1;
        lead[1] = '0';
      }
      l = (3 << 4) | (3 << 8);
      goto radixnum;

    case 'p': // pointer
      fl |= (sizeof(void*) == 8) ? STBSP__INTMAX : 0;
      pr = sizeof(void*) * 2;
      fl &= ~STBSP__LEADINGZERO; // 'p' only prints the pointer with zeros
                                 // fall through - to X

    case 'X': // upper hex
    case 'x': // lower hex
      h = (f[0] == 'X') ? hexu : hex;
      l = (4 << 4) | (4 << 8);
      lead[0] = 0;
      if (fl & STBSP__LEADING_0X) {
        lead[0] = 2;
        lead[1] = '0';
        lead[2] = h[16];
      }
    radixnum:
      // get the number
      if (fl & STBSP__INTMAX)
        n64 = va_arg(va, uint64_t);
      else
        n64 = va_arg(va, uint32_t);

      s = num + STBSP__NUMSZ;
      // clear tail, and clear leading if value is zero
      tail[0] = 0;
      if (n64 == 0) {
        lead[0] = 0;
        if (pr == 0) {
          l = 0;
          cs = (((l >> 4) & 15)) << 24;
          goto scopy;
        }
      }
      // convert to string
      for (;;) {
        *--s = h[n64 & ((1 << (l >> 8)) - 1)];
        n64 >>= (l >> 8);
        if (!((n64) || ((int32_t)((num + STBSP__NUMSZ) - s) < pr)))
          break;
        if (fl & STBSP__TRIPLET_COMMA) {
          ++l;
          if ((l & 15) == ((l >> 4) & 15)) {
            l &= ~15;
            *--s = stbsp__comma;
          }
        }
      };
      // get the tens and the comma pos
      cs = (uint32_t)((num + STBSP__NUMSZ) - s) + ((((l >> 4) & 15)) << 24);
      // get the length that we copied
      l = (uint32_t)((num + STBSP__NUMSZ) - s);
      // copy it
      goto scopy;

    case 'u': // unsigned
    case 'i':
    case 'd': // integer
      // get the integer and abs it
      if (fl & STBSP__INTMAX) {
        int64_t i64 = va_arg(va, int64_t);
        n64 = (uint64_t) i64;
        if ((f[0] != 'u') && (i64 < 0)) {
          n64 = (uint64_t) -i64;
          fl |= STBSP__NEGATIVE;
        }
      } else {
        int32_t i = va_arg(va, int32_t);
        n64 = (uint32_t) i;
        if ((f[0] != 'u') && (i < 0)) {
          n64 = (uint32_t) -i;
          fl |= STBSP__NEGATIVE;
        }
      }
      // convert to string
      s = num + STBSP__NUMSZ;
      l = 0;

      for (;;) {
        // do in 32-bit chunks (avoid lots of 64-bit divides even with constant denominators)
        char* o = s - 8;
        if (n64 >= 100000000) {
          n = (uint32_t)(n64 % 100000000);
          n64 /= 100000000;
        } else {
          n = (uint32_t) n64;
          n64 = 0;
        }
        if ((fl & STBSP__TRIPLET_COMMA) == 0) {
          do {
            s -= 2;
            *(uint16_t*) s = *(uint16_t*) &stbsp__digitpair.pair[(n % 100) * 2];
            n /= 100;
          } while (n);
        }
        while (n) {
          if ((fl & STBSP__TRIPLET_COMMA) && (l++ == 3)) {
            l = 0;
            *--s = stbsp__comma;
            --o;
          } else {
            *--s = (char) (n % 10) + '0';
            n /= 10;
          }
        }
        if (n64 == 0) {
          if ((s[0] == '0') && (s != (num + STBSP__NUMSZ)))
            ++s;
          break;
        }
        while (s != o)
          if ((fl & STBSP__TRIPLET_COMMA) && (l++ == 3)) {
            l = 0;
            *--s = stbsp__comma;
            --o;
          } else {
            *--s = '0';
          }
      }

      tail[0] = 0;
      stbsp__lead_sign(fl, lead);

      // get the length that we copied
      l = (uint32_t)((num + STBSP__NUMSZ) - s);
      if (l == 0) {
        *--s = '0';
        l = 1;
      }
      cs = l + (3 << 24);
      if (pr < 0)
        pr = 0;

    scopy:
      // get fw=leading/trailing space, pr=leading zeros
      if (pr < (int32_t) l)
        pr = l;
      n = pr + lead[0] + tail[0] + tz;
      if (fw < (int32_t) n)
        fw = n;
      fw -= n;
      pr -= l;

      // handle right justify and leading zeros
      if ((fl & STBSP__LEFTJUST) == 0) {
        if (fl & STBSP__LEADINGZERO) // if leading zeros, everything is in pr
        {
          pr = (fw > pr) ? fw : pr;
          fw = 0;
        } else {
          fl &= ~STBSP__TRIPLET_COMMA; // if no leading zeros, then no commas
        }
      }

      // copy the spaces and/or zeros
      if (fw + pr) {
        int32_t i;
        uint32_t c;

        // copy leading spaces (or when doing %8.4d stuff)
        if ((fl & STBSP__LEFTJUST) == 0)
          while (fw > 0) {
            stbsp__cb_buf_clamp(i, fw);
            fw -= i;
            while (i) {
              if ((((uintptr_t) bf) & 3) == 0)
                break;
              *bf++ = ' ';
              --i;
            }
            while (i >= 4) {
              *(uint32_t*) bf = 0x20202020;
              bf += 4;
              i -= 4;
            }
            while (i) {
              *bf++ = ' ';
              --i;
            }
            stbsp__chk_cb_buf(1);
          }

        // copy leader
        sn = lead + 1;
        while (lead[0]) {
          stbsp__cb_buf_clamp(i, lead[0]);
          lead[0] -= (char) i;
          while (i) {
            *bf++ = *sn++;
            --i;
          }
          stbsp__chk_cb_buf(1);
        }

        // copy leading zeros
        c = cs >> 24;
        cs &= 0xffffff;
        cs = (fl & STBSP__TRIPLET_COMMA) ? ((uint32_t)(c - ((pr + cs) % (c + 1)))) : 0;
        while (pr > 0) {
          stbsp__cb_buf_clamp(i, pr);
          pr -= i;
          if ((fl & STBSP__TRIPLET_COMMA) == 0) {
            while (i) {
              if ((((uintptr_t) bf) & 3) == 0)
                break;
              *bf++ = '0';
              --i;
            }
            while (i >= 4) {
              *(uint32_t*) bf = 0x30303030;
              bf += 4;
              i -= 4;
            }
          }
          while (i) {
            if ((fl & STBSP__TRIPLET_COMMA) && (cs++ == c)) {
              cs = 0;
              *bf++ = stbsp__comma;
            } else
              *bf++ = '0';
            --i;
          }
          stbsp__chk_cb_buf(1);
        }
      }

      // copy leader if there is still one
      sn = lead + 1;
      while (lead[0]) {
        int32_t i;
        stbsp__cb_buf_clamp(i, lead[0]);
        lead[0] -= (char) i;
        while (i) {
          *bf++ = *sn++;
          --i;
        }
        stbsp__chk_cb_buf(1);
      }

      // copy the string
      n = l;
      while (n) {
        int32_t i;
        stbsp__cb_buf_clamp(i, n);
        n -= i;
        while (i >= 4) {
          *(uint32_t volatile*) bf = *(uint32_t volatile*) s;
          bf += 4;
          s += 4;
          i -= 4;
        }
        while (i) {
          *bf++ = *s++;
          --i;
        }
        stbsp__chk_cb_buf(1);
      }

      // copy trailing zeros
      while (tz) {
        int32_t i;
        stbsp__cb_buf_clamp(i, tz);
        tz -= i;
        while (i) {
          if ((((uintptr_t) bf) & 3) == 0)
            break;
          *bf++ = '0';
          --i;
        }
        while (i >= 4) {
          *(uint32_t*) bf = 0x30303030;
          bf += 4;
          i -= 4;
        }
        while (i) {
          *bf++ = '0';
          --i;
        }
        stbsp__chk_cb_buf(1);
      }

      // copy tail if there is one
      sn = tail + 1;
      while (tail[0]) {
        int32_t i;
        stbsp__cb_buf_clamp(i, tail[0]);
        tail[0] -= (char) i;
        while (i) {
          *bf++ = *sn++;
          --i;
        }
        stbsp__chk_cb_buf(1);
      }

      // handle the left justify
      if (fl & STBSP__LEFTJUST)
        if (fw > 0) {
          while (fw) {
            int32_t i;
            stbsp__cb_buf_clamp(i, fw);
            fw -= i;
            while (i) {
              if ((((uintptr_t) bf) & 3) == 0)
                break;
              *bf++ = ' ';
              --i;
            }
            while (i >= 4) {
              *(uint32_t*) bf = 0x20202020;
              bf += 4;
              i -= 4;
            }
            while (i--)
              *bf++ = ' ';
            stbsp__chk_cb_buf(1);
          }
        }
      break;

    default: // unknown, just copy code
      s = num + STBSP__NUMSZ - 1;
      *s = f[0];
      l = 1;
      fw = fl = 0;
      lead[0] = 0;
      tail[0] = 0;
      pr = 0;
      cs = 0;
      goto scopy;
    }
    ++f;
  }
endfmt:

  if (!callback)
    *bf = 0;
  else
    stbsp__flush_cb();

done:
  return tlen + (int) (bf - buf);
}

// ============================================================================
//   wrapper functions

int sprintf(char* buf, char const* fmt, ...) {
  int result;
  va_list va;
  va_start(va, fmt);
  result = vsprintfcb(0, 0, buf, fmt, va);
  va_end(va);
  return result;
}

typedef struct stbsp__context {
  char* buf;
  int count;
  int length;
  char tmp[STB_SPRINTF_MIN];
} stbsp__context;

static char* stbsp__clamp_callback(const char* buf, void* user, int len) {
  stbsp__context* c = (stbsp__context*) user;
  c->length += len;

  if (len > c->count)
    len = c->count;

  if (len) {
    if (buf != c->buf) {
      const char *s, *se;
      char* d;
      d = c->buf;
      s = buf;
      se = buf + len;
      do {
        *d++ = *s++;
      } while (s < se);
    }
    c->buf += len;
    c->count -= len;
  }

  if (c->count <= 0)
    return c->tmp;
  return (c->count >= STB_SPRINTF_MIN) ? c->buf : c->tmp; // go direct into buffer if you can
}

static char* stbsp__count_clamp_callback(const char* buf, void* user, int len) {
  stbsp__context* c = (stbsp__context*) user;
  (void) sizeof(buf);

  c->length += len;
  return c->tmp; // go direct into buffer if you can
}

int vsnprintf(char* buf, int count, char const* fmt, va_list va) {
  stbsp__context c;

  if ((count == 0) && !buf) {
    c.length = 0;

    vsprintfcb(stbsp__count_clamp_callback, &c, c.tmp, fmt, va);
  } else {
    int l;

    c.buf = buf;
    c.count = count;
    c.length = 0;

    vsprintfcb(stbsp__clamp_callback, &c, stbsp__clamp_callback(0, &c, 0), fmt, va);

    // zero-terminate
    l = (int) (c.buf - buf);
    if (l >= count) // should never be greater, only equal (or less) than count
      l = count - 1;
    buf[l] = 0;
  }

  return c.length;
}

int snprintf(char* buf, int count, char const* fmt, ...) {
  int result;
  va_list va;
  va_start(va, fmt);

  result = vsnprintf(buf, count, fmt, va);
  va_end(va);

  return result;
}

int vsprintf(char* buf, char const* fmt, va_list va) {
  return vsprintfcb(0, 0, buf, fmt, va);
}
