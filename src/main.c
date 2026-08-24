#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "history.h"
#include "lexer.h"
#include "parser.h"
#include "expand.h"

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

        if (strcmp(input, "exit") == 0) {
            free(input);
            printf("Exiting...\n");
            break;
        }

        add_history(input);

        int count = 0;
        Token *tokens = tokenize(input, &count);

        if (tokens != NULL) {
            expand_tokens(tokens, count);

            Parser parser;
            parser_init(&parser, tokens, count);
            parser_parse(&parser);

            free_tokens(tokens, count);
        }

        free(input);
    }

    return 0;
}
