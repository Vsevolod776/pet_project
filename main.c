#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

void handleErrors(void) {
    fprintf(stderr, "An error occurred\n");
    exit(1);
}

// Функция для считывания ключа от пользователя длиной 8 символов
void getKey(DES_cblock *key)
{
    char user_key[9];
    printf("Введите 8-символьный ключ: ");
    fgets(user_key, 9, stdin);
    user_key[strcspn(user_key, "\n")] = 0; // Удаляем символ новой строки
    memcpy(key, user_key, 8);
}

//Функция для обработки файла (шифрование/дешифрование)
void process_file(const char *input_file, const char *output_file, DES_key_schedule *schedule, int encrypt)
{
    FILE *in_file = fopen(input_file, "rb");
    FILE *out_file = fopen(output_file, "wb");

    if (!in_file || !out_file) {
        fprintf(stderr, "Ошибка при открытии файлов.\n");
        exit(1);
    }

    DES_cblock input;
    DES_cblock output;
    size_t n;

    while ((n = fread(input, 1, 8, in_file)) > 0) //Чтение данных блоками по 8 байт
    {
        if (n < 8)
            memset(input + n, 8 - n, 8 - n);

        if (encrypt) {
            DES_ecb_encrypt(&input, &output, schedule, DES_ENCRYPT);
        } else {
            DES_ecb_encrypt(&input, &output, schedule, DES_DECRYPT);
        }
        fwrite(output, 1, 8, out_file);
    }

    fclose(in_file);
    fclose(out_file);
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

    DES_cblock key;
    DES_key_schedule schedule;
    getKey(&key); // Получаем ключ от пользователя
    DES_set_key_checked(&key, &schedule); // Устанавливаем ключ
    process_file(argv[2], argv[3], &schedule, strcmp(argv[1], "encrypt") == 0 ? 1 : 0); // Обрабатываем файлы
    printf("Операция выполнена успешно.\n");
    return 0;
}
