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
 * @file        String.c
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 * @dicussion   Adapted from "Compiler Design in C" by Allen I. Holub.
 *              ISBN 0-13-155045-4 - https://holub.com/compiler
 */

#include "String.h"
#include <stdlib.h>
#include <string.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

struct String
{
    uint64_t rc;
    char *   cstr;
    size_t   length;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

StringRef String_Create( void )
{
    return String_CreateWithBytes( NULL, 0 );
}

StringRef String_CreateWithCString( const char * s )
{
    size_t length;

    length = ( s == NULL ) ? 0 : strlen( s );

    return String_CreateWithBytes( s, length );
}

StringRef String_CreateWithBytes( const char * s, size_t length )
{
    StringRef str;

    if( s == NULL )
    {
        length = 0;
    }

    if( ( str = calloc( 1, sizeof( struct String ) ) ) == NULL )
    {
        return NULL;
    }

    if( ( str->cstr = calloc( 1, length + 1 ) ) == NULL )
    {
        free( str );

        return NULL;
    }

    if( length > 0 )
    {
        strncpy( str->cstr, s, length );
    }

    str->rc     = 1;
    str->length = length;

    return str;
}

StringRef String_Retain( StringRef str )
{
    if( str == NULL )
    {
        return NULL;
    }

    str->rc++;

    return str;
}

void String_Release( StringRef str )
{
    if( str == NULL )
    {
        return;
    }

    if( --( str->rc ) > 0 )
    {
        return;
    }

    free( str->cstr );
    free( str );
}

const char * String_GetCString( StringRef str )
{
    if( str == NULL )
    {
        return NULL;
    }

    return str->cstr;
}

size_t String_GetLength( StringRef str )
{
    if( str == NULL )
    {
        return 0;
    }

    return str->length;
}
