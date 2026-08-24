#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void lexer_init(Lexer *lexer, const char *input)
{
    lexer->input = input;
    lexer->pos = 0;
    lexer->length = input ? strlen(input) : 0;
}

static char lexer_peek(Lexer *lexer)
{
    if (lexer->pos >= lexer->length)
        return '\0';

    return lexer->input[lexer->pos];
}

static char lexer_advance(Lexer *lexer)
{
    char c = lexer_peek(lexer);

    if (c != '\0')
        lexer->pos++;

    return c;
}

static void lexer_skip_whitespace(Lexer *lexer)
{
    while (lexer->pos < lexer->length &&
           isspace((unsigned char)lexer->input[lexer->pos])) {
        lexer->pos++;
    }
}

Token lexer_next_token(Lexer *lexer)
{
    lexer_skip_whitespace(lexer);

    char c = lexer_peek(lexer);

    if (c == '\0')
        return create_token(TOKEN_END, "END");

    if (c == '|') {
        lexer_advance(lexer);
        return create_token(TOKEN_PIPE, "|");
    }

    if (c == '&') {
        lexer_advance(lexer);
        return create_token(TOKEN_AMPERSAND, "&");
    }

    if (c == '<') {
        lexer_advance(lexer);
        return create_token(TOKEN_REDIRECT_IN, "<");
    }

    if (c == '>') {
        lexer_advance(lexer);

        if (lexer_peek(lexer) == '>') {
            lexer_advance(lexer);
            return create_token(TOKEN_REDIRECT_APPEND, ">>");
        }

        return create_token(TOKEN_REDIRECT_OUT, ">");
    }

    /* Quoted string */
    if (c == '"' || c == '\'') {
        char quote = c;
        lexer_advance(lexer);

        size_t start = lexer->pos;

        while (lexer->pos < lexer->length &&
               lexer->input[lexer->pos] != quote) {
            lexer->pos++;
        }

        size_t len = lexer->pos - start;

        char *value = malloc(len + 1);
        if (!value)
            return create_token(TOKEN_WORD, "");

        memcpy(value, lexer->input + start, len);
        value[len] = '\0';

        if (lexer_peek(lexer) == quote)
            lexer_advance(lexer);

        Token token = create_token(TOKEN_WORD, value);

        free(value);

        return token;
    }

    /* Regular word */
    size_t start = lexer->pos;

    while (lexer->pos < lexer->length) {
        char next = lexer->input[lexer->pos];

        if (isspace((unsigned char)next) ||
            next == '|' ||
            next == '&' ||
            next == '<' ||
            next == '>' ||
            next == '"' ||
            next == '\'') {
            break;
        }

        lexer->pos++;
    }

    size_t len = lexer->pos - start;

    char *value = malloc(len + 1);
    if (!value)
        return create_token(TOKEN_WORD, "");

    memcpy(value, lexer->input + start, len);
    value[len] = '\0';

    Token token = create_token(TOKEN_WORD, value);

    free(value);

    return token;
}

/* Convert input into token array */
Token *tokenize(const char *input, int *count)
{
    if (!input || !count)
        return NULL;

    Lexer lexer;
    lexer_init(&lexer, input);

    int capacity = 16;
    int size = 0;

    Token *tokens = malloc(sizeof(Token) * capacity);

    if (!tokens)
        return NULL;

    while (1) {
        Token token = lexer_next_token(&lexer);

        if (size >= capacity) {
            capacity *= 2;

            Token *temp = realloc(
                tokens,
                sizeof(Token) * capacity
            );

            if (!temp) {
                free_tokens(tokens, size);
                return NULL;
            }

            tokens = temp;
        }

        tokens[size++] = token;

        if (token.type == TOKEN_END)
            break;
    }

    *count = size;

    return tokens;
}

/* Free token array */
void free_tokens(Token *tokens, int count)
{
    if (!tokens)
        return;

    for (int i = 0; i < count; i++)
        free(tokens[i].value);

    free(tokens);
}
