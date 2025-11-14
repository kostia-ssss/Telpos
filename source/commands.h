#ifndef COMMANDS_H
#define COMMANDS_H

void addCommand(char *name, int (*func)(int argc, char *argv[]));

int help(int argc, char *argv[]);
int echo(int argc, char *argv[]);
int logo(int argc, char *argv[]);

typedef struct Command {
    char *name;
    int (*func)(int argc, char *argv[]);
} Command;

extern Command commands[];    // масив структур
extern int commands_count;    // кількість команд

#endif
