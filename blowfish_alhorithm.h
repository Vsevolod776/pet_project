#ifndef BLOWFISH_ALHORITHM_H
#define BLOWFISH_ALHORITHM_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

typedef struct
{
    uint32_t P[18];
    uint32_t S[4][256];
}BlowfishKey;

void encrypt(uint32_t *L, uint32_t *R, const BlowfishKey *key);
void decrypt(uint32_t *L, uint32_t *R, const BlowfishKey *key);
void initKey(BlowfishKey *key, const uint8_t *key_data, int key_length);

#endif
