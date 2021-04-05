.global memmove
memmove:
  mov %rdi, %rax
  mov %rdx, %rcx
  cmp %rdi, %rsi
  jge mov
  dec %rdx
  add %rdx, %rdi
  add %rdx, %rsi
  std
mov:
  rep movsb
  cld
  ret
