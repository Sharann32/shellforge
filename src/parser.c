#include "parser.h"
#include <stdio.h>

void parser_init(Parser *parser, Token *tokens, int count)
{
    parser->tokens = tokens;
    parser->count = count;
    parser->pos = 0;
}

void parser_parse(Parser *parser)
{
    while (parser->pos < parser->count) {
        Token token = parser->tokens[parser->pos];

        if (token.type == TOKEN_END) {
            break;
        }

        printf("PARSE: %-18s %s\n",
               token_type_name(token.type),
               token.value);

        parser->pos++;
    }
}
