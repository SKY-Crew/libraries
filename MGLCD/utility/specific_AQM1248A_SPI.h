/* 
  specific_AQM1248A_SPI.h
  
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

#ifndef SPECIFIC_AQM1248A_SPI_H
#define SPECIFIC_AQM1248A_SPI_H

#include <arduino.h>
#include <MGLCD.h>
#include <utility/MGLCD_buffered.h>

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

template<class T> class specific_AQM1248A_SPI
{
public :
  static const uint8_t Width = 128; // Screen Width
  static const uint8_t Height = 48; // Screen Height
  static const uint8_t PageNum = Height/8; // Number of character lines
  static const uint8_t ColumnNum = Width/MGLCD_FONT_WIDTH; // Number of character columns
  static const uint8_t DisplayOnCommand          = 0xaf;
  static const uint8_t DisplayOffCommand         = 0xae;
  static const uint8_t DisplayLineCommand        = 0x40;
  static const uint8_t PageAddressCommand        = 0xb0;
  static const uint8_t UpperColumnAddressCommand = 0x10;
  static const uint8_t LowerColumnAddressCommand = 0x00;
  static const int8_t ColumnOffset = 0;

  T spi;

  explicit specific_AQM1248A_SPI(MGLCD_SpiPin2 pin2, uint32_t speedMaximum) : spi(pin2, speedMaximum) {}
  explicit specific_AQM1248A_SPI(MGLCD_SpiPin4 pin4, uint16_t delay) : spi(pin4, delay) {}

  inline void WriteData(uint8_t ctrl, uint8_t data) { spi.WriteData(ctrl,data); }
  void Reset(void);
  int16_t GetConstructionError(void) { return spi.GetConstructionError(); }
}; // specific_AQM1248A_SPI

template<class T> class AQM1248A_extension : public MGLCD_buffered<T> {
public:
  // constructor
  explicit AQM1248A_extension(MGLCD_SpiPin2 pin2, uint32_t speedMaximum)
    : MGLCD_buffered<T>::MGLCD_buffered(pin2,speedMaximum) {}; // for hardware SPI
  explicit AQM1248A_extension(MGLCD_SpiPin4 pin4, uint16_t delay=0)
    : MGLCD_buffered<T>::MGLCD_buffered(pin4,delay) {}; // for software SPI

  // This class's original member functions
  int8_t SetRegulatorVoltage(uint8_t val);
  int8_t SetVolumeResistor(uint8_t val);
  int8_t InvertDisplay(uint8_t val);
  
}; // AQM1248A_extension

// member functions of specific_AQM1248A_SPI

template<class T> void specific_AQM1248A_SPI<T>::Reset(void)
{
  spi.BeginSpi();

  WriteData(MGLCD_COMMAND, 0xae); // Display OFF
  WriteData(MGLCD_COMMAND, 0xa0); // ADC normal
  WriteData(MGLCD_COMMAND, 0xc8); // reverse direction
  WriteData(MGLCD_COMMAND, 0xa3); // 1/7 bias

  WriteData(MGLCD_COMMAND, 0x2c); // power control 1
  delay(2);
  WriteData(MGLCD_COMMAND, 0x2e); // power control 2
  delay(2);
  WriteData(MGLCD_COMMAND, 0x2f); // power control 3
  
  WriteData(MGLCD_COMMAND, 0x23); // Vo voltage resistor ratio set
  WriteData(MGLCD_COMMAND, 0x81); // Electronic volume mode set
  WriteData(MGLCD_COMMAND, 0x1c); // Electronic volume value set

  WriteData(MGLCD_COMMAND, 0xa4); // display all point=normal
  WriteData(MGLCD_COMMAND, 0x40); // display start line=0
  WriteData(MGLCD_COMMAND, 0xa6); // Display nomal/reverse=normal
  WriteData(MGLCD_COMMAND, 0xaf); // Display=on
} // specific_AQM1248A_SPI<T>::Reset(void)

// member_functions of AQM1248A_extension

template<class T> int8_t AQM1248A_extension<T>::SetRegulatorVoltage(uint8_t val)
{
  if(val>7) return -1;
  
  AQM1248A_extension<T>::specific.WriteData(MGLCD_COMMAND, 0x20+val);
  return 0;
} // AQM1248A_extension<T>::SetRegulatorVoltage

template<class T> int8_t AQM1248A_extension<T>::SetVolumeResistor(uint8_t val)
{
  if(val>63) return -1;
  
  AQM1248A_extension<T>::specific.WriteData(MGLCD_COMMAND, 0x81);
  AQM1248A_extension<T>::specific.WriteData(MGLCD_COMMAND, val);
} // AQM1248A_extension<T>::SetVolumeResistor

template<class T> int8_t AQM1248A_extension<T>::InvertDisplay(uint8_t val)
{
  if(val==MGLCD_NON_INVERT || val==MGLCD_INVERT) {
    AQM1248A_extension<T>::specific.WriteData(MGLCD_COMMAND, 0xa6+(val==MGLCD_INVERT));
    return 0;
  } else {
    return -1;
  } // if
} // AQM1248A_extension<T>::InvertDisplay

#endif // #ifndef SPECIFIC_AQM1248A_SPI_H
