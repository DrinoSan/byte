#pragma once

#include "chunk.h"
#include "scanner.h"

class Parser
{
  public:
    Parser()  = default;
    ~Parser() = default;

  public:
    void errorAtCurrent( const char* message );

    void error( const char* message );
    void errorAt( Token* token, const char* message );

  public:
    Token current;
    Token previous;
    bool  hadError;
    bool  panicMode;
};

enum class Precedence
{
    PREC_NONE,
    PREC_ASSIGNMENT,   // =
    PREC_OR,           // or
    PREC_AND,          // and
    PREC_EQUALITY,     // == !=
    PREC_COMPARISON,   // < > <= >=
    PREC_TERM,         // + -
    PREC_FACTOR,       // * /
    PREC_UNARY,        // ! -
    PREC_CALL,         // . ()
    PREC_PRIMARY
};

using ParseFn = std::function<void()>;
typedef struct
{
    ParseFn    prefix;
    ParseFn    infix;
    Precedence precedence;
} ParseRule;

class Compiler
{
  public:
    Compiler() { printf( "Constructed Compiler\n" ); }

    ~Compiler() { printf( "DESTROYED Compiler\n" ); };

    bool compile( const char* source, Chunk* chunk );
    void advance();
    void consume( TokenType type, const char* message );
    void emitByte( uint8_t byte );
    void emitBytes( uint8_t byte1, uint8_t byte2 );

    Chunk* currentChunk();

    void       endCompiler();
    void       expression();
    void       parsePrecedence( Precedence precedence );
    ParseRule* getRule( TokenType type )
    {
        return &rules[ static_cast<size_t>( type ) ];
    }
    void binary();
    void grouping();
    void number();
    void unary();

    void initializeRules()
    {
        rules.resize( static_cast<size_t>( TokenType::TOKEN_EOF ) + 1 );

        /* clang-format off */
        // lambda > std::bind
        rules[static_cast<size_t>(TokenType::TOKEN_LEFT_PAREN)]    = { [this]{grouping();}, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_RIGHT_PAREN)]   = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_LEFT_BRACE)]    = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_RIGHT_BRACE)]   = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_COMMA)]         = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_DOT)]           = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_MINUS)]         = { [this]{unary();}, [this]{binary();}, Precedence::PREC_TERM };
        rules[static_cast<size_t>(TokenType::TOKEN_PLUS)]          = { nullptr, [this] {binary();}, Precedence::PREC_TERM };
        rules[static_cast<size_t>(TokenType::TOKEN_SEMICOLON)]     = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_SLASH)]         = { nullptr,[this]{binary();}, Precedence::PREC_FACTOR };
        rules[static_cast<size_t>(TokenType::TOKEN_STAR)]          = { nullptr,[this]{binary();}, Precedence::PREC_FACTOR };
        rules[static_cast<size_t>(TokenType::TOKEN_BANG)]          = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_BANG_EQUAL)]    = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_EQUAL)]         = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_EQUAL_EQUAL)]   = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_GREATER)]       = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_GREATER_EQUAL)] = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_LESS)]          = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_LESS_EQUAL)]    = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_IDENTIFIER)]    = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_STRING)]        = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_NUMBER)]        = { [this]{number();}, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_AND)]           = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_CLASS)]         = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_ELSE)]          = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_FALSE)]         = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_FOR)]           = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_FUN)]           = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_IF)]            = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_NIL)]           = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_OR)]            = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_PRINT)]         = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_RETURN)]        = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_SUPER)]         = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_THIS)]          = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_TRUE)]          = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_VAR)]           = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_WHILE)]         = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_ERROR)]         = { nullptr, nullptr, Precedence::PREC_NONE };
        rules[static_cast<size_t>(TokenType::TOKEN_EOF)]           = { nullptr, nullptr, Precedence::PREC_NONE };
        /* clang-format on */
    }

    void    emitReturn();
    int32_t makeConstant( Value value );
    void    emitConstant( Value value );

  private:
    Scanner                scanner;
    Parser                 parser;
    Chunk*                 compilingChunk;
    std::vector<ParseRule> rules;
};