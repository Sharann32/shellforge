#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        if (token.type == TOKEN_END)
            break;

        printf("PARSE: %-18s %s\n",
               token_type_name(token.type),
               token.value);

        parser->pos++;
    }
}

int parse_command(Token *tokens, int count, Command *command)
{
    if (tokens == NULL || command == NULL)
        return -1;

    memset(command, 0, sizeof(Command));

    for (int i = 0; i < count; i++) {

        Token *token = &tokens[i];

        if (token->type == TOKEN_END)
            break;

        if (token->type == TOKEN_WORD) {

            if (command->argc >= MAX_ARGS - 1)
                return -1;

            command->argv[command->argc++] = token->value;
            continue;
        }

        if (token->type == TOKEN_REDIRECT_IN) {

            if (i + 1 >= count ||
                tokens[i + 1].type != TOKEN_WORD)
                return -1;

            command->input_file = tokens[++i].value;
            continue;
        }

        if (token->type == TOKEN_REDIRECT_OUT) {

            if (i + 1 >= count ||
                tokens[i + 1].type != TOKEN_WORD)
                return -1;

            command->output_file = tokens[++i].value;
            command->append = 0;
            continue;
        }

        if (token->type == TOKEN_REDIRECT_APPEND) {

            if (i + 1 >= count ||
                tokens[i + 1].type != TOKEN_WORD)
                return -1;

            command->output_file = tokens[++i].value;
            command->append = 1;
            continue;
        }

        if (token->type == TOKEN_AMPERSAND) {
            command->background = 1;
            continue;
        }

        if (token->type == TOKEN_PIPE) {
            return -1;
        }
    }

    command->argv[command->argc] = NULL;

    return command->argc > 0 ? 0 : -1;
}

void free_command(Command *command)
{
    if (command == NULL)
        return;

    memset(command, 0, sizeof(Command));
}
