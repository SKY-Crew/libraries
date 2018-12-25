/*
  MGLCD_font.cpp

  This file is part of Monochrome Graphic LCD library (MGLCD) for Arduino.

  MGLCD was developed by Synapse(Hiroshi Tanigawa) from 2012 to 2016.  This Library is
  originally distributed "Synapse's history of making gadgets."
  <https://synapse.kyoto>

  MGLCD is now under beta testing, so specification may be changed
  in the future.

  MGLCD is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 2.1 of the License, or
  (at your option) any later version.

  MGLCD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with MGLCD.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "utility/MGLCD_font.h"
#define s ((((((((((((((((0
#define M <<1)+1)
#define _ <<1))

PROGMEM const uint8_t FontBmp1[][5] =
{
  { // 20
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 21
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ M M M M  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 22
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ M M M  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ M M M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 23
    s  _ _ _ M _ M _ _  ,
    s  _ M M M M M M M  ,
    s  _ _ _ M _ M _ _  ,
    s  _ M M M M M M M  ,
    s  _ _ _ M _ M _ _
  },
  { // 24
    s  _ _ M _ _ M _ _  ,
    s  _ _ M _ M _ M _  ,
    s  _ M M M M M M M  ,
    s  _ _ M _ M _ M _  ,
    s  _ _ _ M _ _ M _
  },
  { // 25
    s  _ _ M _ _ _ M M  ,
    s  _ _ _ M _ _ M M  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ M M _ _ M _ _  ,
    s  _ M M _ _ _ M _
  },
  { // 26
    s  _ _ M M _ M M _  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ M _ M _ M  ,
    s  _ _ M _ _ _ M _  ,
    s  _ M _ M _ _ _ _
  },
  { // 27
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ M _ M  ,
    s  _ _ _ _ _ _ M M  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 28
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ M M M _ _  ,
    s  _ _ M _ _ _ M _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 29
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M M M _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 2A
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ M M M M M _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ M _ M _ _
  },
  { // 2B
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ M M M M M _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _
  },
  { // 2C
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ M _ _ _ _  ,
    s  _ _ M M _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 2D
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _
  },
  { // 2E
    s  _ _ _ _ _ _ _ _  ,
    s  _ M M _ _ _ _ _  ,
    s  _ M M _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 2F
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ _ M _
  },
  { // 30
    s  _ _ M M M M M _  ,
    s  _ M _ M _ _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ _ M _ M  ,
    s  _ _ M M M M M _
  },
  { // 31
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M M M M M M M  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 32
    s  _ M _ _ _ _ M _  ,
    s  _ M M _ _ _ _ M  ,
    s  _ M _ M _ _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ _ M M _
  },
  { // 33
    s  _ _ M _ _ _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ _ _ M _ M  ,
    s  _ M _ _ M _ M M  ,
    s  _ _ M M _ _ _ M
  },
  { // 34
    s  _ _ _ M M _ _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ M _ _ M _  ,
    s  _ M M M M M M M  ,
    s  _ _ _ M _ _ _ _
  },
  { // 35
    s  _ _ M _ _ M M M  ,
    s  _ M _ _ _ M _ M  ,
    s  _ M _ _ _ M _ M  ,
    s  _ M _ _ _ M _ M  ,
    s  _ _ M M M _ _ M
  },
  { // 36
    s  _ _ M M M M _ _  ,
    s  _ M _ _ M _ M _  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ _ M M _ _ _ _
  },
  { // 37
    s  _ _ _ _ _ _ _ M  ,
    s  _ M M M _ _ _ M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ _ M _ M  ,
    s  _ _ _ _ _ _ M M
  },
  { // 38
    s  _ _ M M _ M M _  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ _ M M _ M M _
  },
  { // 39
    s  _ _ _ _ _ M M _  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ _ M _ M _ _ M  ,
    s  _ _ _ M M M M _
  },
  { // 3A
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ M M _ M M _  ,
    s  _ _ M M _ M M _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 3B
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ M _ M M _  ,
    s  _ _ M M _ M M _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 3C
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ M _ _ _ M _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 3D
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ M _ M _ _
  },
  { // 3E
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ _ M _ _ _
  },
  { // 3F
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ _ _ M  ,
    s  _ M _ M _ _ _ M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ _ M M _
  },
  { // 40
    s  _ _ M M _ _ M _  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M M M M _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ M M M M M _
  },
  { // 41
    s  _ M M M M M M _  ,
    s  _ _ _ M _ _ _ M  ,
    s  _ _ _ M _ _ _ M  ,
    s  _ _ _ M _ _ _ M  ,
    s  _ M M M M M M _
  },
  { // 42
    s  _ M M M M M M M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ _ M M _ M M _
  },
  { // 43
    s  _ _ M M M M M _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ M _ _ _ M _
  },
  { // 44
    s  _ M M M M M M M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M M M _ _
  },
  { // 45
    s  _ M M M M M M M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ _ _ _ M
  },
  { // 46
    s  _ M M M M M M M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ _ _ _ M
  },
  { // 47
    s  _ _ M M M M M _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M M M M _ M _
  },
  { // 48
    s  _ M M M M M M M  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ M M M M M M M
  },
  { // 49
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M M M M M M M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 4A
    s  _ _ M _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ M M M M M M  ,
    s  _ _ _ _ _ _ _ M
  },
  { // 4B
    s  _ M M M M M M M  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ M _ _ _ M _  ,
    s  _ M _ _ _ _ _ M
  },
  { // 4C
    s  _ M M M M M M M  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _
  },
  { // 4D
    s  _ M M M M M M M  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ M M _ _  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ M M M M M M M
  },
  { // 4E
    s  _ M M M M M M M  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ M M M M M M M
  },
  { // 4F
    s  _ _ M M M M M _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ M M M M M _
  },
  { // 50
    s  _ M M M M M M M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ _ M M _
  },
  { // 51
    s  _ _ M M M M M _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ M _ _ _ M  ,
    s  _ _ M _ _ _ _ M  ,
    s  _ M _ M M M M _
  },
  { // 52
    s  _ M M M M M M M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ M M _ _ M  ,
    s  _ _ M _ M _ _ M  ,
    s  _ M _ _ _ M M _
  },
  { // 53
    s  _ M _ _ _ M M _  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ _ M M _ _ _ M
  },
  { // 54
    s  _ _ _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ _ M  ,
    s  _ M M M M M M M  ,
    s  _ _ _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ _ M
  },
  { // 55
    s  _ _ M M M M M M  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M M M M M M
  },
  { // 56
    s  _ _ _ M M M M M  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M M M M M
  },
  { // 57
    s  _ _ M M M M M M  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M M M _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M M M M M M
  },
  { // 58
    s  _ M M _ _ _ M M  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ M M _ _ _ M M
  },
  { // 59
    s  _ _ _ _ _ M M M  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ M M M _ _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ _ M M M
  },
  { // 5A
    s  _ M M _ _ _ _ M  ,
    s  _ M _ M _ _ _ M  ,
    s  _ M _ _ M _ _ M  ,
    s  _ M _ _ _ M _ M  ,
    s  _ M _ _ _ _ M M
  },
  { // 5B
    s  _ _ _ _ _ _ _ _  ,
    s  _ M M M M M M M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 5C
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ _ M _ _ _ _ _
  },
  { // 5D
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M M M M M M M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 5E
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ M _ _
  },
  { // 5F
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _
  },
  { // 60
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 61
    s  _ _ M _ _ _ _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M M M M _ _ _
  },
  { // 62
    s  _ M M M M M M M  ,
    s  _ M _ _ M _ _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ _ M M M _ _ _
  },
  { // 63
    s  _ _ M M M _ _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ _ M _ _ _ _ _
  },
  { // 64
    s  _ _ M M M _ _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ M _ _ _  ,
    s  _ M M M M M M M
  },
  { // 65
    s  _ _ M M M _ _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ _ _ M M _ _ _
  },
  { // 66
    s  _ _ _ _ M _ _ _  ,
    s  _ M M M M M M _  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ M _
  },
  { // 67
    s  _ _ _ _ M M _ _  ,
    s  _ M _ M _ _ M _  ,
    s  _ M _ M _ _ M _  ,
    s  _ M _ M _ _ M _  ,
    s  _ _ M M M M M _
  },
  { // 68
    s  _ M M M M M M M  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ M M M M _ _ _
  },
  { // 69
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M M M M M _ M  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 6A
    s  _ _ M _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ _ M M M M _ M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 6B
    s  _ M M M M M M M  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ _ M _ M _ _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 6C
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ M M M M M M M  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 6D
    s  _ M M M M M _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ M M _ _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ M M M M _ _ _
  },
  { // 6E
    s  _ M M M M M _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ M M M M _ _ _
  },
  { // 6F
    s  _ _ M M M _ _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ _ M M M _ _ _
  },
  { // 70
    s  _ M M M M M _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ _ M _ _ _
  },
  { // 71
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ M M _ _ _  ,
    s  _ M M M M M _ _
  },
  { // 72
    s  _ M M M M M _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ M _ _ _
  },
  { // 73
    s  _ M _ _ M _ _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ _ M _ _ _ _ _
  },
  { // 74
    s  _ _ _ _ _ M _ _  ,
    s  _ _ M M M M M M  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _
  },
  { // 75
    s  _ _ M M M M _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ M M M M M _ _
  },
  { // 76
    s  _ _ _ M M M _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M M M _ _
  },
  { // 77
    s  _ _ M M M M _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M M _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M M M M _ _
  },
  { // 78
    s  _ M _ _ _ M _ _  ,
    s  _ _ M _ M _ _ _  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ _ M _ M _ _ _  ,
    s  _ M _ _ _ M _ _
  },
  { // 79
    s  _ _ _ _ M M _ _  ,
    s  _ M _ M _ _ _ _  ,
    s  _ M _ M _ _ _ _  ,
    s  _ M _ M _ _ _ _  ,
    s  _ _ M M M M _ _
  },
  { // 7A
    s  _ M _ _ _ M _ _  ,
    s  _ M M _ _ M _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M _ _ M M _ _  ,
    s  _ M _ _ _ M _ _
  },
  { // 7B
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ M M _ M M _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 7C
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ M M M M M M M  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 7D
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ M M _ M M _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // 7E
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ _ _ _ M _ _ _
  },
  { // 7F
    s  _ _ _ _ _ _ _ M  ,
    s  _ _ M _ M _ M _  ,
    s  _ M M M M M _ _  ,
    s  _ _ M _ M _ M _  ,
    s  _ _ _ _ _ _ _ M
  }
};

PROGMEM const uint8_t FontBmp2[][5] =
{
  { // A0
    s  M M M M M M M M  ,
    s  M M M M M M M M  ,
    s  M M M M M M M M  ,
    s  M M M M M M M M  ,
    s  M M M M M M M M
  },
  { // A1
    s  _ M M M _ _ _ _  ,
    s  _ M _ M _ _ _ _  ,
    s  _ M M M _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // A2
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ M M M M  ,
    s  _ _ _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ _ M
  },
  { // A3
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ M M M M _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // A4
    s  _ _ _ M _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // A5
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ M M _ _ _  ,
    s  _ _ _ M M _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // A6
    s  _ _ _ _ M _ M _  ,
    s  _ _ _ _ M _ M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ _ M _ M _ M _  ,
    s  _ _ _ M M M M _
  },
  { // A7
    s  _ _ _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ _ M M _ M _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ _ M M _ _
  },
  { // A8
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ M M M M _ _ _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // A9
    s  _ _ _ M M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ M _ _ M M _ _  ,
    s  _ M _ _ M _ _ _  ,
    s  _ _ M M M _ _ _
  },
  { // AA
    s  _ M _ _ M _ _ _  ,
    s  _ M _ _ M _ _ _  ,
    s  _ M M M M _ _ _  ,
    s  _ M _ _ M _ _ _  ,
    s  _ M _ _ M _ _ _
  },
  { // AB
    s  _ M _ _ M _ _ _  ,
    s  _ _ M _ M _ _ _  ,
    s  _ _ _ M M _ _ _  ,
    s  _ M M M M M _ _  ,
    s  _ _ _ _ M _ _ _
  },
  { // AC
    s  _ _ _ _ M _ _ _  ,
    s  _ M M M M M _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ M _ M _ _ _  ,
    s  _ _ _ M M _ _ _
  },
  { // AD
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ M _ _ _  ,
    s  _ M _ _ M _ _ _  ,
    s  _ M M M M _ _ _  ,
    s  _ M _ _ _ _ _ _
  },
  { // AE
    s  _ M _ M _ M _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M _ M _ M _ _  ,
    s  _ M M M M M _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // AF
    s  _ _ _ M M _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ M M _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M M M _ _ _
  },
  { // B0
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ _ M _ _ _
  },
  { // B1
    s  _ _ _ _ _ _ _ M  ,
    s  _ M _ _ _ _ _ M  ,
    s  _ _ M M M M _ M  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ _ M M M
  },
  { // B2
    s  _ _ _ M _ _ _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ M M M M M _ _  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ _ _ M
  },
  { // B3
    s  _ _ _ _ M M M _  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ M _ _ _ _ M M  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M M M M _
  },
  { // B4
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M M M M M M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _
  },
  { // B5
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M _ _ M _  ,
    s  _ _ _ _ M _ M _  ,
    s  _ M M M M M M M  ,
    s  _ _ _ _ _ _ M _
  },
  { // B6
    s  _ M _ _ _ _ M _  ,
    s  _ _ M M M M M M  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ _ M M M M M _
  },
  { // B7
    s  _ _ _ _ M _ M _  ,
    s  _ _ _ _ M _ M _  ,
    s  _ M M M M M M M  ,
    s  _ _ _ _ M _ M _  ,
    s  _ _ _ _ M _ M _
  },
  { // B8
    s  _ _ _ _ M _ _ _  ,
    s  _ M _ _ _ M M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M M M M _
  },
  { // B9
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ _ M M  ,
    s  _ M _ _ _ _ M _  ,
    s  _ _ M M M M M _  ,
    s  _ _ _ _ _ _ M _
  },
  { // BA
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M M M M M M _
  },
  { // BB
    s  _ _ _ _ _ _ M _  ,
    s  _ M _ _ M M M M  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M M M M M  ,
    s  _ _ _ _ _ _ M _
  },
  { // BC
    s  _ M _ _ M _ M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M M M _ _
  },
  { // BD
    s  _ M _ _ _ _ M _  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M _ _ M _  ,
    s  _ _ M _ M _ M _  ,
    s  _ M _ _ _ M M _
  },
  { // BE
    s  _ _ _ _ _ _ M _  ,
    s  _ _ M M M M M M  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ M _ _ _ M M _
  },
  { // BF
    s  _ _ _ _ _ M M _  ,
    s  _ M _ _ M _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M M M M _
  },
  { // C0
    s  _ _ _ _ M _ _ _  ,
    s  _ M _ _ _ M M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ _ M M _ _ M _  ,
    s  _ _ _ M M M M _
  },
  { // C1
    s  _ _ _ _ M _ M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ _ M M M M M _  ,
    s  _ _ _ _ M _ _ M  ,
    s  _ _ _ _ M _ _ _
  },
  { // C2
    s  _ _ _ _ M M M _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ M M M _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M M M M _
  },
  { // C3
    s  _ _ _ _ _ M _ _  ,
    s  _ M _ _ _ M _ M  ,
    s  _ _ M M M M _ M  ,
    s  _ _ _ _ _ M _ M  ,
    s  _ _ _ _ _ M _ _
  },
  { // C4
    s  _ _ _ _ _ _ _ _  ,
    s  _ M M M M M M M  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // C5
    s  _ M _ _ _ M _ _  ,
    s  _ _ M _ _ M _ _  ,
    s  _ _ _ M M M M M  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ M _ _
  },
  { // C6
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ _ _
  },
  { // C7
    s  _ M _ _ _ _ M _  ,
    s  _ _ M _ M _ M _  ,
    s  _ _ _ M _ _ M _  ,
    s  _ _ M _ M _ M _  ,
    s  _ _ _ _ _ M M _
  },
  { // C8
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M _ _ M _  ,
    s  _ M M M M _ M M  ,
    s  _ _ _ M _ M M _  ,
    s  _ _ M _ _ _ M _
  },
  { // C9
    s  _ _ _ _ _ _ _ _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M M M M M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // CA
    s  _ M M M M _ _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ M M M M _ _ _
  },
  { // CB
    s  _ _ M M M M M M  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _  ,
    s  _ M _ _ _ M _ _
  },
  { // CC
    s  _ _ _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M M M M _
  },
  { // CD
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ M _ _ _  ,
    s  _ _ M M _ _ _ _
  },
  { // CE
    s  _ _ M M _ _ M _  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ M M M M M M M  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ _ M M _ _ M _
  },
  { // CF
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ M _ _ M _  ,
    s  _ _ M _ _ _ M _  ,
    s  _ M _ M _ _ M _  ,
    s  _ _ _ _ M M M _
  },
  { // D0
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ M _ M _ M _  ,
    s  _ _ M _ M _ M _  ,
    s  _ _ M _ M _ M _  ,
    s  _ M _ _ _ _ _ _
  },
  { // D1
    s  _ _ M M M _ _ _  ,
    s  _ _ M _ _ M _ _  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ M M M _ _ _ _
  },
  { // D2
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ M _ _ _  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ _ M _ M _ _ _  ,
    s  _ _ _ _ _ M M _
  },
  { // D3
    s  _ _ _ _ M _ M _  ,
    s  _ _ M M M M M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ M _ _ M _ M _
  },
  { // D4
    s  _ _ _ _ _ M _ _  ,
    s  _ M M M M M M M  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ M _ M _ _  ,
    s  _ _ _ _ M M _ _
  },
  { // D5
    s  _ M _ _ _ _ _ _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M M M M M M _  ,
    s  _ M _ _ _ _ _ _
  },
  { // D6
    s  _ M _ _ M _ M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ M _ _ M _ M _  ,
    s  _ M M M M M M _
  },
  { // D7
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ M _ M  ,
    s  _ M _ _ _ M _ M  ,
    s  _ _ M _ _ M _ M  ,
    s  _ _ _ M M M _ _
  },
  { // D8
    s  _ _ _ _ M M M M  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M M M M M  ,
    s  _ _ _ _ _ _ _ _
  },
  { // D9
    s  _ M M M M M _ _  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ M M M M M M _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M M _ _ _ _
  },
  { // DA
    s  _ M M M M M M _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M _ _ _ _  ,
    s  _ _ _ _ M _ _ _
  },
  { // DB
    s  _ M M M M M M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M M M M M M _
  },
  { // DC
    s  _ _ _ _ M M M _  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ _ M _ _ _ M _  ,
    s  _ _ _ M M M M _
  },
  { // DD
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ M _  ,
    s  _ M _ _ _ _ _ _  ,
    s  _ _ M _ _ _ _ _  ,
    s  _ _ _ M M _ _ _
  },
  { // DE
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ M _ _  ,
    s  _ _ _ _ _ _ _ M  ,
    s  _ _ _ _ _ _ M _  ,
    s  _ _ _ _ _ _ _ _
  },
  { // DF
    s  _ _ _ _ _ M M M  ,
    s  _ _ _ _ _ M _ M  ,
    s  _ _ _ _ _ M M M  ,
    s  _ _ _ _ _ _ _ _  ,
    s  _ _ _ _ _ _ _ _
  }
};

#undef s
#undef M
#undef _
