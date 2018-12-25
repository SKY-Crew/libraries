/* 
  MGLCD.h
  
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

#ifndef MGLCD_H
#define MGLCD_H

#include <arduino.h>
#include <Print.h>

#if defined(ARDUINO) && ARDUINO<150
#define ARDUINO_ARCH_AVR
#endif

// version 
#define MGLCD_VERSION 42 // version 0.42

// Common constants
#define MGLCD_FONT_WIDTH  6   // width of font
#define MGLCD_FONT_HEIGHT 8   // height of font

// Constants for SetInvertMode
#define MGLCD_NON_INVERT 0
#define MGLCD_INVERT     1

// Constants for SetScrollMode
#define MGLCD_NON_SCROLL 0
#define MGLCD_SCROLL     1

// Constants for UserChars
#define MGLCD_ROM 0
#define MGLCD_RAM 1

// Constants for digitalPinToPort/digitalPinToBitMask functons
#ifdef __AVR_ATmega32U4__
#define MGLCD_NUM_DIGITAL_PINS 30 // for Leonardo
#else
#define MGLCD_NUM_DIGITAL_PINS NUM_DIGITAL_PINS // for others
#endif

// type definition
typedef uint8_t (*PDecodeFunc)(uint8_t);

// function prototypes
uint8_t MGLCD_CODE_STRAIGHT(uint8_t ch);
uint8_t MGLCD_CODE_UTF8(uint8_t ch);
int8_t CodeModeToIndex(PDecodeFunc CodeMode);
PDecodeFunc IndexToCodeMode(uint8_t index);

#include "utility/MGLCD_direct.h"
#include "utility/specific_SG12232.h"
#include "utility/specific_SG12864.h"
#include "utility/specific_S12232ZA.h"
#include "utility/MGLCD_indirect.h"

#include "utility/MGLCD_buffered.h"
#include "utility/SoftwareSpiOut.h"
#include "utility/specific_AQM1248A_SPI.h"
#include "utility/specific_GH12864_20_SPI.h"

typedef AQM1248A_extension<specific_AQM1248A_SPI<SoftwareSpiOut> > MGLCD_AQM1248A_SoftwareSPI;
typedef GH12864_20_extension<specific_GH12864_20_SPI<SoftwareSpiOut> > MGLCD_GH12864_20_SoftwareSPI;

#endif // #ifndef MGLCD_H
