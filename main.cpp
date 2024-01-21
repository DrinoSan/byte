#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"

int main( int argc, const char* argv[] )
{
    VM vm = VM::getInstance();

    Chunk chunk;
    chunk.writeConstant( 1.2, 123 );

    chunk.writeConstant(3.4, 123);

    chunk.writeChunk(OP_ADD, 123);

    chunk.writeConstant(4.6, 123);

    chunk.writeChunk(OP_DIVIDE, 123);
    chunk.writeChunk(OP_NEGATE, 123);

    chunk.writeChunk( OP_RETURN, 123 );

    disassembleChunk( &chunk, "test chunk" );
    vm.interpret(&chunk); 


    return 0;
}