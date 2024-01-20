#pragma once

#include "common.h"

using Value = double;

class ValueArray
{
  public:
    ValueArray();
    ~ValueArray();

  public:
    void initValueArray();
    void writeValueArray( Value value );
    void freeValueArray();
    void printValue(Value value);

  public:
    int    capacity;
    int    count;
    Value* values;
};