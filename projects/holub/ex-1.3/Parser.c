/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Jean-David Gadina - www.xs-labs.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/*!
 * @file        Parser.c
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 * @dicussion   Adapted from "Compiler Design in C" by Allen I. Holub.
 *              ISBN 0-13-155045-4 - https://holub.com/compiler
 */

#include "Parser.h"
#include "Print.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * statements -> declaration SEMICOLON | declaration SEMI statements
 */
void Parser_Statements( void )
{
    while( Lexer_Match( TokenEnd ) == false )
    {
        if( Parser_Declaration() == false )
        {
            Debug( "Invalid Syntax" );
            Lexer_Discard();
            Lexer_Advance();
        }
        else
        {
            if( Lexer_Match( TokenSemicolon ) )
            {
                Debug( "Syntax OK" );
                Lexer_Advance();
            }
            else
            {
                Warning( "Inserting missing semicolon" );
                Debug( "Syntax OK" );
            }
        }
    }
}

/*
 * declaration -> qualifiers type pointer identifier array
 */
bool Parser_Declaration( void )
{
    Qualifier qualifiers[ 10 ];
    size_t    size;

    memset( qualifiers, 0, sizeof( qualifiers ) );

    size = sizeof( qualifiers ) / sizeof( *( qualifiers ) );

    return Parser_Qualifiers( qualifiers, &size )
        && Parser_Type( qualifiers, size )
        && Parser_Pointers()
        && Parser_ID()
        && Parser_Array( NULL );
}

/*
 * qualifiers -> qualifier | qualifier qualifiers | EPSILON
 * qualifier  -> CONST | VOLATILE | SIGN | SIZE | EPSILON
 */
bool Parser_Qualifiers( Qualifier * qualifiers, size_t * size )
{
    size_t i;

    i = 0;

    while( Lexer_Match( TokenConst )
           || Lexer_Match( TokenVolatile )
           || Lexer_Match( TokenSign )
           || Lexer_Match( TokenSize ) )
    {
        Qualifier * qualifier;

        if( i >= *( size ) )
        {
            Error( "Buffer too small" );
            abort();
        }

        qualifier = &( qualifiers[ i ] );

        if( sizeof( qualifier->name ) < Lexer_GetLength() + 1 )
        {
            Error( "Buffer too small" );
            abort();
        }

        qualifier->token = Lexer_GetCurrent();

        strncpy( qualifier->name, Lexer_GetText(), Lexer_GetLength() );
        Lexer_Advance();

        i++;
    }

    *( size ) = i;

    return true;
}

/*
 * pointers -> pointer | pointer pointers | EPSILON
 * pointer  -> PTR | PTR qualifier | EPSILON
 */
bool Parser_Pointers( void )
{
    while( Lexer_Match( TokenPointer ) )
    {
        Lexer_Advance();

        {
            Qualifier qualifiers[ 10 ];
            size_t    size;

            memset( qualifiers, 0, sizeof( qualifiers ) );

            size = sizeof( qualifiers ) / sizeof( *( qualifiers ) );

            if( Parser_Qualifiers( qualifiers, &size ) == false )
            {
                return false;
            }

            for( size_t i = 0; i < size; i++ )
            {
                if( qualifiers[ i ].token != TokenConst && qualifiers[ i ].token != TokenVolatile )
                {
                    Error( "Unexpected pointer qualifier: %s", qualifiers[ i ].name );
                }

                for( size_t j = 0; j < size; j++ )
                {
                    if( i == j )
                    {
                        continue;
                    }

                    if( qualifiers[ i ].token == qualifiers[ j ].token )
                    {
                        Error( "Duplicate pointer qualifier: %s", qualifiers[ i ].name );

                        return false;
                    }
                }
            }
        }
    }

    return true;
}

/*
 * type -> TYPE | EPSILON
 */
bool Parser_Type( Qualifier * qualifiers, size_t size )
{
    char type[ 128 ];

    if( sizeof( type ) < Lexer_GetLength() + 1 )
    {
        Error( "Buffer too small" );
        abort();
    }

    memset( type, 0, sizeof( type ) );

    if( Lexer_Match( TokenType ) )
    {
        strncpy( type, Lexer_GetText(), Lexer_GetLength() );
        Lexer_Advance();
    }
    else
    {
        bool hasSign;
        bool hasSize;

        hasSign = false;
        hasSize = false;

        for( size_t i = 0; i < size; i++ )
        {
            if( qualifiers[ i ].token == TokenSign )
            {
                hasSign = true;
            }

            if( qualifiers[ i ].token == TokenSize )
            {
                hasSize = true;
            }
        }

        if( hasSign == false && hasSize == false )
        {
            Error( "Expected a type" );

            return false;
        }
    }

    if( Parser_ValidQualifiers( type, qualifiers, size ) )
    {
        return true;
    }

    Error( "Invalid qualifiers for type: %s", type );

    return false;
}

