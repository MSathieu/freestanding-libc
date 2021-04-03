#pragma once
#include <stdarg.h>
#include <stddef.h>

int getchar(void);
int printf(const char* restrict format, ...);
int putchar(int c);
int snprintf(char* restrict buffer, size_t size, const char* restrict format, ...);
int sprintf(char* restrict buffer, const char* restrict format, ...);
int vprintf(const char* restrict format, va_list args);
int vsnprintf(char* restrict buffer, size_t size, const char* restrict format, va_list args);
int vsprintf(char* restrict buffer, const char* restrict format, va_list args);
