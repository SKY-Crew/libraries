/* 
  SoftWareSpiOut.h
  
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
#ifndef SOFTWARESPIOUT_H
#define SOFTWARESPIOUT_H

#include <arduino.h>
#include <MGLCD.h>
#include <utility/MGLCD_common.h>

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

class SoftwareSpiOut
{
private:
  int16_t ConstructionError;

  inline void WriteOneBitWithDelay(uint8_t data);
  inline void WriteOneBit(uint8_t data);

public:
  PortBit SckPin;
  PortBit MosiPin;
  PortBit CsPin;
  PortBit DiPin;
  uint16_t DelayCnt;

  explicit SoftwareSpiOut(MGLCD_SpiPin4 pin4, uint16_t delay);
  inline void WriteData(uint8_t ctrl, uint8_t data);
  int16_t GetConstructionError(void) const { return ConstructionError; }
  void BeginSpi();
  inline void DelayLoop() const { for(volatile uint16_t v=0; v<DelayCnt; v++); };
}; // SoftwareSpiOut

inline void SoftwareSpiOut::WriteOneBitWithDelay(uint8_t data)
{
#if defined(ARDUINO_ARCH_AVR)
  cli();
  if(data) {
    *MosiPin.OutReg |=  MosiPin.mask;
  } else {
    *MosiPin.OutReg &= ~MosiPin.mask;
  } // if
  sei();

  cli();
  *(SckPin.OutReg) &= ~SckPin.mask;
  sei();
  DelayLoop();

  cli();
  *(SckPin.OutReg) |=  SckPin.mask;
  sei();
  DelayLoop();
#else
  MosiPin.output(data!=0);
  SckPin.output(0);
  DelayLoop();
  SckPin.output(1);
  DelayLoop();
#endif
} // SoftwareSpiOut::WriteOneBitWithDelay

inline void SoftwareSpiOut::WriteOneBit(uint8_t data)
{
#if defined(ARDUINO_ARCH_AVR)
  cli();
  if(data) {
    *MosiPin.OutReg |=  MosiPin.mask;
  } else {
    *MosiPin.OutReg &= ~MosiPin.mask;
  } // if
  *(SckPin.OutReg) &= ~SckPin.mask;
  *(SckPin.OutReg) |=  SckPin.mask;
  sei();
#else
  MosiPin.output(data!=0);
  SckPin.output(0);
  SckPin.output(1);
#endif
} // SoftwareSpiOut::WriteOneBit

inline void SoftwareSpiOut::WriteData(uint8_t ctrl, uint8_t data) 
{
  if(DelayCnt) {
    CsPin.output(0);
    DiPin.output(ctrl);
    DelayLoop();

    WriteOneBitWithDelay(data & 0x80);
    WriteOneBitWithDelay(data & 0x40);
    WriteOneBitWithDelay(data & 0x20);
    WriteOneBitWithDelay(data & 0x10);
    WriteOneBitWithDelay(data & 0x08);
    WriteOneBitWithDelay(data & 0x04);
    WriteOneBitWithDelay(data & 0x02);
    WriteOneBitWithDelay(data & 0x01);

    CsPin.output(1);
    DelayLoop();
  } else {
#if defined(ARDUINO_ARCH_AVR)
    cli();
    *(CsPin.OutReg) &= ~CsPin.mask;
    if(ctrl) {
      *DiPin.OutReg |=  DiPin.mask;
    } else {
      *DiPin.OutReg &= ~DiPin.mask;
    } // if
    sei();
#else
    CsPin.output(0);
    DiPin.output(ctrl);
#endif

    WriteOneBit(data & 0x80);
    WriteOneBit(data & 0x40);
    WriteOneBit(data & 0x20);
    WriteOneBit(data & 0x10);
    WriteOneBit(data & 0x08);
    WriteOneBit(data & 0x04);
    WriteOneBit(data & 0x02);
    WriteOneBit(data & 0x01);

#if defined(ARDUINO_ARCH_AVR)
    cli();
    *CsPin.OutReg |=  CsPin.mask;
    sei();
#else
    CsPin.output(1);
#endif
  } // if
} // SoftwareSpiOut::WriteData

#endif // ifndef SOFTWARESPIOUT_H