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
#include "Lexer.h"
#include "Print.h"
#include <stdio.h>

/*
 * statements -> declaration SEMICOLON | declaration SEMI statements
 */
void Parser_Statements( void )
{
    while( Lexer_Match( TokenEnd ) == false )
    {
        Parser_Declaration();

        if( Lexer_Match( TokenSemicolon ) )
        {
            Lexer_Advance();
        }
        else
        {
            Warning( "Inserting missing semicolon" );
        }
    }
}

/*
 * declaration -> type identifier
 */
void Parser_Declaration( void )
{
    if( Lexer_LegalLookahead( TokenType, TokenID, TokenEnd ) == false )
    {
        return;
    }

    Parser_Types();
    Parser_Identifier();
}

/*
 * types -> type | type types
 */
void Parser_Types( void )
{
    if( Lexer_LegalLookahead( TokenType, TokenEnd ) == false )
    {
        return;
    }

    Parser_Type();

    while( Lexer_Match( TokenType ) )
    {
        Parser_Type();
    }
}

/*
 * type -> TYPE
 */
void Parser_Type( void )
{
    if( Lexer_LegalLookahead( TokenType, TokenEnd ) == false )
    {
        return;
    }

    Debug( "Type: %1.*s", Lexer_GetLength(), Lexer_GetText() );
    Lexer_Advance();
}

/*
 * identifier -> ID
 */
void Parser_Identifier( void )
{
    if( Lexer_LegalLookahead( TokenID, TokenEnd ) == false )
    {
        return;
    }

    Debug( "ID: %1.*s", Lexer_GetLength(), Lexer_GetText() );
    Lexer_Advance();
}
