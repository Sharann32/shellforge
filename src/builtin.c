#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int is_builtin(const char *command)
{
    if (command == NULL)
        return 0;

    return strcmp(command, "cd") == 0 ||
           strcmp(command, "pwd") == 0 ||
           strcmp(command, "echo") == 0 ||
           strcmp(command, "exit") == 0;
}

int execute_builtin(char **argv)
{
    if (argv == NULL || argv[0] == NULL)
        return 0;

    if (strcmp(argv[0], "cd") == 0) {
        const char *dir = argv[1];

        if (dir == NULL)
            dir = getenv("HOME");

        if (chdir(dir) != 0)
            perror("cd");

        return 1;
    }

    if (strcmp(argv[0], "pwd") == 0) {
        char cwd[4096];

        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("pwd");

        return 1;
    }

    if (strcmp(argv[0], "echo") == 0) {
        for (int i = 1; argv[i] != NULL; i++) {
            if (i > 1)
                printf(" ");

            printf("%s", argv[i]);
        }

        printf("\n");
        return 1;
    }

    if (strcmp(argv[0], "exit") == 0) {
        printf("Exiting...\n");
        exit(0);
    }

    return 0;
}
