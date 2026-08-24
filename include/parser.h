#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define MAX_ARGS 64

typedef struct {
    char *argv[MAX_ARGS];
    int argc;

    char *input_file;
    char *output_file;

    int append;
    int background;
} Command;

typedef struct {
    Token *tokens;
    int count;
    int pos;
} Parser;

void parser_init(Parser *parser, Token *tokens, int count);
void parser_parse(Parser *parser);

int parse_command(Token *tokens, int count, Command *command);
void free_command(Command *command);

#endif
