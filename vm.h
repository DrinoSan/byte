#pragma once

#include "chunk.h"


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

    InterpretResult interpret(Chunk* chunk_);
    InterpretResult run();

    static VM& getInstance()
    {
        static VM instance;
        return instance;
    }

    private:
     Chunk* chunk;
     uint8_t* ip;
};