#include <cstdarg>
#include <stdarg.h>
#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "value.h"
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

void VM::runtimeError( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fputs( "\n", stderr );

    size_t instruction = ip - chunk->code - 1;
    int    line        = chunk->lines[ instruction ];
    fprintf( stderr, "[line %d] in script\n", line );
    resetStack();
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

Value VM::peek( int distance )
{
    return stackTop[ -1 - distance ];
}

InterpretResult VM::run()
{
#define READ_BYTE()     ( *ip++ )
#define READ_CONSTANT() ( chunk->getConstantsValues( READ_BYTE() ) )
// The use of a do while loop enables us to contain multiple statements inside a
// block but also enables us to add a semicolon at the end
#define BINARY_OP( valueType, op )                                             \
    do                                                                         \
    {                                                                          \
        if ( !IS_NUMBER( peek( 0 ) ) || !IS_NUMBER( peek( 1 ) ) )              \
        {                                                                      \
            runtimeError( "Operands must be numbers." );                       \
            return InterpretResult::INTERPRET_RUNTIME_ERROR;                   \
        }                                                                      \
        double b = AS_NUMBER( pop() );                                         \
        double a = AS_NUMBER( pop() );                                         \
        push( valueType( a op b ) );                                           \
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
        case OP_NIL:
            push( NIL_VAL );
            break;
        case OP_TRUE:
            push( BOOL_VAL( true ) );
            break;
        case OP_FALSE:
            push( BOOL_VAL( false ) );
            break;
        case OP_ADD:
        {
            BINARY_OP( NUMBER_VAL, +);
            break;
        }
        case OP_SUBTRACT:
        {
            BINARY_OP( NUMBER_VAL, -);
            break;
        }
        case OP_MULTIPLY:
        {
            BINARY_OP( NUMBER_VAL, * );
            break;
        }
        case OP_DIVIDE:
        {
            BINARY_OP( NUMBER_VAL, / );
            break;
        }
        case OP_NEGATE:
        {
            if ( !IS_NUMBER( peek( 0 ) ) )
            {
                runtimeError( "Operand must be a number." );
                return InterpretResult::INTERPRET_RUNTIME_ERROR;
            }
            push( NUMBER_VAL( -AS_NUMBER( pop() ) ) );
            break;
            break;
        }
        case OP_RETURN:
        {
            // Pop the last in stack... hopefully
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