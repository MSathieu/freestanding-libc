#pragma once
#include <stdarg.h>
#include <stddef.h>

int getchar(void);
int putchar(int);
int snprintf(char* restrict, size_t, const char* restrict, ...);
int sprintf(char* restrict, const char* restrict, ...);
int vsnprintf(char* restrict, size_t, const char* restrict, va_list);
int vsprintf(char* restrict, const char* restrict, va_list);
