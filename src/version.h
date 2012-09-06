//==========================================================================
// Application Information
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
#ifndef _INC_VERSION_H
#define _INC_VERSION_H

//==========================================================================
//==========================================================================
#define STR_PRJ_NAME1_(x)       #x
#define STR_PRJ_NAME_(x)        STR_PRJ_NAME1_(x)
#define STR_PROGNAME            STR_PRJ_NAME_(PRJ_NAME)

#define STR_VERSION             "V1.01.00"
#define STR_COPYRIGHT           "Copyright 2012 DogRatIan"

//==========================================================================
// Version Informations
// Rule: For major changes, inc the 1st digit. (V1.00.00 -> V2.00.00)
//       For minor changes, inc the 2nd digit. (V1.00.00 -> V1.01.00)
//       For branches, inc the 3rd digit.      (V1.00.00 -> V1.00.01)
//==========================================================================
/*
    History
   ------------------------------------------------------------------------
    2012-SEP-06 (Version 1.00.00)
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Released under GPLv3.
*/
//==========================================================================
//==========================================================================
#endif
