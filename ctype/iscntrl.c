int iscntrl(int c) {
  return (unsigned) c <= 31 || c == 127;
}
