#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_AMPERSAND,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token create_token(TokenType type, const char *value);
const char *token_type_name(TokenType type);

#endif
