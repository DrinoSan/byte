#include "chunk.h"
#include "common.h"
#include "debug.h"

int main( int argc, const char* argv[] )
{
    Chunk chunk;
    for(int i = 0; i < 270; ++i)
    {
        int   constant = chunk.addConstant( i );
        chunk.writeConstant(i, i + 10);
    }

    chunk.writeChunk( OP_RETURN, 123 );

    disassembleChunk( &chunk, "test chunk" );

    return 0;
}