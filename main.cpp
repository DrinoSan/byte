#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"

int main( int argc, const char* argv[] )
{
    VM vm = VM::getInstance();

    Chunk chunk;
    chunk.writeConstant( 1.2, 123 );

    // for ( int i = 0; i < 270; ++i )
    // {
    //     // int   constant = chunk.addConstant( i );
    //     chunk.writeConstant( i, i * 10 );
    // }

    chunk.writeChunk( OP_RETURN, 123 );

    disassembleChunk( &chunk, "test chunk" );
    vm.interpret(&chunk); 


    return 0;
}