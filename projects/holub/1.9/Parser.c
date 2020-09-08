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

        tmp = Parser_Expression();

        if( Lexer_Match( TokenSemicolon ) )
        {
            Lexer_Advance();
        }
        else
        {
            Warning( "Inserting missing semicolon" );
        }

        Name_FreeName( tmp );
    }
}

/*
 * expression  -> term expression'
 * expression' -> ADD term expression' | epsilon
 */
const char * Parser_Expression( void )
{
    const char * tmp1;

    if( Lexer_LegalLookahead( TokenNumericOrID, TokenLeftParenthesis, TokenEnd ) == false )
    {
        return NULL;
    }

    tmp1 = Parser_Term();

    while( Lexer_Match( TokenAdd ) )
    {
        const char * tmp2;

        Lexer_Advance();

        tmp2 = Parser_Term();

        Debug( "%s += %s", tmp1, tmp2 );

        Name_FreeName( tmp2 );
    }

    return tmp1;
}

/*
 * term  -> factor term'
 * term' -> MULTIPLY factor term' | epsilon
 */
const char * Parser_Term( void )
{
    const char * tmp1;

    if( Lexer_LegalLookahead( TokenNumericOrID, TokenLeftParenthesis, TokenEnd ) == false )
    {
        return NULL;
    }

    tmp1 = Parser_Factor();

    while( Lexer_Match( TokenMultiply ) )
    {
        const char * tmp2;

        Lexer_Advance();

        tmp2 = Parser_Factor();

        Debug( "%s *= %s", tmp1, tmp2 );

        Name_FreeName( tmp2 );
    }

    return tmp1;
}

/* factor -> NUMERIC_OR_ID | LEFT_PARENTHESIS expression RIGHT_PARENTHESIS */
const char * Parser_Factor( void )
{
    const char * tmp;

    tmp = NULL;

    if( Lexer_LegalLookahead( TokenNumericOrID, TokenLeftParenthesis, TokenEnd ) == false )
    {
        return NULL;
    }

    if( Lexer_Match( TokenNumericOrID ) )
    {
        Debug( "%s = %1.*s", tmp = Name_NewName(), Lexer_GetLength(), Lexer_GetText() );
        Lexer_Advance();
    }
    else if( Lexer_Match( TokenLeftParenthesis ) )
    {
        Lexer_Advance();

        tmp = Parser_Expression();

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

    return tmp;
}
