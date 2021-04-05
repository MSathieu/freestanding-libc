.global memcpy
memcpy:
  mov %rdi, %rax
  mov %rdx, %rcx
  rep movsb
  ret
