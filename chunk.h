#pragma once

#include "common.h"
#include "memory.h"
#include "value.h"
#include <_types/_uint32_t.h>
#include <sys/_types/_int8_t.h>

enum OpCode
{
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_RETURN,
};

// Representation of a sequence of bytecode
class Chunk
{
  public:
    Chunk();
    ~Chunk();

    void initChunk();
    void freeChunk();

    template <typename T>
    void writeChunk( T byte, int line )
    {
        if ( capacity < count + 1 )
        {
            int oldCapacity = capacity;
            capacity        = GROW_CAPACITY( oldCapacity );
            code            = GROW_ARRAY( code, oldCapacity, capacity );
            lines           = GROW_ARRAY( lines, oldCapacity, capacity );
        }

        code[ count ]  = byte;
        lines[ count ] = line;
        count++;
    }

    void writeConstant( Value value, int line );

    int     addConstant( Value value );
    uint8_t getOpCodebyOffset( int offset );

    int8_t getConstantIndexByOpCodeOffset( int offset );
    Value  getConstantsValues( int constant );

    int32_t getLongConstantIndexByOpCodeOffset( int offset );
    Value   getLoncConstantValues( uint32_t constant );
    void    printValue( Value value );

  public:
    int      count;      // How many entries are used
    int      capacity;   // Max capacity of entries
    int*     lines;      // Line information
    uint8_t* code;       // Instructions

  private:
    ValueArray constants;   // Array to hold data of chunk
};