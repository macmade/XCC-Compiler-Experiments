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
 * @header      Variable.h
 * @copyright   (c) 2020, Jean-David Gadina - www.xs-labs.com
 * @dicussion   Adapted from "Compiler Design in C" by Allen I. Holub.
 *              ISBN 0-13-155045-4 - https://holub.com/compiler
 */

#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include "String.h"

typedef struct Variable * VariableRef;

typedef enum
{
    TypeChar,
    TypeInt,
    TypeFloat,
    TypeDouble,
    TypePointer,
    TypeStruct
} Type;

typedef enum
{
    QualifierConst    = 1 << 0,
    QualifierVolatile = 1 << 1
} Qualifier;

VariableRef Variable_Create( void );
VariableRef Variable_Retain( VariableRef var );
void        Variable_Release( VariableRef var );
void        Variable_SetName( VariableRef var, StringRef name );
void        Variable_SetType( VariableRef var, Type type );
void        Variable_AddQualifier( VariableRef var, Qualifier qualifier );
void        Variable_RemoveQualifier( VariableRef var, Qualifier qualifier );
bool        Variable_HasQualifier( VariableRef var, Qualifier qualifier );
void        Variable_SetAsPointer( VariableRef var, VariableRef pointee );
void        Variable_SetAsArray( VariableRef var, size_t size );
void        Variable_SetAsStruct( VariableRef var, StringRef identifier );
void        Variable_PrintDescription( VariableRef var, FILE * fh );

#endif /* VARIABLE_H */
