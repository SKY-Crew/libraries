/* 
  HardwareSpiOut.cpp
  
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
#include <SPI.h>
#include <MGLCD.h>
#include <utility/MGLCD_common.h>
#include "HardwareSpiOut.h"

#if MGLCD_VERSION != 42
#error versions of MGLCD and MGLCD_SPI are different.
#endif

HardwareSpiOut::HardwareSpiOut(MGLCD_SpiPin2 pin2, uint32_t speedMaximum) : SpiSetting(speedMaximum,MSBFIRST,SPI_MODE3)
{
  // assign pins
  if(CsPin.AssignPin(pin2.CS)) {
    ConstructionError=-1;
    return;
  } // if
  
  if(DiPin.AssignPin(pin2.DI)) {
    ConstructionError=-2;
    return;
  } // if

  ConstructionError=0;
} // HardwareSpiOut::HardwareSpiOut

void HardwareSpiOut::BeginSpi()
{
  CsPin.SetOutputMode();
  CsPin.output(1);
  DiPin.SetOutputMode();

  SPI.begin();
} // BeginSpi
