#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//own
#include "blowfish_alhorithm.h"

void handleErrors(const char *text) {
    fprintf(stderr, "%s", text);
    exit(EXIT_FAILURE);
}

uint8_t *readFile(const char *filename, size_t *length)
{
    FILE *file = fopen(filename, "rb");
    if (!file) handleErrors("Ошибка при открытии файла для чтения");

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *data = (uint8_t *)malloc(*length);
    if (data == 0) handleErrors("Ошибка выделения памяти");

    fread(data, 1, *length, file);
    fclose(file);

    return data;
}

void writeFile(const char *filename, uint8_t *data, size_t length)
{
    FILE *file = fopen(filename, "wb");
    if (!file) handleErrors("Ошибка при открытии файла для записи");
    fwrite(data, 1, length, file);
    fclose(file);
}

size_t addDataForBlock(uint8_t **data, size_t length)
{
    size_t padded_length = (length + 7) / 8 * 8;
    uint8_t *padded_data = (uint8_t *)malloc(padded_length);
    memcpy(padded_data, *data, length);
    memset(padded_data + length, 0, padded_length - length);
    *data = padded_data;
    return padded_length;
}

void codingDataFromFile(const char *input_filename, const char *output_filename, BlowfishKey *key, int crypto_mode)
{
    size_t length;
    uint8_t *data = readFile(input_filename, &length);
    const size_t all_lenght = crypto_mode ? addDataForBlock(&data, length) : length;

    for (size_t i = 0; i < all_lenght; i += 8)
    {
        uint32_t L = (data[i] << 24) | (data[i+1] << 16) | (data[i+2] << 8) | data[i+3];
        uint32_t R = (data[i+4] << 24) | (data[i+5] << 16) | (data[i+6] << 8) | data[i+7];

        crypto_mode ? encrypt(&L, &R, key) : decrypt(&L, &R, key);

        data[i] = (L >> 24) & 0xFF;
        data[i+1] = (L >> 16) & 0xFF;
        data[i+2] = (L >> 8) & 0xFF;
        data[i+3] = L & 0xFF;
        data[i+4] = (R >> 24) & 0xFF;
        data[i+5] = (R >> 16) & 0xFF;
        data[i+6] = (R >> 8) & 0xFF;
        data[i+7] = R & 0xFF;
    }

    writeFile(output_filename, data, all_lenght);
    free(data);
}

int isValidHexString(const char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isxdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

void enteryKey( BlowfishKey *key )
{
    printf("Введите 8-байтный ключ (в шестнадцатеричном формате, без '0x', например: 0011223344556677): ");
    char key_input[17];
    scanf("%16s", key_input);

    if (strlen(key_input) != 16 || !isValidHexString(key_input))
        handleErrors("Введенный ключ некорректный");

    uint8_t key_data[8];
    for (int i = 0; i < 8; i++) {
        sscanf(key_input + 2 * i, "%2hhx", &key_data[i]);
    }
    initKey(key, key_data, sizeof(key_data));
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Использование: %s <encrypt|decrypt> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "encrypt") != 0 && strcmp(argv[1], "decrypt") != 0) {
        fprintf(stderr, "Неправильный аргумент операции: %s. Используйте 'encrypt' или 'decrypt'.\n", argv[1]);
        return 1;
    }

    BlowfishKey key;
    // = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    enteryKey( &key );
    codingDataFromFile(argv[2], argv[3], &key, strcmp(argv[1], "encrypt") == 0 ? 1 : 0 );
    printf("Операция выполнена успешно.\n");
    return 0;
}
