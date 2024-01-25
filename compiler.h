#pragma once

#include "chunk.h"
#include "scanner.h"

class Parser
{
  public:
    Parser()  = default;
    ~Parser() = default;

  public:
    void errorAtCurrent( const char* message );

    void error( const char* message );
    void errorAt( Token* token, const char* message );

  public:
    Token current;
    Token previous;
    bool  hadError;
    bool  panicMode;
};

enum class Precedence
{
    PREC_NONE,
    PREC_ASSIGNMENT,   // =
    PREC_OR,           // or
    PREC_AND,          // and
    PREC_EQUALITY,     // == !=
    PREC_COMPARISON,   // < > <= >=
    PREC_TERM,         // + -
    PREC_FACTOR,       // * /
    PREC_UNARY,        // ! -
    PREC_CALL,         // . ()
    PREC_PRIMARY
};

class Compiler
{
  public:
    Compiler() = default;
    Compiler( Scanner scanner, Parser parser, Chunk* compilingChunk )
        : scanner( scanner ), parser( parser ), compilingChunk( compilingChunk )
    {
    }
    ~Compiler() = default;

    bool compile( const char* source, Chunk* chunk );
    void advance();
    void consume( TokenType type, const char* message );
    void emitByte( uint8_t byte );
    void emitBytes( uint8_t byte1, uint8_t byte2 );

    Chunk* currentChunk();

    void endCompiler();
    void binary();
    void grouping();
    void number();
    void unary();
    void parsePrecedence( Precedence precedence );

    void    expression();
    void    emitReturn();
    int32_t makeConstant( Value value );
    void    emitConstant( Value value );

  private:
    Scanner scanner;
    Parser  parser;
    Chunk*  compilingChunk;
};