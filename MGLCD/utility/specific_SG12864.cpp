/*
  specific_SG12864.cpp
  
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
#include "specific_SG12864.h"

#ifdef ARDUINO_ARCH_AVR

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

// Set one bit of I/O port. Operands must be constants.
static void inline sbi(volatile uint8_t *port, int16_t bit)
{
  __asm__ volatile("sbi %0,%1 \n\t" :: "M"(port-0x20) ,"M"(bit));
} // sbi

// Clear one bit of I/O port. Operands must be constants.
static void inline cbi(volatile uint8_t *port, int16_t bit)
{
  __asm__ volatile("cbi %0,%1\n\t" :: "M"(port-0x20), "M"(bit));
} // cbi

static void inline GeneralWaitForReady(const TPinSet &PinSet)
{
  // Control D/I pin 
  PinSet.A0_DIPin.output(0);

  // Control R/W pin
  PinSet.RWPin.output(1);

//  __asm__ volatile("nop\n\t"); // to meet access time
//  __asm__ volatile("nop\n\t"); // to meet access time

  // Control E pin
  PinSet.EPin.output(1);

  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
//  __asm__ volatile("nop\n\t"); // to meet access time
  
  // Wait for ready
  while(*PinSet.LcdBus.InReg[7]&PinSet.LcdBus.mask[7]);

  // Control E pin
  PinSet.EPin.output(0);
} // GeneralWaitForReady

static void inline StandardWaitForReady(void)
{
  
  // Control D/I pin
  cbi(&STANDARD_A0_DI_PIN_OUT_REG,STANDARD_A0_DI_PIN_BIT);
  
  // Control R/W pin
  sbi(&STANDARD_RW_PIN_OUT_REG,STANDARD_RW_PIN_BIT);
  
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time

  // control E pin
  sbi(&STANDARD_E_PIN_OUT_REG,STANDARD_E_PIN_BIT);
  
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time

  // wait for ready
  while(PIND&0x80);

  // control E pin
  cbi(&STANDARD_E_PIN_OUT_REG,STANDARD_E_PIN_BIT);
} // WaitForReady

static uint8_t GeneralReadData(uint8_t ctrl, const TPinSet &PinSet) 
{
  uint8_t result;
  
  // Control CS1 and CS2 pin
  if(ctrl&MGLCD_CS1) {
    PinSet.CS1_E1Pin.output(1);
    PinSet.CS2_E2Pin.output(0);
  } else {
    PinSet.CS1_E1Pin.output(0);
    PinSet.CS2_E2Pin.output(1);
  } // if

  // Wait for ready
  GeneralWaitForReady(PinSet);
 
  // Control D/I pin 
  if(ctrl&MGLCD_DATA) PinSet.A0_DIPin.output(1);
  
  //__asm__ volatile("nop\n\t"); // to meet access time
  //__asm__ volatile("nop\n\t"); // to meet access time
  
  // Control E pin
  PinSet.EPin.output(1);
 
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
//  __asm__ volatile("nop\n\t"); // to meet access time

  // read data
  result= (*PinSet.LcdBus.InReg[0]&PinSet.LcdBus.mask[0] ? 0x01 : 0)
         +(*PinSet.LcdBus.InReg[1]&PinSet.LcdBus.mask[1] ? 0x02 : 0)
         +(*PinSet.LcdBus.InReg[2]&PinSet.LcdBus.mask[2] ? 0x04 : 0)
         +(*PinSet.LcdBus.InReg[3]&PinSet.LcdBus.mask[3] ? 0x08 : 0)
         +(*PinSet.LcdBus.InReg[4]&PinSet.LcdBus.mask[4] ? 0x10 : 0)
         +(*PinSet.LcdBus.InReg[5]&PinSet.LcdBus.mask[5] ? 0x20 : 0)
         +(*PinSet.LcdBus.InReg[6]&PinSet.LcdBus.mask[6] ? 0x40 : 0)
         +(*PinSet.LcdBus.InReg[7]&PinSet.LcdBus.mask[7] ? 0x80 : 0);
         
  // Control E pin
  PinSet.EPin.output(0);

  return result;  
} // GeneralReadData

static uint8_t inline StandardReadData(uint8_t ctrl, const TPinSet &PinSet)
{
  uint8_t result;

  // Control CS1 and CS2 pin
  if(ctrl&MGLCD_CS1) { // C1
    sbi(&STANDARD_CS1_E1_PIN_OUT_REG,STANDARD_CS1_E1_PIN_BIT);
    cbi(&STANDARD_CS2_E2_PIN_OUT_REG,STANDARD_CS2_E2_PIN_BIT);
  } else { // C2
    cbi(&STANDARD_CS1_E1_PIN_OUT_REG,STANDARD_CS1_E1_PIN_BIT);
    sbi(&STANDARD_CS2_E2_PIN_OUT_REG,STANDARD_CS2_E2_PIN_BIT);
  } // if

  // Wait for ready
  StandardWaitForReady();
    
  // Control D/I pin
  if(ctrl&MGLCD_DATA) sbi(&STANDARD_A0_DI_PIN_OUT_REG,STANDARD_A0_DI_PIN_BIT);

  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time

  // control E pin
  sbi(&STANDARD_E_PIN_OUT_REG,STANDARD_E_PIN_BIT);

  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
    
  // read data
  result=(PINB & 0x0f)|(PIND & 0xf0);
    
  // control E pin
  cbi(&STANDARD_E_PIN_OUT_REG,STANDARD_E_PIN_BIT);

  return result;
} // StandardReadData

static void GeneralWriteData(uint8_t ctrl, uint8_t data, const TPinSet &PinSet)
{
  uint8_t oldSREG;
    
  // Control CS1/CS2 and wait for ready
  if(ctrl&MGLCD_CS1) {
    // wait until CS1 is ready
    PinSet.CS1_E1Pin.output(1);
    PinSet.CS2_E2Pin.output(0);
    GeneralWaitForReady(PinSet);
    
    if(ctrl&MGLCD_CS2) {
      // wait until CS2 is ready
      PinSet.CS1_E1Pin.output(0);
      PinSet.CS2_E2Pin.output(1);
      GeneralWaitForReady(PinSet);
    
      // enable CS1
      PinSet.CS1_E1Pin.output(1);
    } // if
  } else { // CS2 only
    PinSet.CS1_E1Pin.output(0);
    PinSet.CS2_E2Pin.output(1);
    GeneralWaitForReady(PinSet);
  } // if

  // Control D/I pin 
  if(ctrl&MGLCD_DATA) PinSet.A0_DIPin.output(1);

  // Control R/W pin
  PinSet.RWPin.output(0);
  
  // save SREG and disable interrupt    
  oldSREG=SREG;
  cli();

  // control E pin
  *PinSet.EPin.OutReg|= PinSet.EPin.mask;
  
  // set bus output mode
#if PORT_NUM==3
  switch(PinSet.LcdBus.PortNum) {
  case 3:
    *PinSet.LcdBus.NModeReg[2]|=PinSet.LcdBus.RegMask[2];
  case 2:
    *PinSet.LcdBus.NModeReg[1]|=PinSet.LcdBus.RegMask[1];
  case 1:
    *PinSet.LcdBus.NModeReg[0]|=PinSet.LcdBus.RegMask[0];
  } // switch
#else
  for(int16_t i=0; i<PinSet.LcdBus.PortNum; i++) {
    *PinSet.LcdBus.NModeReg[i]|=PinSet.LcdBus.RegMask[i];
  } // for i
#endif

  // send data on bus
  if(data&0x01) {
      *PinSet.LcdBus.OutReg[0]|= PinSet.LcdBus.mask[0];
  } else {
      *PinSet.LcdBus.OutReg[0]&=~PinSet.LcdBus.mask[0];
  } // if
  if(data&0x02) {
      *PinSet.LcdBus.OutReg[1]|= PinSet.LcdBus.mask[1];
  } else {
      *PinSet.LcdBus.OutReg[1]&=~PinSet.LcdBus.mask[1];
  } // if
  if(data&0x04) {
      *PinSet.LcdBus.OutReg[2]|= PinSet.LcdBus.mask[2];
  } else {
      *PinSet.LcdBus.OutReg[2]&=~PinSet.LcdBus.mask[2];
  } // if
  if(data&0x08) {
      *PinSet.LcdBus.OutReg[3]|= PinSet.LcdBus.mask[3];
  } else {
      *PinSet.LcdBus.OutReg[3]&=~PinSet.LcdBus.mask[3];
  } // if
  if(data&0x10) {
      *PinSet.LcdBus.OutReg[4]|= PinSet.LcdBus.mask[4];
  } else {
      *PinSet.LcdBus.OutReg[4]&=~PinSet.LcdBus.mask[4];
  } // if
  if(data&0x20) {
      *PinSet.LcdBus.OutReg[5]|= PinSet.LcdBus.mask[5];
  } else {
      *PinSet.LcdBus.OutReg[5]&=~PinSet.LcdBus.mask[5];
  } // if
  if(data&0x40) {
      *PinSet.LcdBus.OutReg[6]|= PinSet.LcdBus.mask[6];
  } else {
      *PinSet.LcdBus.OutReg[6]&=~PinSet.LcdBus.mask[6];
  } // if
  if(data&0x80) {
      *PinSet.LcdBus.OutReg[7]|= PinSet.LcdBus.mask[7];
  } else {
      *PinSet.LcdBus.OutReg[7]&=~PinSet.LcdBus.mask[7];
  } // if
    
  // control E pin
  *PinSet.EPin.OutReg|= PinSet.EPin.mask;
  
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
//  __asm__ volatile("nop\n\t"); // to meet access time
//  __asm__ volatile("nop\n\t"); // to meet access time

  // control E pin
  *PinSet.EPin.OutReg&=~PinSet.EPin.mask;

  // set bus pullup mode
#if PORT_NUM==3
  switch(PinSet.LcdBus.PortNum) {
  case 3:
    *PinSet.LcdBus.NModeReg[2]&=~PinSet.LcdBus.RegMask[2];
    *PinSet.LcdBus.NOutReg [2]|= PinSet.LcdBus.RegMask[2];
  case 2:
    *PinSet.LcdBus.NModeReg[1]&=~PinSet.LcdBus.RegMask[1];
    *PinSet.LcdBus.NOutReg [1]|= PinSet.LcdBus.RegMask[1];
  case 1:
    *PinSet.LcdBus.NModeReg[0]&=~PinSet.LcdBus.RegMask[0];
    *PinSet.LcdBus.NOutReg [0]|= PinSet.LcdBus.RegMask[0];
  } // switch
#else
  for(int16_t i=0; i<PinSet.LcdBus.PortNum; i++) {
    *PinSet.LcdBus.NModeReg[i]&=~PinSet.LcdBus.RegMask[i];
    *PinSet.LcdBus.NOutReg [i]|= PinSet.LcdBus.RegMask[i];
  } // for i
#endif 

  // restore SREG and enable interrupt
  SREG=oldSREG;   
} // GeneralWriteData

static void inline StandardWriteData(uint8_t ctrl, uint8_t data, const TPinSet &PinSet)
{
  uint8_t oldSREG;
  
  // Control CS1/CS2 and wait for ready
  if(ctrl&MGLCD_CS1) {
    // wait until CS1 is ready
    sbi(&STANDARD_CS1_E1_PIN_OUT_REG,STANDARD_CS1_E1_PIN_BIT);
    cbi(&STANDARD_CS2_E2_PIN_OUT_REG,STANDARD_CS2_E2_PIN_BIT);
    StandardWaitForReady();
    
    if(ctrl&MGLCD_CS2) {
      // wait until CS2 is ready
      cbi(&STANDARD_CS1_E1_PIN_OUT_REG,STANDARD_CS1_E1_PIN_BIT);
      sbi(&STANDARD_CS2_E2_PIN_OUT_REG,STANDARD_CS2_E2_PIN_BIT);
      StandardWaitForReady();
    
      // enable CS1
      sbi(&STANDARD_CS1_E1_PIN_OUT_REG,STANDARD_CS1_E1_PIN_BIT);
    } // if
  } else { // CS2 only
    cbi(&STANDARD_CS1_E1_PIN_OUT_REG,STANDARD_CS1_E1_PIN_BIT);
    sbi(&STANDARD_CS2_E2_PIN_OUT_REG,STANDARD_CS2_E2_PIN_BIT);
    StandardWaitForReady();
  } // if
  
  // Control D/I pin
  if(ctrl&MGLCD_DATA) sbi(&STANDARD_A0_DI_PIN_OUT_REG,STANDARD_A0_DI_PIN_BIT);
  
  // Control R/W pin
  cbi(&STANDARD_RW_PIN_OUT_REG,STANDARD_RW_PIN_BIT);
    
  // save SREG and disable interrupt
  oldSREG=SREG;
  cli();
    
  // set bus output mode
  DDRB|=0x0f;
  DDRD|=0xf0;

  // send data on bus
  PORTB=(PORTB & 0xf0)|(data & 0x0f);
  PORTD=(PORTD & 0x0f)|(data & 0xf0);
  
  // control E pin
  sbi(&STANDARD_E_PIN_OUT_REG,STANDARD_E_PIN_BIT);

  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time
  __asm__ volatile("nop\n\t"); // to meet access time

  // control E pin
  cbi(&STANDARD_E_PIN_OUT_REG,STANDARD_E_PIN_BIT);

  // set bus pullup mode
  DDRB &=~0x0f;
  PORTB|= 0x0f;
  DDRD &=~0xf0;    
  PORTD|= 0xf0;

  // restore SREG and enable interrupt
  SREG=oldSREG; 
} // StandardWriteData

uint8_t (*const specific_SG12864::GeneralReadData)(uint8_t ctrl, const TPinSet &PinSet) = ::GeneralReadData;
uint8_t (*const specific_SG12864::StandardReadData)(uint8_t ctrl, const TPinSet &PinSet) = ::StandardReadData;
void (*const specific_SG12864::GeneralWriteData )(uint8_t ctrl, uint8_t data, const TPinSet &PinSet) = ::GeneralWriteData;
void (*const specific_SG12864::StandardWriteData)(uint8_t ctrl, uint8_t data, const TPinSet &PinSet) = ::StandardWriteData;

void specific_SG12864::Reset(void)
{
  PinSet.A0_DIPin .output(1);
  PinSet.CS1_E1Pin.output(0);
  PinSet.CS2_E2Pin.output(0);
  PinSet.EPin     .output(0);
  PinSet.RWPin    .output(0);
  while(ReadData(MGLCD_CS1_COMMAND,PinSet)&0x90 || ReadData(MGLCD_CS2_COMMAND,PinSet)&0x90); // wait until LCD is ready
  WriteData(MGLCD_CS1_CS2_COMMAND, DisplayOnCommand    ,PinSet);
  WriteData(MGLCD_CS1_CS2_COMMAND, DisplayLineCommand  ,PinSet);
  WriteData(MGLCD_CS1_CS2_COMMAND, PageAddressCommand  ,PinSet);
  WriteData(MGLCD_CS1_CS2_COMMAND, ColumnAddressCommand,PinSet);
} // specific_SG12864::Reset

#endif // #ifdef ARDUINO_ARCH_AVR
