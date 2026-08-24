#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "lexer.h"

int main(void)
{
    char *input;

    printf("================================\n");
    printf("          Shellforge\n");
    printf("   A Unix Style Shell written in C\n");
    printf("================================\n");

    while ((input = readline("shellforge$ ")) != NULL) {

        if (input[0] == '\0') {
            free(input);
            continue;
        }

        if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            free(input);
            break;
        }

        Lexer lexer;
        lexer_init(&lexer, input);

        int index = 0;

        while (1) {
            Token token = lexer_next_token(&lexer);

            printf("%d : %-18s %s\n",
                   index,
                   token_type_name(token.type),
                   token.value);

            free(token.value);

            index++;

            if (token.type == TOKEN_END)
                break;
        }

        free(input);
    }

    return 0;
}
