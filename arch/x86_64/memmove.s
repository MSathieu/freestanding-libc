.global memmove
memmove:
  mov %rdi, %rax
  mov %rdx, %rcx
  shr $3, %rcx
  cmp %rdi, %rsi
  jge mov
  dec %rdx
  add %rdx, %rdi
  add %rdx, %rsi
  std
mov:
  rep movsq
  mov %rdx, %rcx
  and $7, %rcx
  rep movsb
  cld
  ret
