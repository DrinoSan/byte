#include <stdio.h>

#include "common.h"
#include "vm.h"

VM::VM() {}

VM::~VM()
{
    freeVM();
}

void VM::initVM() {}

void VM::freeVM() {}

InterpretResult VM::interpret( Chunk* chunk_ )
{
    chunk = chunk_;
    ip    = chunk->code;
    return run();
}

InterpretResult VM::run()
{
#define READ_BYTE()     ( *ip++ )
#define READ_CONSTANT() ( chunk->getConstantsValues( READ_BYTE() ) )

    for ( ;; )
    {
        uint8_t instruction;
        switch ( instruction = READ_BYTE() )
        {
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            chunk->printValue( constant );
            printf( "\n" );
            break;
        }
        case OP_RETURN:
        {
            return InterpretResult::INTERPRET_OK;
        }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
}