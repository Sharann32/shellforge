#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "history.h"
#include "lexer.h"
#include "parser.h"
#include "expand.h"
#include "builtin.h"
#include "executor.h"

int main(void)
{
    char *input;

    printf("================================\n");
    printf("          Shellforge\n");
    printf("   A Unix Style Shell written in C\n");
    printf("================================\n");

    while ((input = readline("shellforge$ ")) != NULL) {

        if (strlen(input) == 0) {
            free(input);
            continue;
        }

        add_history(input);

        int count = 0;
        Token *tokens = tokenize(input, &count);

        if (tokens == NULL) {
            free(input);
            continue;
        }

        expand_tokens(tokens, count);

        Command command;

        if (parse_command(tokens, count, &command) == 0) {

            if (is_builtin(command.argv[0])) {
                execute_builtin(command.argv);
            } else {
                execute_command(&command);
            }

            free_command(&command);
        }

        free_tokens(tokens, count);
        free(input);
    }

    printf("Exiting...\n");

    return 0;
}
