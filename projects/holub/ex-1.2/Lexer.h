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
 * @header      Lexer.h
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 * @dicussion   Adapted from "Compiler Design in C" by Allen I. Holub.
 *              ISBN 0-13-155045-4 - https://holub.com/compiler
 */

#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    TokenEnd       = 0, /* End of input */
    TokenSemicolon = 1, /* ; */
    TokenConst     = 2, /* const */
    TokenVolatile  = 3, /* volatile */
    TokenSign      = 4, /* signed, unsigned */
    TokenSize      = 5, /* short, long, long long */
    TokenType      = 6, /* Type */
    TokenID        = 7  /* Identifier */
}
Token;

const char * Lexer_GetText( void );
size_t       Lexer_GetLength( void );
size_t       Lexer_GetLine( void );
Token        Lexer_GetCurrent( void );

Token Lexer_Next( void );
void  Lexer_Advance( void );
void  Lexer_Discard( void );
bool  Lexer_Match( Token token );
bool  Lexer_Compare( const char * value );
bool  Lexer_LegalLookahead( Token first, ... );

#endif /* LEXER_H */
