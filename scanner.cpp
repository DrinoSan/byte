#include <cstring>
#include <stdio.h>
#include <string>

#include "common.h"
#include "scanner.h"

void Scanner::initScanner( const char* source )
{
    start   = source;
    current = source;
    line    = 1;
}

Token Scanner::scanToken()
{
    skipWhitespace();
    start = current;
    if ( isAtEnd() )
        return makeToken( TokenType::TOKEN_EOF );

    char c = advance();
    if ( isAlpha( c ) )
        return identifier();
    if ( isDigit( c ) )
        return number();

    switch ( c )
    {
    case '(':
        return makeToken( TokenType::TOKEN_LEFT_PAREN );
    case ')':
        return makeToken( TokenType::TOKEN_RIGHT_PAREN );
    case '{':
        return makeToken( TokenType::TOKEN_LEFT_BRACE );
    case '}':
        return makeToken( TokenType::TOKEN_RIGHT_BRACE );
    case ';':
        return makeToken( TokenType::TOKEN_SEMICOLON );
    case ',':
        return makeToken( TokenType::TOKEN_COMMA );
    case '.':
        return makeToken( TokenType::TOKEN_DOT );
    case '-':
        return makeToken( TokenType::TOKEN_MINUS );
    case '+':
        return makeToken( TokenType::TOKEN_PLUS );
    case '/':
        return makeToken( TokenType::TOKEN_SLASH );
    case '*':
        return makeToken( TokenType::TOKEN_STAR );
    case '!':
        return makeToken( match( '=' ) ? TokenType::TOKEN_BANG_EQUAL
                                       : TokenType::TOKEN_BANG );
    case '=':
        return makeToken( match( '=' ) ? TokenType::TOKEN_EQUAL_EQUAL
                                       : TokenType::TOKEN_EQUAL );
    case '<':
        return makeToken( match( '=' ) ? TokenType::TOKEN_LESS_EQUAL
                                       : TokenType::TOKEN_LESS );
    case '>':
        return makeToken( match( '=' ) ? TokenType::TOKEN_GREATER_EQUAL
                                       : TokenType::TOKEN_GREATER );
    case '"':
        return string();
    }

    return errorToken( "Unexpected character." );
}

bool Scanner::isAtEnd()
{
    return *current == '\0';
}

Token Scanner::makeToken( TokenType type )
{
    Token token;
    token.type   = type;
    token.start  = start;
    token.length = static_cast<int>( current - start );
    token.line   = line;

    return token;
}

Token Scanner::errorToken( const char* message )
{
    Token token;
    token.type   = TokenType::TOKEN_ERROR;
    token.start  = message;
    token.length = static_cast<int>( strlen( message ) );
    token.line   = line;

    return token;
}

char Scanner::advance()
{
    current++;
    return current[ -1 ];
}

bool Scanner::match( char expected )
{
    if ( isAtEnd() )
        return false;

    if ( *current != expected )
        return false;

    current++;
    return true;
}

void Scanner::skipWhitespace()
{
    for ( ;; )
    {
        char c = peek();
        switch ( c )
        {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
        {
            line++;
            advance();
            break;
        }
        case '/':
        {
            if ( peekNext() == '/' )
            {
                // Comment goes until the end of the line
                while ( peek() != '\n' && !isAtEnd() )
                    advance();
            }
            else
            {
                return;
            }
            break;
        }
        default:
            return;
        }
    }
}

char Scanner::peek()
{
    return *current;
}

char Scanner::peekNext()
{
    if ( isAtEnd() )
        return '\0';
    return current[ 1 ];
}

Token Scanner::string()
{
    while ( peek() != '"' && !isAtEnd() )
    {
        if ( peek() == '\n' )
            line++;
        advance();
    }

    if ( isAtEnd() )
        return errorToken( "Unterminated string." );

    // Closing quote
    advance();
    return makeToken( TokenType::TOKEN_STRING );
}

bool Scanner::isDigit( char c )
{
    return c >= '0' && c <= '9';
}

Token Scanner::number()
{
    while ( isDigit( peek() ) )
        advance();

    if ( peek() == '.' && isDigit( peekNext() ) )
    {
        advance();
    }

    while ( isDigit( peek() ) )
        advance();

    return makeToken( TokenType::TOKEN_NUMBER );
}

bool Scanner::isAlpha( char c )
{
    return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || c == '_';
}

Token Scanner::identifier()
{
    while ( isAlpha( peek() ) || isDigit( peek() ) )
        advance();

    return makeToken( identifierType() );
}

TokenType Scanner::identifierType()
{
    switch ( start[ 0 ] )
    {
    case 'a':
        return checkKeyword( 1, 2, "nd", TokenType::TOKEN_AND );
    case 'c':
        return checkKeyword( 1, 4, "lass", TokenType::TOKEN_CLASS );
    case 'e':
        return checkKeyword( 1, 3, "lse", TokenType::TOKEN_ELSE );
    case 'f':
        if ( current - start > 1 )
        {
            switch ( start[ 1 ] )
            {
            case 'a':
                return checkKeyword( 2, 3, "lse", TokenType::TOKEN_FALSE );
            case 'o':
                return checkKeyword( 2, 1, "r", TokenType::TOKEN_FOR );
            case 'u':
                return checkKeyword( 2, 1, "n", TokenType::TOKEN_FUN );
            }
        }
        break;
    case 'i':
        return checkKeyword( 1, 1, "f", TokenType::TOKEN_IF );
    case 'n':
        return checkKeyword( 1, 2, "il", TokenType::TOKEN_NIL );
    case 'o':
        return checkKeyword( 1, 1, "r", TokenType::TOKEN_OR );
    case 'p':
        return checkKeyword( 1, 4, "rint", TokenType::TOKEN_PRINT );
    case 'r':
        return checkKeyword( 1, 5, "eturn", TokenType::TOKEN_RETURN );
    case 's':
    case 't':
        if ( current - start > 1 )
        {
            switch ( start[ 1 ] )
            {
            case 'h':
                return checkKeyword( 2, 2, "is", TokenType::TOKEN_THIS );
            case 'r':
                return checkKeyword( 2, 2, "ue", TokenType::TOKEN_TRUE );
            }
        }
        break;
        return checkKeyword( 1, 4, "uper", TokenType::TOKEN_SUPER );
    case 'v':
        return checkKeyword( 1, 2, "ar", TokenType::TOKEN_VAR );
    case 'w':
        return checkKeyword( 1, 4, "hile", TokenType::TOKEN_WHILE );
    }
    return TokenType::TOKEN_IDENTIFIER;
}

TokenType Scanner::checkKeyword( int start_, int length_, const char* rest,
                                 TokenType type )
{
    // current - start should be equal to start_ + length_ because we scanned
    // the whole token at this point check advance() function
    if ( current - start == start_ + length_ &&
         memcmp( start + start_, rest, length_ ) == 0 )
    {
        return type;
    }

    return TokenType::TOKEN_IDENTIFIER;
}