#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

char* readConfigValue(const char *path, const char *key) {
    FILE *file = fopen(path, "r");
    if (!file) return NULL;

    char line[256];
    size_t keyLen = strlen(key);

    while (fgets(line, sizeof(line), file)) {

        // Пропускаємо коментарі або пусті строки
        if (line[0] == '#' || line[0] == '\n') continue;

        // Чи починається строка з ключа?
        if (strncmp(line, key, keyLen) == 0 && line[keyLen] == '=') {

            // Знаходимо начало value
            char *value = line + keyLen + 1;

            // Видаляємо \n
            value[strcspn(value, "\r\n")] = 0;

            // Копія значення
            char *result = malloc(strlen(value) + 1);
            strcpy(result, value);

            fclose(file);
            return result;
        }
    }

    fclose(file);
    return NULL; // ключ не знайдено
}

int writeConfigValue(const char *path, const char *key, const char *value) {
    FILE *file = fopen(path, "r");
    if (!file) return 0;

    // Читаємо весь файл в пам'ять
    char lines[100][256];
    int count = 0;
    int found = 0;

    while (fgets(lines[count], sizeof(lines[count]), file) && count < 100) {
        size_t keyLen = strlen(key);
        if (strncmp(lines[count], key, keyLen) == 0 && lines[count][keyLen] == '=') {
            // Заміна значення
            snprintf(lines[count], sizeof(lines[count]), "%s=%s\n", key, value);
            found = 1;
        }
        count++;
    }
    fclose(file);

    // Якщо ключ не знайдено, додаємо його в кінець
    if (!found) {
        snprintf(lines[count], sizeof(lines[count]), "%s=%s\n", key, value);
        count++;
    }

    // Записуємо назад у файл
    file = fopen(path, "w");
    if (!file) return 0;

    for (int i = 0; i < count; i++) {
        fputs(lines[i], file);
    }

    fclose(file);
    return 1;
}


int colorNameToCode(const char *name) {
    if (strcmp(name, "black") == 0) return 0;
    if (strcmp(name, "red") == 0) return 4;
    if (strcmp(name, "green") == 0) return 2;
    if (strcmp(name, "yellow") == 0) return 6;
    if (strcmp(name, "blue") == 0) return 1;
    if (strcmp(name, "magenta") == 0) return 5;
    if (strcmp(name, "cyan") == 0) return 3;
    if (strcmp(name, "white") == 0) return 7;
    return 7; // стандартний
}