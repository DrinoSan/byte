#include <_types/_uint8_t.h>
#include <cstdlib>
#include <stdio.h>

#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "scanner.h"

void Parser::errorAtCurrent( const char* message )
{
    errorAt( &current, message );
}

void Parser::error( const char* message )
{
    errorAt( &previous, message );
}

void Parser::errorAt( Token* token, const char* message )
{
    if ( panicMode )
        return;
    panicMode = true;
    fprintf( stderr, "[line %d] Error", token->line );

    if ( token->type == TokenType::TOKEN_EOF )
    {
        fprintf( stderr, " at end" );
    }
    else if ( token->type == TokenType::TOKEN_ERROR )
    {
        // Nothing.
    }
    else
    {
        fprintf( stderr, " at '%.*s'", token->length, token->start );
    }

    fprintf( stderr, ": %s\n", message );
    hadError = true;
}

///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////

bool Compiler::compile( const char* source, Chunk* chunk )
{
    scanner.initScanner( source );
    compilingChunk = chunk;

    parser.hadError  = false;
    parser.panicMode = false;

    advance();
    expression();
    consume( TokenType::TOKEN_EOF, "Expected end of expression," );

    endCompiler();
    return !parser.hadError;
}

void Compiler::advance()
{
    parser.previous = parser.current;

    for ( ;; )
    {
        parser.current = scanner.scanToken();
        if ( parser.current.type != TokenType::TOKEN_ERROR )
            break;

        parser.errorAtCurrent( parser.current.start );
    }
}

void Compiler::consume( TokenType type, const char* message )
{
    if ( parser.current.type == type )
    {
        advance();
        return;
    }

    parser.errorAtCurrent( message );
}

void Compiler::emitByte( uint8_t byte )
{
    currentChunk()->writeChunk( byte, parser.previous.line );
}

void Compiler::emitBytes( uint8_t byte1, uint8_t byte2 )
{
    emitByte( byte1 );
    emitByte( byte2 );
}

Chunk* Compiler::currentChunk()
{
    return compilingChunk;
}

void Compiler::endCompiler()
{
    emitReturn();
}

void Compiler::grouping()
{
    expression();
    consume( TokenType::TOKEN_RIGHT_PAREN, "Expect ')' after expression." );
}

void Compiler::number()
{
    double value = strtod( parser.previous.start, NULL );
    emitConstant( value );
}

void Compiler::unary()
{
    TokenType operatorType = parser.previous.type;

    // Compile the operand
    expression();

    switch ( operatorType )
    {
    case TokenType::TOKEN_MINUS:
    {
        emitByte( OP_NEGATE );
        break;
    }
    default:
    {
        return;
    }
    }
}

void Compiler::emitReturn()
{
    emitByte( OP_RETURN );
}

int32_t Compiler::makeConstant( Value value )
{
    return currentChunk()->writeConstant( value, scanner.line );
}

void Compiler::emitConstant( Value value )
{
    emitBytes( OP_CONSTANT, makeConstant( value ) );
}

void Compiler::expression() {}