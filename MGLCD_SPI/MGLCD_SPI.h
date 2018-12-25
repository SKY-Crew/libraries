/* 
  mgcld_SPI.h
  
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

#ifndef MGLCD_SPI_H
#define MGLCD_SPI_H

#if !defined(ARDUINO) || ARDUINO<150
#error "MGLCD_SPI must be used on Arduino IDE 1.5.0 or later."
#endif

#include <mglcd.h>
#include <utility/MGLCD_buffered.h>
#include <utility/specific_AQM1248A_SPI.h>
#include <utility/specific_GH12864_20_SPI.h>
#include "utility/HardwareSpiOut.h"

#if MGLCD_VERSION != 42
#error versions of MGLCD and MGLCD_SPI are different.
#endif

typedef AQM1248A_extension<specific_AQM1248A_SPI<HardwareSpiOut> > MGLCD_AQM1248A_SPI;
typedef GH12864_20_extension<specific_GH12864_20_SPI<HardwareSpiOut> > MGLCD_GH12864_20_SPI;

#endif // #ifndef MGLCD_H
