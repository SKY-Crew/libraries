/*
  MGLCD_common.cpp
  
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
#include <avr/pgmspace.h>
#include "MGLCD.h"

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

int16_t PortBit::AssignPin(uint8_t pin)
{
#if defined(ARDUINO_ARCH_AVR)
  uint8_t port;

  if(pin>=MGLCD_NUM_DIGITAL_PINS) return -1;
  port=digitalPinToPort(pin);
  if(port==NOT_A_PIN) return -2;
  
  mask   =digitalPinToBitMask(pin);
  ModeReg=portModeRegister(port);
  OutReg =portOutputRegister(port);
  InReg  =portInputRegister(port);
  return 0;
#elif defined(ARDUINO_ARCH_SAM)
  if(g_APinDescription[pin].ulPinType==PIO_NOT_A_PIN) return -1;
  mask=g_APinDescription[pin].ulPin;
  port=g_APinDescription[pin].pPort;
  PinNo=pin;
  return 0;
#elif defined(ARDUINO_ARCH_SAMD)
  if(g_APinDescription[pin].ulPinType == PIO_NOT_A_PIN) return -1;
  {
    PortGroup *port=&(PORT->Group[g_APinDescription[pin].ulPort]);
    uint8_t bit=g_APinDescription[pin].ulPin;
    
    PINCFG=&(port->PINCFG[bit].reg);
    OUTSET=&(port->OUTSET.reg);
    OUTCLR=&(port->OUTCLR.reg);
    mask=1ul<<bit;
    PinNo=pin;
  }
  return 0;
#else
  PinNo=pin;
  return 0;
#endif
} // PortBit::AssignPin

void PortBit::SetOutputMode() const
{
#if defined(ARDUINO_ARCH_AVR)
  uint8_t oldSREG;
    
  oldSREG=SREG;
  cli();
  *ModeReg|=mask;
  SREG=oldSREG;
#else
  pinMode(PinNo,OUTPUT);
#endif
} // PortBit::SetOutputMode
