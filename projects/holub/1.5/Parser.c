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

/* statements -> expression SEMICOLON | expression SEMI statements */
void Parser_Statements( void )
{
    Parser_Expression();

    if( Lexer_Match( TokenEnd ) )
    {
        return;
    }

    if( Lexer_Match( TokenSemicolon ) )
    {
        Lexer_Advance();
    }
    else
    {
        Warning( "Inserting missing semicolon" );
    }

    if( Lexer_Match( TokenEnd ) == false )
    {
        Parser_Statements();
    }
}

/* expression -> term expression' */
void Parser_Expression( void )
{
    Parser_Term();
    Parser_ExpressionPrime();
}

/* expression' -> ADD term expression' | epsilon */
void Parser_ExpressionPrime( void )
{
    if( Lexer_Match( TokenAdd ) )
    {
        Lexer_Advance();
        Parser_Term();
        Parser_ExpressionPrime();
    }
}

/* term -> factor term' */
void Parser_Term( void )
{
    Parser_Factor();
    Parser_TermPrime();
}

/* term' -> MULTIPLY factor term' | epsilon */
void Parser_TermPrime( void )
{
    if( Lexer_Match( TokenMultiply ) )
    {
        Lexer_Advance();
        Parser_Factor();
        Parser_TermPrime();
    }
}

/* factor -> NUMERIC_OR_ID | LEFT_PARENTHESIS expression RIGHT_PARENTHESIS */
void Parser_Factor( void )
{
    if( Lexer_Match( TokenEnd ) )
    {
        return;
    }

    if( Lexer_Match( TokenNumericOrID ) )
    {
        Lexer_Advance();
    }
    else if( Lexer_Match( TokenLeftParenthesis ) )
    {
        Lexer_Advance();
        Parser_Expression();

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
