extern int getc(void*);
extern void* stdin;

int getchar(void) {
  return getc(stdin);
}
