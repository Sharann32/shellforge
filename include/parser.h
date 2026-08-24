#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct {
    Token *tokens;
    int count;
    int pos;
} Parser;

void parser_init(Parser *parser, Token *tokens, int count);
void parser_parse(Parser *parser);

#endif
