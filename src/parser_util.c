#include "postgres.h"
#include "parser_util.h"

Token construct_token(TokenType type, const char* token, int32 lenght) {
    return (Token){type, token, lenght};
}