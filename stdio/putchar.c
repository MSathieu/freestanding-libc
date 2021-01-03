extern int putc(int, void*);
extern void* stdout;

int putchar(int c) {
  return putc(c, stdout);
}
