/* 
  specific_GH12864_20_SPI.h
  
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

#ifndef SPECIFIC_GH12864_20_SPI_H
#define SPECIFIC_GH12864_20_SPI_H

#include <arduino.h>
#include <MGLCD.h>
#include <utility/MGLCD_buffered.h>

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

template<class T> class specific_GH12864_20_SPI
{
public :
  static const uint8_t Width = 128; // Screen Width
  static const uint8_t Height = 64; // Screen Height
  static const uint8_t PageNum = Height/8; // Number of character lines
  static const uint8_t ColumnNum = Width/MGLCD_FONT_WIDTH; // Number of character columns
  static const uint8_t DisplayOnCommand          = 0xaf;
  static const uint8_t DisplayOffCommand         = 0xae;
  static const uint8_t PageAddressCommand        = 0xb0;
  static const uint8_t UpperColumnAddressCommand = 0x10;
  static const uint8_t LowerColumnAddressCommand = 0x00;
  static const int8_t ColumnOffset = 2;

  T spi;

  explicit specific_GH12864_20_SPI(MGLCD_SpiPin2 pin2, uint32_t speedMaximum) : spi(pin2, speedMaximum) {}
  explicit specific_GH12864_20_SPI(MGLCD_SpiPin4 pin4, uint16_t delay) : spi(pin4,delay) {}

  inline void WriteData(uint8_t ctrl, uint8_t data) { spi.WriteData(ctrl,data); }
  void Reset(void);
  int16_t GetConstructionError(void) { return spi.GetConstructionError(); }
}; // specific_GH12864_20_SPI

template<class T> class GH12864_20_extension : public MGLCD_buffered<T> {
public:
  // constructor
  explicit GH12864_20_extension(MGLCD_SpiPin2 pin2, uint32_t speedMaximum)
    : MGLCD_buffered<T>::MGLCD_buffered(pin2,speedMaximum) {}; // for hardware SPI
  explicit GH12864_20_extension(MGLCD_SpiPin4 pin4, uint16_t delay=0) 
    : MGLCD_buffered<T>::MGLCD_buffered(pin4,delay) {}; // for software SPI

  // This class's original member functions
  int8_t SetLcdBiasRatio(uint8_t val);
  int8_t SetVbiasPotentiometer(uint8_t val);
  int8_t InvertDisplay(uint8_t val);
  
}; // GH12864_20_extension

// member functions of specific_GH12864_20_SPI

template<class T> void specific_GH12864_20_SPI<T>::Reset(void)
{
  spi.BeginSpi();
  delay(10);
  WriteData(MGLCD_COMMAND, 0xe2); // System Reset
  WriteData(MGLCD_COMMAND, 0x24); // Set Temp. Compensation
  WriteData(MGLCD_COMMAND, 0xc2); // Set LCD Mapping Control
  WriteData(MGLCD_COMMAND, 0x8c); // Set RAM Address Control
  WriteData(MGLCD_COMMAND, 0xa0); // Set Frame Rate
  WriteData(MGLCD_COMMAND, 0xeb); // Set LCD Vias Ratio
  WriteData(MGLCD_COMMAND, 0x81); // Set VBIAS Potentiometer 1st. byte
  WriteData(MGLCD_COMMAND, 150 ); // Set VBIAS Potentiometer 2nd. byte
  WriteData(MGLCD_COMMAND, 0xaf); // Set Display Enable
} // specific_GH12864_20_SPI<T>::Reset(void)

// member_functions of GH12864_20_extension

template<class T> int8_t GH12864_20_extension<T>::SetLcdBiasRatio(uint8_t val)
{
  if(val>3) return -1;
  
  GH12864_20_extension<T>::specific.WriteData(MGLCD_COMMAND, 0xe8+val);
  return 0;
} // GH12864_20_extension<T>::SetLcdBiasRatio

template<class T> int8_t GH12864_20_extension<T>::SetVbiasPotentiometer(uint8_t val)
{
  GH12864_20_extension<T>::specific.WriteData(MGLCD_COMMAND, 0x81);
  GH12864_20_extension<T>::specific.WriteData(MGLCD_COMMAND, val);
  
  return 0;
} // GH12864_20_extension<T>::SetVbiasPotentiometer

template<class T> int8_t GH12864_20_extension<T>::InvertDisplay(uint8_t val)
{
  if(val==MGLCD_NON_INVERT || val==MGLCD_INVERT) {
    GH12864_20_extension<T>::specific.WriteData(MGLCD_COMMAND, 0xa6+(val==MGLCD_INVERT));
    return 0;
  } else {
    return -1;
  } // if
} // GH12864_20_extension<T>::InvertDisplay

#endif // #ifndef SPECIFIC_GH12864_20_SPI_H
