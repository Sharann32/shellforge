#include <stdlib.h>
#include <string.h>
#include "token.h"

Token create_token(TokenType type, const char *value)
{
    Token token;

    token.type = type;
    token.value = strdup(value ? value : "");

    return token;
}

const char *token_type_name(TokenType type)
{
    switch (type) {
        case TOKEN_WORD:
            return "WORD";
        case TOKEN_PIPE:
            return "PIPE";
        case TOKEN_AMPERSAND:
            return "AMPERSAND";
        case TOKEN_REDIRECT_IN:
            return "REDIRECT_IN";
        case TOKEN_REDIRECT_OUT:
            return "REDIRECT_OUT";
        case TOKEN_REDIRECT_APPEND:
            return "REDIRECT_APPEND";
        case TOKEN_END:
            return "END";
        default:
            return "UNKNOWN";
    }
}
