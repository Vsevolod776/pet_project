#ifndef MY_STRING_H
#define MY_STRING_H

#include "stdio.h"

int strcmp(const char *s1, const char *s2);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *ptr, int value, size_t num);
size_t strlen(const char *string);

#endif