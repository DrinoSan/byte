#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

void Compiler::compile(const char* source)
{
    scanner.initScanner(source);
    int line = -1;
    for(;;)
    {
        Token token = scanner.scanToken();
        if( scanner.line != line )
        {
            printf("%4d ", token.line);
            line = token.line;
        }
        else
        {
            printf("    | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if( token.type == TokenType::TOKEN_EOF )
            break;
    }
}