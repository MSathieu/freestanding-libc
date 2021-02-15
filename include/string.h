#pragma once
#include <stddef.h>

int memcmp(const void*, const void*, size_t);
void* memcpy(void* restrict, const void* restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
char* strcat(char* restrict, const char* restrict);
char* strchr(const char*, int);
int strcmp(const char*, const char*);
char* strcpy(char* restrict, const char* restrict);
size_t strcspn(const char*, const char*);
int strncmp(const char*, const char*, size_t);
char* strncpy(char* restrict, const char* restrict, size_t);
size_t strlen(const char*);
size_t strspn(const char*, const char*);
char* strtok(char* restrict, const char* restrict);
