#pragma once
#include <stddef.h>

void* memchr(const void* ptr, int c, size_t size);
int memcmp(const void* aptr, const void* bptr, size_t size);
void* memcpy(void* restrict destptr, const void* restrict srcptr, size_t size);
void* memmove(void* destptr, const void* srcptr, size_t size);
void* memset(void* ptr, int c, size_t size);
char* strcat(char* restrict dest, const char* restrict src);
char* strchr(const char* str, int c);
int strcmp(const char* a, const char* b);
char* strcpy(char* restrict dest, const char* restrict src);
size_t strcspn(const char* str, const char* chars);
int strncmp(const char* a, const char* b, size_t size);
char* strncpy(char* restrict dest, const char* restrict src, size_t size);
size_t strlen(const char* str);
size_t strspn(const char* str, const char* chars);
char* strstr(const char* a, const char* b);
char* strtok(char* restrict new_str, const char* restrict sep);
