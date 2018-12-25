/* 
  MGLCD_common.h
  
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

#ifndef MGLCD_COMMON_H
#define MGLCD_COMMON_H

#include <arduino.h>
#include <MGLCD.h>
#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

class PortBit{
public:
#if defined(ARDUINO_ARCH_AVR)
  uint8_t mask;
  volatile uint8_t *ModeReg;
  volatile uint8_t *OutReg;
  volatile uint8_t *InReg;
#elif defined(ARDUINO_ARCH_SAM)
  uint32_t mask;
  Pio *port;
  uint8_t PinNo;
#elif defined(ARDUINO_ARCH_SAMD)
  uint8_t PinNo;
  volatile uint8_t *PINCFG;
  volatile uint32_t *OUTSET;
  volatile uint32_t *OUTCLR;
  uint32_t mask;
#else
  uint8_t PinNo;
#endif
  int16_t AssignPin(uint8_t pin);
  void SetOutputMode() const;
  void output(uint8_t val) const;
};

inline void PortBit::output(uint8_t val) const
{
#if defined(ARDUINO_ARCH_AVR)
  cli();
  if(val) {
    *OutReg |= mask ;
  } else {
    *OutReg &= ~mask;
  } // if
  sei();
#elif defined(ARDUINO_ARCH_SAM)
  if(val) {
    port->PIO_SODR=mask;
  } else {
    port->PIO_CODR=mask;
  } // if
#elif defined(ARDUINO_ARCH_SAMD)
  *PINCFG=(uint8_t)(PORT_PINCFG_PULLEN);
  *(val ? OUTSET : OUTCLR)=mask;
#else
  digitalWrite(PinNo,(val ? HIGH : LOW));
#endif
} // PortBit::output

#endif // #ifndef MGLCD_COMMON_H
