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
 * @file        Lexer.c
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 * @dicussion   Adapted from "Compiler Design in C" by Allen I. Holub.
 *              ISBN 0-13-155045-4 - https://holub.com/compiler
 */

#include "Lexer.h"
#include "Print.h"
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

static char * Lexer_Text      = "";
static size_t Lexer_Length    = 0;
static size_t Lexer_Line      = 0;
static int    Lexer_Lookahead = -1;

const char * Lexer_GetText( void )
{
    return Lexer_Text;
}

size_t Lexer_GetLength( void )
{
    return Lexer_Length;
}

size_t Lexer_GetLine( void )
{
    return Lexer_Line;
}

Token Lexer_Next( void )
{
    static char   buf[ 128 ];
           char * current;
    
    current = Lexer_Text + Lexer_Length;
    
    while( true )
    {
        while( *( current ) == 0 )
        {
            current = buf;
            
            if( fgets( buf, sizeof( buf ), stdin ) == NULL )
            {
                *( current ) = 0;
                
                return TokenEnd;
            }
            
            Lexer_Line++;
            
            while( isspace( *( current ) ) )
            {
                current++;
            }
        }
        
        for( ; *( current ) != 0; current++ )
        {
            Lexer_Text   = current;
            Lexer_Length = 1;
            
            switch( *( current ) )
            {
                case EOF: Debug( "Token: End" ); return TokenEnd;
                case ';': Debug( "Token: ;" );   return TokenSemicolon;
                case '+': Debug( "Token: +" );   return TokenAdd;
                case '*': Debug( "Token: *" );   return TokenMultiply;
                case '(': Debug( "Token: (" );   return TokenLeftParenthesis;
                case ')': Debug( "Token: )" );   return TokenRightParenthesis;
                
                case '\n':
                case '\t':
                case ' ':
                    
                    break;
                    
                default:
                    
                    if( isalnum( *( current ) ) == false )
                    {
                        Error( "Ignoring illegal input: %c", *( current ) );
                        
                        break;
                    }
                    
                    while( isalnum( *( current ) ) )
                    {
                        current++;
                    }
                    
                    Lexer_Length = ( uintptr_t )current - ( uintptr_t )Lexer_Text;
                    
                    Debug( "Token: %1.*s", Lexer_Length, Lexer_Text );
                    
                    return TokenNumericOrID;
            }
        }
    }
}

void Lexer_Advance( void )
{
    Lexer_Lookahead = ( int )( Lexer_Next() );
}

bool Lexer_Match( Token token )
{
    if( Lexer_Lookahead == -1 )
    {
        Lexer_Advance();
    }
    
    return ( int )token == Lexer_Lookahead;
}

bool Lexer_LegalLookahead( Token first, ... )
{
    va_list ap;
    bool    ret;
    
    va_start( ap, first );
    
    ret = false;
    
    if( first == TokenEnd )
    {
        if( Lexer_Match( TokenEnd ) )
        {
            ret = true;
        }
    }
    else
    {
        Token   token;
        Token   lookaheads[ 16 ];
        Token * p;
        Token * current;
        bool    err;
        
        err      = false;
        p        = lookaheads;
        *( p++ ) = first;
        
        while( ( token = va_arg( ap, Token ) ) && p < &( lookaheads[ 16 ] ) )
        {
            *( p++ ) = token;
        }
        
        while( Lexer_Match( TokenSemicolon ) == false )
        {
            for( current = lookaheads; current < p; current++ )
            {
                if( Lexer_Match( *( current ) ) )
                {
                    ret = true;
                    
                    goto done;
                }
            }
            
            if( err == false )
            {
                err = true;
                
                Error( "Syntax error" );
            }
            
            Lexer_Advance();
        }
    }
    
    done:
        
        va_end( ap );
        
        return ret;
}