/*
 * identifier -> ID
 */
bool Parser_ID( void )
{
    bool error;

    if( Lexer_LegalLookahead( &error, TokenID, TokenEnd ) == false || error )
    {
        Error( "Expected identifier" );

        return false;
    }

    {
        const char * id;

        id = Lexer_GetText();

        if( isdigit( id[ 0 ] ) )
        {
            Error( "Bad identifier" );

            return false;
        }
    }

    Lexer_Advance();

    return true;
}

/*
 * array         -> array_fixed | array_dynamic | epsilon
 * array_fixed   -> LEFT_BRACKET RIGHT_BRACKET
 * array_dynamic -> LEFT_BRACKET NUMERIC RIGHT_BRACKET
 */
bool Parser_Array( int * size )
{
    if( Lexer_Match( TokenLeftBracket ) == false )
    {
        return true;
    }

    Lexer_Advance();

    if( Lexer_Match( TokenRightBracket ) )
    {
        if( size != NULL )
        {
            *( size ) = -1;
        }

        Lexer_Advance();

        return true;
    }
    else if( Lexer_Match( TokenNumeric ) )
    {
        char s[ 128 ];
        int  i;

        memset( s, 0, sizeof( s ) );

        if( Lexer_GetLength() + 1 > sizeof( s ) )
        {
            Error( "Buffer too small" );
            abort();
        }

        strncpy( s, Lexer_GetText(), sizeof( s ) );

        i = atoi( s );

        if( i == 0 )
        {
            Error( "Array size cannot be zero" );

            return false;
        }

        if( size != NULL )
        {
            *( size ) = i;
        }

        Lexer_Advance();

        if( Lexer_Match( TokenRightBracket ) == false )
        {
            Error( "Expected ]" );

            return false;
        }

        Lexer_Advance();

        return true;
    }
    else
    {
        Error( "Expected array size" );

        return false;
    }
}

bool Parser_ValidQualifiers( const char * type, Qualifier * qualifiers, size_t size )
{
    bool   isIntegral;
    bool   isFloat;
    bool   isChar;
    char * signQualifier;
    char * sizeQualifier;
    size_t longCount;

    isIntegral    = false;
    isFloat       = false;
    isChar        = false;
    signQualifier = NULL;
    sizeQualifier = NULL;
    longCount     = 0;

    if( strcmp( type, "float" ) == 0 || strcmp( type, "double" ) == 0 )
    {
        isFloat = true;
    }
    else if( strcmp( type, "char" ) == 0 )
    {
        isChar = true;
    }
    else
    {
        isIntegral = true;
    }

    for( size_t i = 0; i < size; i++ )
    {
        Qualifier * q1;

        q1 = &( qualifiers[ i ] );

        if( q1->token == TokenSize && strcmp( q1->name, "long" ) == 0 )
        {
            longCount++;
        }

        if( q1->token == TokenSign )
        {
            signQualifier = q1->name;
        }

        if( q1->token == TokenSize )
        {
            sizeQualifier = q1->name;
        }

        for( size_t j = 0; j < size; j++ )
        {
            Qualifier * q2;

            q2 = &( qualifiers[ j ] );

            if( i == j )
            {
                continue;
            }

            if( q1->token == q2->token )
            {
                if( q1->token != TokenSize
                    || strcmp( q1->name, "long" ) != 0
                    || strcmp( q2->name, "long" ) != 0 )
                {
                    Error( "Duplicate qualifier: %s", q1->name );

                    return false;
                }
            }
        }
    }

    if( signQualifier != NULL )
    {
        if( isFloat )
        {
            Error( "Invalid sign qualifier for floating point type: %s %s", signQualifier, type );

            return false;
        }
    }
    else if( sizeQualifier != NULL )
    {
        if( isChar )
        {
            Error( "Invalid size qualifier for char type: %s %s", sizeQualifier, type );

            return false;
        }
        else if( isFloat && strcmp( type, "double" ) == 0 && strcmp( sizeQualifier, "long" ) == 0 )
        {
            if( longCount > 1 )
            {
                Error( "Invalid size qualifier for floating point type: %s(%zu) %s", sizeQualifier, longCount, type );

                return false;
            }
        }
        else if( isFloat )
        {
            Error( "Invalid size qualifier for floating point type: %s %s", sizeQualifier, type );

            return false;
        }
        else if( isIntegral && strcmp( sizeQualifier, "long" ) == 0 && longCount > 2 )
        {
            Error( "Invalid size qualifier for integral type: %s(%zu) %s", sizeQualifier, longCount, type );

            return false;
        }
    }

    return true;
}
