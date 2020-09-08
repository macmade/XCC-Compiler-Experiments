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
 * @file        main.c
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 * @dicussion   Adapted from "Compiler Design in C" by Allen I. Holub.
 *              ISBN 0-13-155045-4 - https://holub.com/compiler
 */

#include <stdlib.h>
#include <stdio.h>
#include "Parser.h"
#include "Variable.h"

int main( void )
{
    VariableRef v1;
    VariableRef v2;
    VariableRef v3;
    VariableRef v4;
    VariableRef v5;

    v1 = Variable_Create();
    v2 = Variable_Create();
    v3 = Variable_Create();
    v4 = Variable_Create();
    v5 = Variable_Create();

    {
        StringRef name;

        name = String_CreateWithCString( "var1" );

        Variable_SetName( v1, name );
        Variable_AddQualifier( v1, QualifierConst );
        Variable_AddQualifier( v1, QualifierVolatile );
        String_Release( name );
    }

    {
        StringRef name;

        name = String_CreateWithCString( "var2" );

        Variable_SetName( v2, name );
        Variable_SetType( v2, TypeDouble );
        Variable_SetAsArray( v2, 42 );
        String_Release( name );
    }

    {
        StringRef name;
        StringRef id;

        name = String_CreateWithCString( "var3" );
        id   = String_CreateWithCString( "TestStruct" );

        Variable_SetName( v3, name );
        Variable_SetAsStruct( v3, id );
        String_Release( name );
        String_Release( id );
    }

    {
        StringRef name;

        name = String_CreateWithCString( "var4" );

        Variable_SetName( v4, name );
        Variable_AddQualifier( v4, QualifierConst );
        Variable_SetAsPointer( v4, v1 );
        String_Release( name );
    }

    {
        StringRef name;

        name = String_CreateWithCString( "var5" );

        Variable_SetName( v5, name );
        Variable_SetAsPointer( v5, v4 );
        String_Release( name );
    }

    Variable_PrintDescription( v1, stdout );
    Variable_PrintDescription( v2, stdout );
    Variable_PrintDescription( v3, stdout );
    Variable_PrintDescription( v4, stdout );
    Variable_PrintDescription( v5, stdout );

    Variable_Release( v1 );
    Variable_Release( v2 );

    return EXIT_SUCCESS;
}
