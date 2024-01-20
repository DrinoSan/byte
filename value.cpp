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
    FREE_ARRAY<Value>(values, capacity);
    initValueArray();
}

void ValueArray::printValue(Value value)
{
    printf("%g", value);
}

void ValueArray::initValueArray()
{
    values = nullptr;
    capacity = 0;
    count = 0;
}