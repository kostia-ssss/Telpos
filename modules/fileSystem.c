#include <stdio.h>
#include <windows.h>
#include "../utils/utils.h"
#include "../source/commands.h"

int touch(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: touch <filename>\n");
        return 0;
    }

    FILE *f = fopen(argv[1], "a"); // створює, якщо нема
    if (!f) {
        printf("Cannot create file: %s\n", argv[1]);
        return 0;
    }

    fclose(f);
    return 0;
}

int cat(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: cat <filename>\n");
        return 0;
    }

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

    return 0;
}

void init_fs() {
    addCommand("touch", touch);
    addCommand("cat", cat);
    addCommand("ls", ls);
    addCommand("rm", rm);
    addCommand("cd", cd);
    addCommand("mkdir", mkdir_cmd);
    addCommand("rmdir", rmdir_cmd);
    addCommand("write", write);
}