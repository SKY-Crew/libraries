/*
  MGLCD_direct.cpp
  
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

#include <arduino.h>
#include <MGLCD.h>
#include "MGLCD_direct.h"

#if defined(ARDUINO_ARCH_AVR)

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

void SetBusInputPullupMode(TBus const &LcdBus)
{
    uint8_t oldSREG;
    int i;
    
    oldSREG=SREG;
    cli();
    for(i=0; i<LcdBus.PortNum; i++) {
      *LcdBus.NModeReg[i]&=~LcdBus.RegMask[i];
      *LcdBus.NOutReg [i]|= LcdBus.RegMask[i];
    } // for i
    SREG=oldSREG;    
} // SetBusInputPullupMode

int16_t AssignBus(TBus &b, const uint8_t db[], boolean &StandardConnection)
{
  uint8_t port;
  uint8_t i,j;
  uint8_t mask;
  
  b.PortNum=0;
  
  i=PORT_NUM;
  while(i--) b.NModeReg[i]=NULL;

  for(i=0; i<BUS_WIDTH; i++) {  

    if(db[i]>=MGLCD_NUM_DIGITAL_PINS) return -1;
    port=digitalPinToPort(db[i]);
    if(port==NOT_A_PIN) return -2;

    b.ModeReg[i]=portModeRegister(port);
    b.OutReg [i]=portOutputRegister(port);
    b.InReg  [i]=portInputRegister(port);
    j=0;
    while(j<b.PortNum && b.NModeReg[j]!=b.ModeReg[i]) j++;
    if(j>=PORT_NUM) return -2;
    if(j==b.PortNum) {
      b.NModeReg[j]=b.ModeReg[i];
      b.NOutReg [j]=b.OutReg [i];
      b.RegMask[j]=0;
      b.PortNum++;
    } // if
    b.mask[i]=digitalPinToBitMask(db[i]);
    b.RegMask[j]|=b.mask[i];
  } // for i

  StandardConnection=true;
  mask=1;
  for(i=0; i<BUS_WIDTH; i++) {
    if(b.OutReg[i]!=(i<4 ? &PORTB : &PORTD) || b.mask[i]!=mask) StandardConnection=false;
    mask<<=1;
  } // for i
  
  return 0;
} // AssignBus

#endif
