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

#define MAX_COMMANDS 16

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

        int has_pipe = 0;

        for (int i = 0; i < count; i++) {
            if (tokens[i].type == TOKEN_PIPE) {
                has_pipe = 1;
                break;
            }
        }

        if (has_pipe) {

            Command commands[MAX_COMMANDS];
            int command_count = 0;

            if (parse_pipeline(tokens, count,
                               commands, &command_count) == 0) {

                execute_pipeline(commands, command_count);

                for (int i = 0; i < command_count; i++)
                    free_command(&commands[i]);

            } else {
                fprintf(stderr, "Invalid pipeline\n");
            }

        } else {

            Command command;

            if (parse_command(tokens, count, &command) == 0) {

                if (is_builtin(command.argv[0]))
                    execute_builtin(command.argv);
                else
                    execute_command(&command);

                free_command(&command);
            }
        }

        free_tokens(tokens, count);
        free(input);
    }

    printf("Exiting...\n");

    return 0;
}
