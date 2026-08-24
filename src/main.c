#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "history.h"

int main(void)
{
    char *input;

    printf("================================\n");
    printf("          Shellforge\n");
    printf("   A Unix Style Shell written in C\n");
    printf("================================\n\n");

    while (1)
    {
        input = readline("shellforge$ ");

        if (input == NULL)
        {
            printf("\nExiting...\n");
            break;
        }

        if (strlen(input) == 0)
        {
            free(input);
            continue;
        }

        printf("YOU ENTERED: %s\n", input);

        add_history(input);

        if (strcmp(input, "history") == 0)
        {
            print_history();
        }
        else if (strcmp(input, "exit") == 0)
        {
            free(input);
            printf("Exiting...\n");
            break;
        }

        free(input);
    }

    free_history();

    return 0;
}
