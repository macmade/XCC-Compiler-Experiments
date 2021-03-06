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
 * @header      Parser.h
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 * @dicussion   Adapted from "Compiler Design in C" by Allen I. Holub.
 *              ISBN 0-13-155045-4 - https://holub.com/compiler
 */

#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdbool.h>
#include "Lexer.h"

typedef struct
{
    Token token;
    char  name[ 128 ];
} Qualifier;

void Parser_Statements( void );
bool Parser_Declaration( void );
bool Parser_Qualifiers( Qualifier * qualifiers, size_t * size );
bool Parser_Pointers( void );
bool Parser_Type( Qualifier * qualifiers, size_t size );
bool Parser_IDOrPointerID( void );
bool Parser_ID( void );
bool Parser_PointerID( void );
bool Parser_Array( int * size );
bool Parser_ValidQualifiers( const char * type, Qualifier * qualifiers, size_t size );

#endif /* PARSER_H */
