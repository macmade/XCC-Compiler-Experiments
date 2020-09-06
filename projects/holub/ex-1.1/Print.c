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
 * @file        Print.c
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 * @dicussion   Adapted from "Compiler Design in C" by Allen I. Holub.
 *              ISBN 0-13-155045-4 - https://holub.com/compiler
 */

#include "Print.h"
#include "Lexer.h"
#include <stdio.h>
#include <stdarg.h>

void Print( FILE * fh, const char * level, const char * fmt, va_list ap );
void Print( FILE * fh, const char * level, const char * fmt, va_list ap )
{
    fprintf( fh, "*** [ %s ]> [ #%zu ]> ", level, Lexer_GetLine() );
    
    #ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wformat-nonliteral"
    #endif
    vfprintf( fh, fmt, ap );
    #ifdef __clang__
    #pragma clang diagnostic pop
    #endif
    
    fprintf( fh, "\n" );
}

void Error( const char * fmt, ... )
{
    va_list ap;
    
    va_start( ap, fmt );
    Print( stderr, "ERROR", fmt, ap );
    va_end( ap );
}

void Warning( const char * fmt, ... )
{
    va_list ap;
    
    va_start( ap, fmt );
    Print( stderr, "WARNING", fmt, ap );
    va_end( ap );
}

void Debug( const char * fmt, ... )
{
    va_list ap;
    
    va_start( ap, fmt );
    Print( stdout, "DEBUG", fmt, ap );
    va_end( ap );
}
