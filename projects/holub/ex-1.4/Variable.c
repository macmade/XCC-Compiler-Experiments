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
 * @file        Variable.c
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 * @dicussion   Adapted from "Compiler Design in C" by Allen I. Holub.
 *              ISBN 0-13-155045-4 - https://holub.com/compiler
 */

#include "Variable.h"
#include <stdlib.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

struct Variable
{
    uint64_t          rc;
    StringRef         name;
    Type              type;
    uint64_t          qualifiers;
    struct Variable * pointee;
    int64_t           arraySize;
    StringRef         structName;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

VariableRef Variable_Create()
{
    VariableRef var;

    if( ( var = calloc( 1, sizeof( struct Variable ) ) ) == NULL )
    {
        return NULL;
    }

    var->rc   = 1;
    var->name = String_Create();
    var->type = TypeInt;

    return var;
}

VariableRef Variable_Retain( VariableRef var )
{
    if( var == NULL )
    {
        return NULL;
    }

    var->rc++;

    return var;
}

void Variable_Release( VariableRef var )
{
    if( var == NULL )
    {
        return;
    }

    if( --( var->rc ) > 0 )
    {
        return;
    }

    String_Release( var->name );
    Variable_Release( var->pointee );
    free( var );
}

void Variable_SetName( VariableRef var, StringRef name )
{
    if( var == NULL )
    {
        return;
    }

    String_Release( var->name );

    var->name = String_Retain( name );
}

void Variable_SetType( VariableRef var, Type type )
{
    if( var == NULL )
    {
        return;
    }

    Variable_Release( var->pointee );
    String_Release( var->structName );

    var->type       = type;
    var->pointee    = NULL;
    var->structName = NULL;
    var->arraySize  = 0;
}

void Variable_AddQualifier( VariableRef var, Qualifier qualifier )
{
    if( var == NULL )
    {
        return;
    }

    var->qualifiers |= qualifier;
}

void Variable_RemoveQualifier( VariableRef var, Qualifier qualifier )
{
    if( var == NULL )
    {
        return;
    }

    var->qualifiers &= ~qualifier;
}

bool Variable_HasQualifier( VariableRef var, Qualifier qualifier )
{
    if( var == NULL )
    {
        return false;
    }

    return ( var->qualifiers & qualifier ) != 0;
}

void Variable_SetAsPointer( VariableRef var, VariableRef pointee )
{
    if( var == NULL )
    {
        return;
    }

    Variable_SetType( var, TypePointer );

    var->pointee = Variable_Retain( pointee );
}

void Variable_SetAsArray( VariableRef var, size_t size )
{
    if( var == NULL )
    {
        return;
    }

    var->arraySize = ( size == 0 ) ? -1 : ( int64_t )size;
}

void Variable_SetAsStruct( VariableRef var, StringRef identifier )
{
    if( var == NULL )
    {
        return;
    }

    Variable_SetType( var, TypeStruct );

    var->structName = String_Retain( identifier );
}

void Variable_PrintDescription( VariableRef var, FILE * fh )
{
    if( fh == NULL )
    {
        return;
    }

    if( var == NULL )
    {
        printf( "<null>\n" );

        return;
    }

    if( var->type != TypePointer )
    {
        if( Variable_HasQualifier( var, QualifierConst ) )
        {
            printf( "const " );
        }

        if( Variable_HasQualifier( var, QualifierVolatile ) )
        {
            printf( "volatile " );
        }
    }

    if( var->type == TypeChar )
    {
        printf( "char " );
    }
    else if( var->type == TypeInt )
    {
        printf( "int " );
    }
    else if( var->type == TypeFloat )
    {
        printf( "float " );
    }
    else if( var->type == TypeDouble )
    {
        printf( "double " );
    }
    else if( var->type == TypeStruct )
    {
        if( String_GetLength( var->structName ) == 0 )
        {
            printf( "struct <unknown> " );
        }
        else
        {
            printf( "struct %s ", String_GetCString( var->structName ) );
        }
    }
    else if( var->type == TypePointer )
    {
        printf( "* " );

        if( Variable_HasQualifier( var, QualifierConst ) )
        {
            printf( "const " );
        }

        if( Variable_HasQualifier( var, QualifierVolatile ) )
        {
            printf( "volatile " );
        }
    }

    if( String_GetLength( var->name ) == 0 )
    {
        printf( "<unnamed>" );
    }
    else
    {
        printf( "%s", String_GetCString( var->name ) );
    }

    if( var->arraySize < 0 )
    {
        printf( "[]" );
    }
    else if( var->arraySize > 0 )
    {
        printf( "[ %lli ]", var->arraySize );
    }

    printf( ";\n" );
}
