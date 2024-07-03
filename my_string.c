#include "my_string.h"

size_t strlen(const char *string)
{
    size_t length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

/*void *memcpy(void *dest, const void *src, size_t n)
{
}*/