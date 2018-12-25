/* 
  HardwareSpiOut.h
  
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

#ifndef HARDWARESPIOUT_H
#define HARDWARESPIOUT_H

#include <arduino.h>
#include <SPI.h>
#include <MGLCD.h>
#include <utility/MGLCD_common.h>
#include <utility/MGLCD_buffered.h>

#if MGLCD_VERSION != 42
#error versions of MGLCD and MGLCD_SPI are different.
#endif

class HardwareSpiOut
{
private:
  int16_t ConstructionError;
public:
  SPISettings SpiSetting;
  PortBit CsPin;
  PortBit DiPin;

  explicit HardwareSpiOut(MGLCD_SpiPin2 pin2, uint32_t speedMaximum);
  inline void WriteData(uint8_t ctrl, uint8_t data); __attribute__((always_inline))
  int16_t GetConstructionError(void) const { return ConstructionError; }
  void BeginSpi();
}; // HardwareSpiOut

inline void HardwareSpiOut::WriteData(uint8_t ctrl, uint8_t data) 
{
  SPI.beginTransaction(SpiSetting);
  
#if defined(ARDUINO_ARCH_AVR)
  uint8_t oldSREG = SREG;
  cli();
  *CsPin.OutReg &= ~CsPin.mask;
  SREG=oldSREG;
  cli();
  if(ctrl) {
    *DiPin.OutReg |=  DiPin.mask;
  } else {
    *DiPin.OutReg &= ~DiPin.mask;
  } // if
  SREG=oldSREG;
#else
  CsPin.output(0);
  DiPin.output(ctrl);
#endif
  
  SPI.transfer(data);
  CsPin.output(1);
  
  SPI.endTransaction();
} // HardwareSpiOut::WriteData

#endif // #ifndef HARDWARESPIOUT_H
