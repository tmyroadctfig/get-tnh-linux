//==========================================================================
// Showing debug message
//==========================================================================
//  Copyright (c) 2012 DogRatIan.  All rights reserved.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//==========================================================================
#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

//==========================================================================
//==========================================================================

//==========================================================================
// Initialize of debug mode
//==========================================================================
void DebugInit (void)
{
}

//==========================================================================
// Exit of Debug
//==========================================================================
void DebugExit (void)
{
}

//==========================================================================
// Show binary array in HEX string
//==========================================================================
void DebugHex2String (const unsigned char *aSrc, int aLen)
{
    printf ("[DEBUG]");
    for (int i = 0; i < aLen; i ++)
    {
        printf ("%02X ", *aSrc);
        aSrc ++;
    }
    printf ("\n");
}

//==========================================================================
// Format and show string
//==========================================================================
void DebugPrintf (const char *aMsg, ...)
{
    va_list args;
    printf ("[DEBUG]");
    va_start (args, aMsg);
    vprintf (aMsg, args);
    va_end (args);
    printf ("\n");
}

