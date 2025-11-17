#ifndef COMMANDS_H
#define COMMANDS_H

typedef struct Command {
    char *name;
    int (*func)(int argc, char *argv[]);
} Command;

extern Command *commands;    // масив структур
extern int commandsCount;    // кількість команд

void addCommand(char *name, int (*func)(int argc, char *argv[]));

int help(int argc, char *argv[]);
int echo(int argc, char *argv[]);
int logo(int argc, char *argv[]);
int theme(int argc, char *argv[]);
int clear(int argc, char *argv[]);
int timer(int argc, char *argv[]);
int clear_(void);

#endif
