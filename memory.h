#pragma once

#include <algorithm>

#include "common.h"

template <typename T>
inline T GROW_CAPACITY( T capacity )
{
    return capacity < 8 ? 8 : capacity * 2;
}

void* reallocate( void* pointer, size_t oldSize, size_t newSize );

template <typename T>
inline T* GROW_ARRAY( T* pointer, size_t oldCount, size_t newCount )
{
    return reinterpret_cast<T*>(
        reallocate( pointer, sizeof( T ) * oldCount, sizeof( T ) * newCount ) );
}

template <typename T>
inline void FREE_ARRAY( T* pointer, size_t oldCount )
{
    reallocate( pointer, sizeof( T ) * oldCount, 0 );
}