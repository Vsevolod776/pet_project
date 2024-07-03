#include "my_string.h"

size_t strlen(const char *string)
{
    size_t length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char* destChar = (unsigned char*)dest;
    const unsigned char* srcChar = (const unsigned char*)src;
    size_t i;

    for( i=0; i<n; ++i )
        destChar[i]=srcChar[i];

    return dest;
}

int strcmp(const char *string1, const char *string2)
{
    while (*string1 == *string2 && *string1 != '\0' ) {
        string1++;
        string2++;
    }

    if (*string1 == '\0' && *string2 == '\0')
        return 0;

    return *string1 > *string2 ? 1 : -1;
}

void *memset(void *ptr, int value, size_t num) {
    unsigned char *copy_p = ptr;
    size_t i;

    for (i = 0; i < num; ++i)
        copy_p[i] = (unsigned char)value;

    return ptr;
}
