#include <stdio.h>

#include "memory.h"
#include "value.h"

ValueArray::ValueArray()
{
    initValueArray();
}

ValueArray::~ValueArray()
{
    freeValueArray();
}

void ValueArray::writeValueArray( Value value )
{
    if ( capacity < count + 1 )
    {
        int oldCapacity = capacity;
        capacity        = GROW_CAPACITY( oldCapacity );
        values          = GROW_ARRAY<Value>( values, oldCapacity, capacity );
    }

    values[ count ] = value;
    count++;
}

void ValueArray::freeValueArray()
{
    FREE_ARRAY<Value>( values, capacity );
    initValueArray();
}

void ValueArray::printValue( Value value )
{
    switch ( value.type )
    {
    case VAL_BOOL:
        printf( AS_BOOL( value ) ? "true" : "false" );
        break;
    case VAL_NIL:
        printf( "nil" );
        break;
    case VAL_NUMBER:
        printf( "%g", AS_NUMBER( value ) );
        break;
    }
}

void ValueArray::initValueArray()
{
    values   = nullptr;
    capacity = 0;
    count    = 0;
}