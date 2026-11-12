// re2c --lang c # vscode плагин
#include "postgres.h"
#include "utilfunc.h"
#include "scaner.h"
#include "parser_util.h"

int scaner(char* predicate_string, int32 len, Token* result, int32 token_len) {
    int32 token_position = 0;
    char *YYCURSOR = predicate_string;
    char *YYLIMIT = predicate_string + len;
    char *YYTOKEN = YYCURSOR;
    char *YYMARKER;

    memset(result, 0, token_len * sizeof(*result));
    for(;;) {
        YYTOKEN = YYCURSOR;
        /*!re2c
            re2c:yyfill:enable = 0;
            re2c:define:YYCTYPE = char;
            re2c:define:YYLIMIT = YYLIMIT;

            " " {continue;}
            [_a-zA-Z]+ [_a-zA-Z0-9]* {
                result[token_position++] =
                    construct_token(T_VARIABLE, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;
            }

            "%" {
                result[token_position++] = 
                    construct_token(T_MOD, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;
            }
            "==" {
                result[token_position++] = 
                    construct_token(T_EQ, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;
            }
            "!=" {
                result[token_position++] = 
                    construct_token(T_NEQ, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;
            }
            "<=" {
                result[token_position++] = 
                    construct_token(T_LEE, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;
            }
            ">=" {
                result[token_position++] = 
                    construct_token(T_GRE, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;
            }
            "<" {
                result[token_position++] = 
                    construct_token(T_LE, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;
            }
            ">" {
                result[token_position++] =
                    construct_token(T_GR, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;
            }
            "=" {
                result[token_position++] =
                    construct_token(T_SEQ, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;
            }
            ["] [^"\x00]* ["] {
                result[token_position++] =
                    construct_token(T_STRING, YYTOKEN+1, (int32)(YYCURSOR - YYTOKEN)-2);
                continue;
            }
            "'Infinity'" | "'-Infinity'" | "'NaN'" | "'Inf'" | "'-Inf'" {
                result[token_position++] =
                    construct_token(T_NUMBER, YYTOKEN+1, (int32)(YYCURSOR - YYTOKEN)-2);
                continue;    
            }
            "-"? [0-9]+ ("." [0-9]+)? {
                result[token_position++] =
                    construct_token(T_NUMBER, YYTOKEN, (int32)(YYCURSOR - YYTOKEN));
                continue;    
            }
            $ {
                result[token_position++] =
                    construct_token(T_END, NULL, 0);
                return token_position;
            }

            * {
                ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("lexical_analyze: invalid token: %.*s\nFATAL STRING: %.*s",
                        (int)(YYCURSOR - YYTOKEN), YYTOKEN,
                        len, predicate_string)));
            }
        */
    }
}


