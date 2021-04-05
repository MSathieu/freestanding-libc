.global memset
memset:
  push %rdi
  mov %rsi, %rax
  mov %rdx, %rcx
  rep stosb
  pop %rax
  ret
