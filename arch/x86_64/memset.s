.global memset
memset:
  mov %rdi, %r8
  movzb %sil, %rsi
  mov %rsi, %rax
  shl $8, %rsi
  or %rsi, %rax
  shl $8, %rsi
  or %rsi, %rax
  shl $8, %rsi
  or %rsi, %rax
  shl $8, %rsi
  or %rsi, %rax
  shl $8, %rsi
  or %rsi, %rax
  shl $8, %rsi
  or %rsi, %rax
  shl $8, %rsi
  or %rsi, %rax
  mov %rdx, %rcx
  shr $3, %rcx
  rep stosq
  mov %rdx, %rcx
  and $7, %rcx
  rep stosb
  mov %r8, %rax
  ret
