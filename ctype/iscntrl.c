int iscntrl(int c) {
  return (unsigned int) c <= 31 || c == 127;
}
