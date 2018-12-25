/* 
  SoftwareSpiOut.cpp
  
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
#include <utility/MGLCD_common.h>
#include "SoftwareSpiOut.h"

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

SoftwareSpiOut::SoftwareSpiOut(MGLCD_SpiPin4 pin4, uint16_t delay) : DelayCnt(delay)
{
  // assign pins
  if(SckPin.AssignPin(pin4.SCK)) {
    ConstructionError=-1;
    return;
  } // if
  
  if(MosiPin.AssignPin(pin4.MOSI)) {
    ConstructionError=-2;
    return;
  } // if
  
  if(CsPin.AssignPin(pin4.CS)) {
    ConstructionError=-3;
    return;
  } // if
  
  if(DiPin.AssignPin(pin4.DI)) {
    ConstructionError=-4;
    return;
  } // if

  ConstructionError=0;
} // SoftwareSpiOut::HardwareSpiOut

void SoftwareSpiOut::BeginSpi()
{
  CsPin  .SetOutputMode();
  CsPin  .output(1);
  SckPin .SetOutputMode();
  SckPin .output(1);
  MosiPin.SetOutputMode();
  DiPin  .SetOutputMode();
} // SoftwareSpiOut::BeginSpi
