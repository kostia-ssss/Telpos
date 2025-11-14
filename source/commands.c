#include "commands.h"
#include <stdio.h>

Command *commands = NULL;
int commandsCount = 0;

void addCommand(char *name, int (*func)(int argc, char *argv[]))
{
    commands = realloc(commands, sizeof(Command) * (commandsCount + 1));
    commands[commandsCount].name = name;
    commands[commandsCount].func = func;
    commandsCount++;
}

int help(int argc, char *argv[]) {
    printf("Commands available:\n");
    printf("help - show this message\n");
    printf("echo - repeat what you type\n");
    printf("logo - show Telpos logo\n");
    printf("exit - exit the terminal\n");
    return 0;
}

int echo(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {  // починаємо з 1, бо argv[0] - команда
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}

int logo(int argc, char *argv[]) {
    printf("██████  ██████  ██      ██████  ██████  ██████\n");
    printf("  ██    ██      ██      ██  ██  ██  ██  ██    \n");
    printf("  ██    ██████  ██      ██████  ██  ██  ██████\n");
    printf("  ██    ██      ██      ██      ██  ██      ██\n");
    printf("  ██    ██████  ██████  ██      ██████  ██████\n");
    return 0;
}

Command commands[] = {
    {"help", help},
    {"echo", echo},
    {"logo", logo},
};
int commands_count = sizeof(commands) / sizeof(commands[0]);
