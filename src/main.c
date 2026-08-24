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

        char *argv[128];
        int argc = 0;

        for (int i = 0; i < count && argc < 127; i++) {
            if (tokens[i].type != TOKEN_WORD)
                break;

            argv[argc++] = tokens[i].value;
        }

        argv[argc] = NULL;

        if (argc > 0) {
            if (is_builtin(argv[0])) {
                execute_builtin(argv);
            } else {
                execute_external(argv);
            }
        }

        free_tokens(tokens, count);
        free(input);
    }

    return 0;
}
