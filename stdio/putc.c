extern int fputc(int, void*);

int putc(int c, void* file) {
  return fputc(c, file);
}
