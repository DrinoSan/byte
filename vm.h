#pragma once

#include "chunk.h"
#include "compiler.h"
#include "scanner.h"
#include "value.h"

#define STACK_MAX 256

enum class InterpretResult
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
};

class VM
{
    // Constructor is private due to singelton pattern
    // Object must be used via getInstance function
  private:
    VM();

  public:
    ~VM();

    void initVM();
    void freeVM();

    void  resetStack();
    void  runtimeError( const char* format, ... );
    void  push( Value value );
    Value pop();
    Value peek( int distance );

    InterpretResult interpret( const char* source );
    InterpretResult run();

    static VM& getInstance()
    {
        static VM instance;
        return instance;
    }

  private:
    Chunk*   chunk;
    uint8_t* ip;
    Value    stack[ STACK_MAX ];
    Value*   stackTop;
    Compiler compiler;
    Scanner  scanner;
};