/* 
  MGLCD.cpp
  
  This file is part of Monochrome Graphic LCD library (MGLCD) for Arduino.
  
  MGLCD was developed by Synapse(Hiroshi Tanigawa) from 2012 to 2016.  This Library is
  originally distributed at "Synapse's history of making gadgets."  
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

#include "MGLCD.h"
#include <Arduino.h>

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

uint8_t MGLCD_CODE_STRAIGHT(uint8_t ch)
{
  return ch;
} // MGLCD_CODE_STRAIGHT

uint8_t MGLCD_CODE_UTF8(uint8_t ch)
{
  static uint8_t OneNum=0; // Number of successive 1s at MSBs first byte (Number of remaining bytes)
  static uint16_t Utf16; // UTF-16 code for multi byte character
  static boolean InUtf16Area; // Flag that shows character can be expressed as UTF-16 code

  if(OneNum==0) { // First byte
    uint8_t c;
    
    // Get OneNum
    c=ch;
    while(c&0x80) {
      c<<=1;
      OneNum++;
    } // while

    if(OneNum==1 || OneNum>6) { // First byte is in undefined area
      OneNum=0;
      return ch;
    } else if(OneNum==0) { // 1-byte character
      return ch;
    } else { // Multi byte character
      InUtf16Area=true;
      Utf16=ch&((1<<(7-OneNum--))-1); // Get first byte
    } // if
  } else { // not first byte
    if((ch&0xc0)!=0x80) { // First byte appears illegally
      OneNum=0;
      return ch;
    } // if
    if(Utf16&0xfc00) InUtf16Area=false;
    Utf16=(Utf16<<6)+(ch&0x3f);
    if(--OneNum==0) { // Last byte
      return (InUtf16Area && Utf16>=0xff61 && Utf16<=0xff9f) ? Utf16-(0xff61-0xa1) // kana
                                                             : ' ';                // other character
    } // if
  } // if
  
  return 0;
} // MGLCD_CODE_UTF8

int8_t CodeModeToIndex(PDecodeFunc CodeMode)
{
  if(CodeMode==MGLCD_CODE_STRAIGHT) return 0;
  if(CodeMode==MGLCD_CODE_UTF8    ) return 1;
  return -1;
} // CodeModeToIndex

PDecodeFunc IndexToCodeMode(uint8_t index)
{
  switch(index) {
    case 0:
      return MGLCD_CODE_STRAIGHT;
    case 1:
      return MGLCD_CODE_UTF8;
    default:
      return NULL;
  }
} // IndexToCodeMode
