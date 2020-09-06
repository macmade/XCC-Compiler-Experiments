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
 * declaration -> qualifiers type identifier
 */
bool Parser_Declaration( void )
{
    Qualifier qualifiers[ 10 ];
    size_t    size;
    
    memset( qualifiers, 0, sizeof( qualifiers ) );
    
    size = sizeof( qualifiers ) / sizeof( *( qualifiers ) );
    
    return Parser_Qualifiers( qualifiers, &size )
        && Parser_Type( qualifiers, size )
        && Parser_ID();
}

/*
 * qualifiers -> qualifier | qualifier qualifiers | EPSILON
 */
bool Parser_Qualifiers( Qualifier * qualifiers, size_t * size )
{
    size_t i;
    
    i = 0;
    
    while
    (
           Lexer_Match( TokenConst )
        || Lexer_Match( TokenVolatile )
        || Lexer_Match( TokenSign )
        || Lexer_Match( TokenSize )
    )
    {
        if( i >= *( size ) )
        {
            Error( "Buffer too small" );
            abort();
        }
        
        if( Parser_Qualifier( &( qualifiers[ i ] ) ) == false )
        {
            return false;
        }
        
        i++;
    }
    
    *( size ) = i;
    
    return true;
}

/*
 * qualifier -> CONST | VOLATILE | SIGN | SIZE | EPSILON
 */
bool Parser_Qualifier( Qualifier * qualifier )
{
    if( Lexer_LegalLookahead( TokenConst, TokenVolatile, TokenSign, TokenSize, TokenEnd ) == false )
    {
        return false;
    }
    
    if( sizeof( qualifier->name ) < Lexer_GetLength() + 1 )
    {
        Error( "Buffer too small" );
        abort();
    }
    
    qualifier->token = Lexer_GetCurrent();
    
    strncpy( qualifier->name, Lexer_GetText(), Lexer_GetLength() );
    Lexer_Advance();
    
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
        Debug( "No type specified - Defaults to int" );
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
    if( Lexer_LegalLookahead( TokenID, TokenEnd ) == false )
    {
        Error( "Expected identifier" );
        
        return false;
    }
    
    Lexer_Advance();
    
    return true;
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
                if
                (
                       q1->token != TokenSize
                    || strcmp( q1->name, "long" ) != 0
                    || strcmp( q2->name, "long" ) != 0
                )
                {
                    Error( "Duplicate qualifier type: %s / %s", q1->name, q2->name );
                    
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
    else if( isIntegral && strlen( type ) == 0 && signQualifier == NULL && sizeQualifier == NULL )
    {
        Error( "Invalid implicit int without sign or size qualification" );
            
        return false;
    }
    
    return true;
}
