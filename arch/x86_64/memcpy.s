.global memcpy
memcpy:
  mov %rdi, %rax
  mov %rdx, %rcx
  shr $3, %rcx
  rep movsq
  mov %rdx, %rcx
  and $7, %rcx
  rep movsb
  ret
