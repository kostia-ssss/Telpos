#ifndef COMMANDS_H
#define COMMANDS_H

#include <windows.h>

typedef struct Command {
    char *name;
    int (*func)(int argc, char *argv[]);
} Command;

extern Command *commands; 
extern int commandsCount; 
extern char baseDir[MAX_PATH];

void addCommand(char *name, int (*func)(int argc, char *argv[]));

int help(int argc, char *argv[]);
int echo(int argc, char *argv[]);
int logo(int argc, char *argv[]);
int theme(int argc, char *argv[]);
int clear(int argc, char *argv[]);
int timer(int argc, char *argv[]);
int time_(int argc, char *argv[]);
int history(int argc, char *argv[]);
int clear_(void);

#endif
