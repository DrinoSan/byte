#pragma once

#include "chunk.h"
#include "scanner.h"
#include "vm.h"
#include <_types/_uint8_t.h>

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

class Compiler
{
  public:
    Compiler()  = default;
    ~Compiler() = default;

    bool compile( const char* source, Chunk* chunk );
    void advance();
    void consume( TokenType type, const char* message );
    void emitByte( uint8_t byte );
    void emitBytes( uint8_t byte1, uint8_t byte2 );

    Chunk* currentChunk();

    void endCompiler();
    void grouping();
    void number();
    void unary();
    void expression();
    void emitReturn();
    int32_t makeConstant(Value value);
    void emitConstant(Value value);

  private:
    Scanner scanner;
    Parser  parser;
    Chunk*  compilingChunk;
};