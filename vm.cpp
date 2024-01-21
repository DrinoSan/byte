#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

VM::VM()
{
    initVM();
}

VM::~VM()
{
    freeVM();
}

void VM::initVM()
{
    resetStack();
}

void VM::freeVM() {}

InterpretResult VM::interpret( const char* source )
{
    Chunk chunk_;

    if ( !compiler.compile( source, &chunk_ ) )
    {
        return InterpretResult::INTERPRET_COMPILE_ERROR;
    }

    chunk = &chunk_;
    ip    = chunk->code;

    InterpretResult result = run();

    return result;
}

void VM::resetStack()
{
    stackTop = stack;
}

void VM::push( Value value )
{
    *stackTop = value;
    stackTop++;
}

Value VM::pop()
{
    return *( --stackTop );
}

InterpretResult VM::run()
{
#define READ_BYTE()     ( *ip++ )
#define READ_CONSTANT() ( chunk->getConstantsValues( READ_BYTE() ) )
// The use of a do while loop enables us to contain multiple statements inside a
// block but also enables us to add a semicolon at the end
#define BINARY_OP( op )                                                        \
    do                                                                         \
    {                                                                          \
        double b = pop();                                                      \
        double a = pop();                                                      \
        push( a op b );                                                        \
    } while ( false )

    for ( ;; )
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf( "          " );
        for ( Value* slot = stack; slot < stackTop; slot++ )
        {
            printf( "[ " );
            chunk->printValue( *slot );
            printf( " ]" );
        }

        printf( "\n" );
        disassembleInstruction( chunk, ( int ) ( ip - chunk->code ) );
#endif
        uint8_t instruction;
        switch ( instruction = READ_BYTE() )
        {
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            push( constant );
            break;
        }
        case OP_ADD:
        {
            BINARY_OP( +);
            break;
        }
        case OP_SUBTRACT:
        {
            BINARY_OP( -);
            break;
        }
        case OP_MULTIPLY:
        {
            BINARY_OP( * );
            break;
        }
        case OP_DIVIDE:
        {
            BINARY_OP( / );
            break;
        }
        case OP_NEGATE:
        {
            push( -pop() );
            break;
        }
        case OP_RETURN:
        {
            chunk->printValue( pop() );
            printf( "\n" );
            return InterpretResult::INTERPRET_OK;
        }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}