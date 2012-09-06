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
//==========================================================================
#ifndef _INC_DEBUG_H
#define _INC_DEBUG_H

//==========================================================================
//==========================================================================

//==========================================================================
//==========================================================================
#if (DEBUG)

#define DEBUG_INIT()            DebugInit ()
#define DEBUG_EXIT()            DebugExit ()
#define DEBUG_PRINTF(x...)      DebugPrintf (x)
#define DEBUG_HEX2STRING(x,y)   DebugHex2String (x,y)

#else   // #if (DEBUG)

#define DEBUG_INIT()            {}
#define DEBUG_EXIT()            {}
#define DEBUG_PRINTF(x...)      {}
#define DEBUG_HEX2STRING(x,y)   {}

#endif   // #if (DEBUG)


//==========================================================================
//==========================================================================
void DebugInit (void);
void DebugExit (void);
void DebugPrintf (const char *aMsg, ...);
void DebugHex2String (const unsigned char *aSrc, int aLen);

//==========================================================================
//==========================================================================
#endif
