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
#include "Name.h"
#include <stdio.h>

/*
 * statements -> expression SEMICOLON | expression SEMI statements
 */
void Parser_Statements( void )
{
    while( Lexer_Match( TokenEnd ) == false )
    {
        const char * tmp;

        Parser_Expression( tmp = Name_NewName() );
        Name_FreeName( tmp );

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
 * expression  -> term expression'
 * expression' -> ADD term expression' | epsilon
 */
void Parser_Expression( const char * tmp )
{
    if( Lexer_LegalLookahead( TokenNumericOrID, TokenLeftParenthesis, TokenEnd ) == false )
    {
        return;
    }

    Parser_Term( tmp );

    while( Lexer_Match( TokenAdd ) )
    {
        const char * tmp2;

        Lexer_Advance();
        Parser_Term( tmp2 = Name_NewName() );
        Debug( "%s += %s", tmp, tmp2 );
        Name_FreeName( tmp2 );
    }
}

/*
 * term  -> factor term'
 * term' -> MULTIPLY factor term' | epsilon
 */
void Parser_Term( const char * tmp )
{
    if( Lexer_LegalLookahead( TokenNumericOrID, TokenLeftParenthesis, TokenEnd ) == false )
    {
        return;
    }

    Parser_Factor( tmp );

    while( Lexer_Match( TokenMultiply ) )
    {
        const char * tmp2;

        Lexer_Advance();
        Parser_Factor( tmp2 = Name_NewName() );
        Debug( "%s *= %s", tmp, tmp2 );
        Name_FreeName( tmp2 );
    }
}

/* factor -> NUMERIC_OR_ID | LEFT_PARENTHESIS expression RIGHT_PARENTHESIS */
void Parser_Factor( const char * tmp )
{
    if( Lexer_LegalLookahead( TokenNumericOrID, TokenLeftParenthesis, TokenEnd ) == false )
    {
        return;
    }

    if( Lexer_Match( TokenNumericOrID ) )
    {
        Debug( "%s = %1.*s", tmp, Lexer_GetLength(), Lexer_GetText() );
        Lexer_Advance();
    }
    else if( Lexer_Match( TokenLeftParenthesis ) )
    {
        Lexer_Advance();
        Parser_Expression( tmp );

        if( Lexer_Match( TokenRightParenthesis ) )
        {
            Lexer_Advance();
        }
        else
        {
            Error( "Mismatch parenthesis" );
        }
    }
    else
    {
        Error( "Number or identifier expected" );
    }
}
