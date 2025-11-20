#include <stdio.h>
#include <windows.h>
#include "../utils/utils.h"
#include "../source/commands.h"

int create(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: create <filename>\n");
        return 0;
    }

    FILE *f = fopen(argv[1], "a"); // створює, якщо нема
    if (!f) {
        printf("Cannot create file: %s\n", argv[1]);
        return 0;
    }

    fclose(f);
    writeHistory("create");
    return 0;
}

int read(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Usage: read <filename>\n");
        printf("Usage: read <filename> <line number>\n");
        return 0;
    }
    else if (argc == 2) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            printf("File not found: %s\n", argv[1]);
            return 0;
        }

        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file)) {
            printf("%s", buffer);
        }

        fclose(file);
        writeHistory("read");
    }
    else if (argc == 3) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            printf("Cannot open file\n");
            return 1;
        }

        int n = atoi(argv[2]);
        char buffer[256]; 

        int current = 1;
        while (fgets(buffer, sizeof(buffer), f)) {
            if (current == n) {
                printf(buffer);
                break;
            }
            current++;
        }

        fclose(f);
        writeHistory("read");
    } 
    return 0;
}

int ls(int argc, char *argv[]) {
    WIN32_FIND_DATAA data;
    HANDLE h = FindFirstFileA("*", &data);

    if (h == INVALID_HANDLE_VALUE) {
        printf("Cannot list directory.\n");
        return 0;
    }

    do {
        printf("%s\n", data.cFileName);
    } while (FindNextFileA(h, &data));

    FindClose(h);
    writeHistory("ls");
    return 0;
}

int rm(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: rm <filename>\n");
        return 0;
    }

    if (DeleteFileA(argv[1])) {
        printf("Deleted: %s\n", argv[1]);
    } else {
        printf("Cannot delete: %s\n", argv[1]);
    }
    writeHistory("rm");

    return 0;
}

int mkdir_cmd(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: mkdir <dirname>\n");
        return 0;
    }

    if (CreateDirectoryA(argv[1], NULL)) {
        printf("Directory created: %s\n", argv[1]);
    } else {
        printf("Cannot create directory: %s\n", argv[1]);
    }
    writeHistory("mkdir");

    return 0;
}

int rmdir_cmd(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: rmdir <dirname>\n");
        return 0;
    }

    if (RemoveDirectoryA(argv[1])) {
        printf("Directory removed: %s\n", argv[1]);
    } else {
        printf("Cannot remove directory: %s\n", argv[1]);
    }
    writeHistory("rmdir");

    return 0;
}

int cd(int argc, char *argv[]) {
    if (argc < 2) {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        printf("%s\n", buffer);
        return 0;
    }

    if (!SetCurrentDirectoryA(argv[1])) {
        printf("cd: cannot access '%s'\n", argv[1]);
        return 1;
    }
    writeHistory("cd");

    return 0;
}

int write(int argc, char *argv[]) {
    if (argc < 3) {
        printEvent("Usage: write <text> <file>", "ERROR", "red");
        printEvent("       write <text> -> <file>", "ERROR", "red");
        return 1;
    }

    int index = -1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "->") == 0) {
            index = i;
            break;
        }
    }

    char *filename;
    int start = 1;
    if (index != -1) {
        if (index + 1 >= argc) {
            printEvent("No file specified after ->", "ERROR", "red");
            return 1;
        }
        filename = argv[index + 1];
    } else {
        filename = argv[argc - 1];
        index = argc - 1;
    }

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printEvent("Cannot open file", "ERROR", "red");
        return 1;
    }

    for (int i = start; i < index; i++) {
        fputs(argv[i], file);
        if (i < index - 1) fputc(' ', file);
    }

    fputc('\n', file);
    fclose(file);
    writeHistory("write");

    return 0;
}

int rename_(int argc, char *argv[]) {
    if (argc == 3) rename(argv[1], argv[2]);
    else printEvent("ERROR", "Usage: <oldFileName> <newFileName>", "red");
    writeHistory("rename");
    return 0;
}

int countLines(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: countLines <filename>\n");
        return 1;
    }

    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s/%s", baseDir, argv[1]);

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("[ERROR] Cannot open file: %s\n", argv[1]);
        return 1;
    }

    int lines = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }

    fseek(file, -1, SEEK_END);
    ch = fgetc(file);
    if (ch != '\n') {
        lines++;
    }

    fclose(file);

    writeHistory("cl");
    printf("%d\n", lines);
    return 0;
}

int copy(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: copy <source> <destination>\n");
        return 0;
    }

    char pathFrom[MAX_PATH];
    char pathTo[MAX_PATH];

    // Формуємо абсолютні шляхи
    snprintf(pathFrom, sizeof(pathFrom), "%s/%s", baseDir, argv[1]);
    snprintf(pathTo, sizeof(pathTo), "%s/%s", baseDir, argv[2]);

    FILE *src = fopen(pathFrom, "rb");
    if (!src) {
        printf("File not found: %s\n", argv[1]);
        return 0;
    }

    FILE *dst = fopen(pathTo, "wb");
    if (!dst) {
        fclose(src);
        printf("Cannot create file: %s\n", argv[2]);
        return 0;
    }

    char buffer[4096];
    size_t bytes;

    // Копіюємо блоками
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dst);
    }

    fclose(src);
    fclose(dst);

    // Записуємо в історію
    char histPath[MAX_PATH];
    snprintf(histPath, sizeof(histPath), "%s/%s", baseDir, "data/history.txt");
    writeToFile("copy", histPath);

    printf("Copied: %s -> %s\n", argv[1], argv[2]);
    return 1;
}

void init_fs() {
    addCommand("create", create);
    addCommand("read", read);
    addCommand("ls", ls);
    addCommand("rm", rm);
    addCommand("cd", cd);
    addCommand("mkdir", mkdir_cmd);
    addCommand("rmdir", rmdir_cmd);
    addCommand("write", write);
    addCommand("rename", rename_);
    addCommand("cl", countLines);
    addCommand("copy", copy);
}