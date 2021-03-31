extern int fgetc(void*);

int getc(void* file) {
  return fgetc(file);
}
