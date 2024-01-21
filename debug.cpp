#include <stdio.h>

#include "chunk.h"
#include "debug.h"
#include "value.h"

void disassembleChunk( Chunk* chunk, const char* name )
{
    printf( "== %s ==\n", name );
    for ( int offset = 0; offset < chunk->count; )
    {
        offset = disassembleInstruction( chunk, offset );
    }
}

static int constantInstruction( const char* name, Chunk* chunk, int offset )
{
    uint8_t constant = chunk->getConstantIndexByOpCodeOffset( offset );
    printf( "%-16s %4d '", name, constant );
    chunk->printValue( chunk->getConstantsValues( constant ) );
    printf( "'\n" );
    return offset + 2;
}

static int longConstantInstruction( const char* name, Chunk* chunk, int offset )
{
    uint32_t constant = chunk->getLongConstantIndexByOpCodeOffset( offset );
    printf( "%-16s %4d '", name, constant );
    chunk->printValue( chunk->getLoncConstantValues( constant ) );
    printf( "'\n" );
    return offset + 4;
}

static int simpleInstruction( const char* name, int offset )
{
    printf( "%s\n", name );
    return offset + 1;
}

int disassembleInstruction( Chunk* chunk, int offset )
{
    // Priting offset in array of opcode
    // For return we do not print line number
    printf( "%04d ", offset );
    if ( offset > 0 && chunk->lines[ offset ] == chunk->lines[ offset - 1 ] )
    {
        printf( "   | " );
    }
    else
    {
        printf( "%4d ", chunk->lines[ offset ] );
    }

    uint8_t instruction = chunk->getOpCodebyOffset( offset );
    switch ( instruction )
    {
    case OP_CONSTANT:
        return constantInstruction( "OP_CONSTANT", chunk, offset );
    case OP_CONSTANT_LONG:
        return longConstantInstruction( "OP_CONSTANT_LONG", chunk, offset );
    // All the basic arithmetic instructions are simple like OP_RETURN they do
    // the operators take operands (which are on the stack) the arithmetic
    // bytecode instructions do not
    case OP_ADD:
        return simpleInstruction( "OP_ADD", offset );
    case OP_SUBTRACT:
        return simpleInstruction( "OP_SUBTRACT", offset );
    case OP_MULTIPLY:
        return simpleInstruction( "OP_MULTIPLY", offset );
    case OP_DIVIDE:
        return simpleInstruction( "OP_DIVIDE", offset );
    case OP_NEGATE:
        return simpleInstruction( "OP_NEGATE", offset );
    case OP_RETURN:
        return simpleInstruction( "OP_RETURN", offset );
    default:
        printf( "Unknown opcode %d\n", instruction );
        return offset + 1;
    }
}