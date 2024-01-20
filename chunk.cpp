#include <cstdint>
#include <sys/_types/_int8_t.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

Chunk::Chunk()
{
    initChunk();
}

Chunk::~Chunk()
{
    freeChunk();
}

// Init empty chunk
void Chunk::initChunk()
{
    count    = 0;
    capacity = 0;
    code     = nullptr;
    lines    = nullptr;
    constants.initValueArray();
}

void Chunk::freeChunk()
{
    FREE_ARRAY<uint8_t>( code, capacity );
    FREE_ARRAY<int>( lines, capacity );

    constants.freeValueArray();
    initChunk();
}

uint8_t Chunk::getOpCodebyOffset( int offset )
{
    return code[ offset ];
}

int8_t Chunk::getConstantIndexByOpCodeOffset( int offset )
{
    return code[ offset + 1 ];
}

int Chunk::addConstant( Value value )
{
    constants.writeValueArray( value );
    return constants.count - 1;
}

Value Chunk::getConstantsValues( int constant )
{
    return constants.values[ constant ];
}

void Chunk::printValue( Value value )
{
    constants.printValue( value );
}

void Chunk::writeConstant( Value value, int line )
{
    int index = addConstant( value );
    if ( index < 256 )
    {
        writeChunk( OP_CONSTANT, line );
        writeChunk( index, line );

        return;
    }

    // Highest number is in the most right array element
    writeChunk( OP_CONSTANT_LONG, line );
    writeChunk( static_cast<int8_t>( index & 0xff ), line );
    writeChunk( static_cast<int8_t>( ( index >> 8 ) & 0xff ), line );
    writeChunk( static_cast<int8_t>( ( index >> 16 ) & 0xff ), line );
}

int32_t Chunk::getLongConstantIndexByOpCodeOffset( int offset )
{
    return code[ offset + 1 ] | ( code[ offset + 2 ] << 8 ) |
           ( code[ offset + 3 ] << 16 );
}

Value Chunk::getLoncConstantValues( uint32_t constant )
{
    return constants.values[ constant ];
}