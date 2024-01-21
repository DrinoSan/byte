#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"

static void repl( VM& vm )
{
    std::string line;
    for ( ;; )
    {
        std::cout << "> ";
        if ( !std::getline( std::cin, line ) )
        {
            std::cout << "\n";
            break;
        }

        vm.interpret( line.c_str() );
    }
}

static std::string readFile(const char* path) {
    std::ifstream file(path, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Could not open file \"" << path << "\"." << std::endl;
        exit(74);
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);

    if (!file) {
        std::cerr << "Could not read file \"" << path << "\"." << std::endl;
        exit(74);
    }

    return std::string(buffer.data(), fileSize);
}

static void runFile( const char* path, VM& vm )
{
    std::string     source = readFile( path );
    InterpretResult result = vm.interpret( source.c_str() );

    if ( result == InterpretResult::INTERPRET_COMPILE_ERROR )
        exit( 65 );
    if ( result == InterpretResult::INTERPRET_RUNTIME_ERROR )
        exit( 70 );
}

int main( int argc, const char* argv[] )
{
    VM vm = VM::getInstance();

    if ( argc == 1 )
    {
        repl( vm );
    }
    else if ( argc == 2 )
    {
        runFile( argv[ 1 ], vm );
    }
    else
    {
        fprintf( stderr, "Usage: clox [path]\n" );
        exit( 64 );
    }

    return 0;
}