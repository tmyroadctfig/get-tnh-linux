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
#ifndef _INC_UTILS_H
#define _INC_UTILS_H

//==========================================================================
//==========================================================================
#include <time.h>

//==========================================================================
//==========================================================================
unsigned long GetTick (void);
unsigned long TickElapsed (unsigned long aTick);
void GenDateString (char *aDest, time_t aTimeStamp, int aLen, int aLong);
const char *FileNameOnly (const char *aSrc);

//==========================================================================
//==========================================================================
#endif
