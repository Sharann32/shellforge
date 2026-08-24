#include "expand.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void expand_tokens(Token *tokens, int count)
{
    for (int i = 0; i < count; i++) {
        if (tokens[i].type != TOKEN_WORD || tokens[i].value == NULL)
            continue;

        if (tokens[i].value[0] == '$') {
            const char *name = tokens[i].value + 1;
            const char *value = getenv(name);

            if (value != NULL) {
                free(tokens[i].value);
                tokens[i].value = strdup(value);
            }
        }
    }
}
