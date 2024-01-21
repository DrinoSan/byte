#pragma once

#include "scanner.h"

class Compiler
{
    public:
    Compiler() = default;
    ~Compiler() = default;

    void compile(const char* source);

    private:
    Scanner scanner;
};