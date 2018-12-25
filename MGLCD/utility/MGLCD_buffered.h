/* 
  MGLCD_buffered.h
  
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

#ifndef MGLCD_BUFFERED_H
#define MGLCD_BUFFERED_H

#include <arduino.h>
#include <Print.h>
#include <utility/MGLCD_font.h>
#include <utility/MGLCD_common.h>

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

// Constants for WriteData
#define MGLCD_COMMAND 0
#define MGLCD_DATA    1

class MGLCD_SpiPin2
{
public:
  uint8_t CS,DI;
  
  MGLCD_SpiPin2(uint8_t CsPin, uint8_t DiPin) : CS(CsPin), DI(DiPin) {};
}; // MGLCD_SpiPin2

class MGLCD_SpiPin4
{
public:
  uint8_t SCK,MOSI,CS,DI;
  
  MGLCD_SpiPin4(uint8_t SckPin, uint8_t MosiPin, uint8_t CsPin, uint8_t DiPin) 
    : SCK(SckPin), MOSI(MosiPin), CS(CsPin), DI(DiPin) {};
}; // MGLCD_SpiPin4

template<class T> class MGLCD_buffered : public Print
{
private:
  // This class's original member variables
  uint8_t TextX,TextY;
  bool NewLine;
  uint8_t UserCharNum;
  boolean UserCharMemoryType;
  const uint8_t (*UserCharPtr)[5];
  PDecodeFunc DecodeChar;
  
  // Member variables compatible with the other classes
  uint8_t WindowX1,WindowY1,WindowX2,WindowY2; 
  uint8_t InvertMode;
  uint8_t ScrollMode;
  uint8_t LcdPage;
  uint8_t LcdColumn;
  uint8_t ImageBuf[T::PageNum][T::Width];

  // This class's original member functions
  void scroll(void);

protected:
  // This class's original member variables
  T specific;

public:
  // This class's original member variables
  static const uint8_t DisplayOnCommand          = T::DisplayOnCommand         ;
  static const uint8_t DisplayOffCommand         = T::DisplayOffCommand        ;
  static const uint8_t PageAddressCommand        = T::PageAddressCommand       ;
  static const uint8_t UpperColumnAddressCommand = T::UpperColumnAddressCommand;
  static const uint8_t LowerColumnAddressCommand = T::LowerColumnAddressCommand;

  // constructor
  explicit MGLCD_buffered(MGLCD_SpiPin2 pin2, uint32_t speedMaximum); // for hardware SPI
  explicit MGLCD_buffered(MGLCD_SpiPin4 pin4, uint16_t delay); // for software SPI

  // This class's original member functions
  void ColumnAddress(uint8_t col);
  void PageAddress(uint8_t page);
  void WriteByteData(uint8_t data);
  uint8_t ReadByteData(uint8_t page, uint8_t col) { return ImageBuf[page][col]; }
  void DisplayChar(uint8_t ch);
   
  // Member functions compatible with other classes
  int16_t GetConstructionError(void) { return specific.GetConstructionError; }
  int8_t Reset(uint8_t invert=MGLCD_NON_INVERT, uint8_t scroll=MGLCD_SCROLL);
  int8_t ClearScreen(void);
  uint8_t GetWidth(void) { return T::Width; }
  uint8_t GetHeight(void) { return T::Height; }
  uint8_t GetPageNum(void) { return T::PageNum; }
  uint8_t GetLineNum(void) { return T::PageNum; }
  uint8_t GetColumnNum(void) { return T::ColumnNum; }  
  int8_t SetTextWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
  void GetTextWindow(uint8_t &x1, uint8_t &y1, uint8_t &x2, uint8_t &y2) { x1=WindowX1; y1=WindowY1; x2=WindowX2; y2=WindowY2; }
  int8_t SetInvertMode(uint8_t mode);
  uint8_t GetInvertMode(void) { return InvertMode; }
  int8_t SetScrollMode(uint8_t mode);
  uint8_t GetScrollMode(void) { return ScrollMode; }
  int8_t SetCodeMode(PDecodeFunc CodeMode);
  PDecodeFunc GetCodeMode(void) { return DecodeChar; }
  int8_t Locate(uint8_t x, uint8_t y);
  int16_t GetX(void) { return TextX; }
  int16_t GetY(void) { return TextY; }
  int8_t ClearRestOfLine(void);
  int8_t PutChar(uint8_t ch);
  int8_t UserChars(const uint8_t uc[][5], uint8_t num, uint8_t mem=MGLCD_ROM);
  int8_t SetPixel(uint8_t x, uint8_t y, uint8_t color=1);
  int8_t GetPixel(uint8_t x, uint8_t y);
  int8_t Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color=1);
  int8_t Rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color=1);
  int8_t FillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color=1);
  int8_t RoundRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t color=1);  
  int8_t Circle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color=1);
  int8_t FillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color=1);
  int8_t DrawBitmap(const uint8_t bmp[], uint8_t x, uint8_t y, uint8_t mem=MGLCD_ROM);
  virtual size_t write(uint8_t); // inherit Print
}; // MGLCD_buffered

template<class T> MGLCD_buffered<T>::MGLCD_buffered(MGLCD_SpiPin2 pin2, uint32_t speedMaximum)
  : UserCharNum(0), DecodeChar(MGLCD_CODE_STRAIGHT), specific(pin2, speedMaximum)
{
} // MGLCD_buffered<T>::MGLCD_buffered

template<class T> MGLCD_buffered<T>::MGLCD_buffered(MGLCD_SpiPin4 pin4, uint16_t delay)
  : UserCharNum(0), DecodeChar(MGLCD_CODE_STRAIGHT), specific(pin4, delay)
{
} // MGLCD_buffered<T>::MGLCD_buffered

template<class T> void MGLCD_buffered<T>::ColumnAddress(uint8_t col)
{
  uint8_t OffsetCol      =col      +T::ColumnOffset;
  uint8_t OffsetLcdColumn=LcdColumn+T::ColumnOffset;
  
  if((OffsetCol^OffsetLcdColumn)&0xf0) specific.WriteData(MGLCD_COMMAND, UpperColumnAddressCommand+(OffsetCol>>4  ));
  if((OffsetCol^OffsetLcdColumn)&0x0f) specific.WriteData(MGLCD_COMMAND, LowerColumnAddressCommand+(OffsetCol&0x0f));
  LcdColumn=col;
} // MGLCD_buffered<T>::ColumnAddress

template<class T> void MGLCD_buffered<T>::PageAddress(uint8_t page)
{
  if(page==LcdPage) return;
  
  specific.WriteData(MGLCD_COMMAND, PageAddressCommand+page);
  LcdPage=page;
} // MGLCD_buffered<T>::PageAddress

template<class T> void MGLCD_buffered<T>::WriteByteData(uint8_t data)
{
  specific.WriteData(MGLCD_DATA, data);
  ImageBuf[LcdPage][LcdColumn]=data;
  if(LcdColumn<=T::Width) LcdColumn++;
} // MGLCD_buffered<T>::WriteByteData

template<class T> int8_t MGLCD_buffered<T>::Reset(uint8_t invert, uint8_t scroll)
{
  specific.Reset();
  if(SetTextWindow(0,0,T::ColumnNum-1,T::PageNum-1)!=0) return -1;
  if(SetInvertMode(invert)!=0) return -2;
  if(SetScrollMode(scroll)!=0) return -3;

  specific.WriteData(MGLCD_COMMAND, PageAddressCommand);
  specific.WriteData(MGLCD_COMMAND, UpperColumnAddressCommand+(T::ColumnOffset>>4  ));
  specific.WriteData(MGLCD_COMMAND, LowerColumnAddressCommand+(T::ColumnOffset&0x0f));  
  LcdPage=LcdColumn=0;
  
  ClearScreen();

  return 0;
} // MGLCD_buffered<T>::Reset

template<class T> void MGLCD_buffered<T>::DisplayChar(uint8_t ch)
{
  uint8_t col;
  uint8_t data;

  col=(WindowX1+TextX)*MGLCD_FONT_WIDTH;
  PageAddress(WindowY1+TextY);
  ColumnAddress(col);
  for(uint8_t i=0; i<MGLCD_FONT_WIDTH; i++) {
    if(i<MGLCD_FONT_WIDTH-1) {
      if(ch<=0x7f) { // ASCII code
        data=pgm_read_byte_near(&FontBmp1[ch-0x20][i]);
      } else if(ch<=0x9f) { // user character
        data=UserCharMemoryType==MGLCD_ROM ? pgm_read_byte_near(&UserCharPtr[ch-0x80][i]) : UserCharPtr[ch-0x80][i];
      } else { // kana code
        data=pgm_read_byte_near(&FontBmp2[ch-0xa0][i]);
      } // if
    } else {
      data=0;
    } // if
        
    if(InvertMode==MGLCD_INVERT) data=~data;
    WriteByteData(data);
  } // for i
} // MGLCD_buffered<T>::DisplayChar

template<class T> int8_t MGLCD_buffered<T>::ClearScreen(void)
{
  uint8_t i,j;
  uint8_t data;

  data=InvertMode==MGLCD_INVERT ? 0xff : 0;
  i=T::PageNum;
  while(i--) {
    PageAddress(i);
    ColumnAddress(0);
    j=T::Width;
    while(j--) WriteByteData(data);
  } // while
  TextX=TextY=0;
  NewLine=false;
  
  return 0;
} // MGLCD_buffered<T>::ClearScreen

template<class T> int8_t MGLCD_buffered<T>::SetTextWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  if(x1>x2 || y1>y2 || x2>=T::ColumnNum || y2>=T::PageNum) return -1;
  
  WindowX1=x1; WindowY1=y1;
  WindowX2=x2; WindowY2=y2;
  
  TextX=TextY=0;  
  NewLine=true;
  
  return 0;
} // MGLCD_buffered<T>::SetTextWindow

template<class T> int8_t MGLCD_buffered<T>::SetInvertMode(uint8_t mode)
{
  if(mode!=MGLCD_NON_INVERT && mode!=MGLCD_INVERT) return -1;
  InvertMode=mode;
  
  return 0;
} // MGLCD_buffered<T>::SetInvertMode

template<class T> int8_t MGLCD_buffered<T>::SetScrollMode(uint8_t mode)
{
  if(mode!=MGLCD_NON_SCROLL && mode!=MGLCD_SCROLL) return -1;
  ScrollMode=mode;
  if(mode==MGLCD_NON_SCROLL && TextY>WindowY2-WindowY1) {
    TextY=0;
    NewLine=true;
  } // if
  
  return 0;
} // MGLCD_buffered<T>::SetScrollMode

template<class T> int8_t MGLCD_buffered<T>::SetCodeMode(PDecodeFunc CodeMode)
{ 
  if(CodeMode==NULL) return -1;
  DecodeChar=CodeMode; 
  return 0;
} // MGLCD_buffered<T>::SetCodeMode

template<class T> int8_t MGLCD_buffered<T>::Locate(uint8_t x, uint8_t y)
{
  if(x>T::ColumnNum || y>=T::PageNum || (x==T::ColumnNum && y!=0)) return -1;
  TextX=x;
  TextY=y;
  NewLine=false;
  
  return 0;
} // MGLCD_buffered<T>::Locate

template<class T> void MGLCD_buffered<T>::scroll(void)
{
  uint8_t xstart, xend;
  uint8_t i, j;
        
  xstart=WindowX1*MGLCD_FONT_WIDTH; 
  xend  =(WindowX2+1)*MGLCD_FONT_WIDTH-1;
  for(i=WindowY1; i<WindowY2; i++) {
    PageAddress(i);
    ColumnAddress(xstart);
    for(j=xstart; j<=xend; j++) {
      WriteByteData(ReadByteData(i+1,j));
    } // for j;
  } // for i
} // MGLCD_buffered<T>::scroll

template<class T> int8_t MGLCD_buffered<T>::ClearRestOfLine(void)
{
  int16_t i;
  uint8_t data;
  
  if(TextY>WindowY2-WindowY1) return 0;
  PageAddress(WindowY1+TextY);
  data=InvertMode==MGLCD_INVERT ? 0xff : 0;
  i=(WindowX1+TextX)*MGLCD_FONT_WIDTH;
  ColumnAddress(i);
  do {
    WriteByteData(data);
  } while(++i<(WindowX2+1)*MGLCD_FONT_WIDTH);
  
  return 0;
} // MGLCD_buffered<T>::ClearRestOfLine

template<class T> int8_t MGLCD_buffered<T>::PutChar(uint8_t ch)
{
  uint8_t WindowHeight;

  ch=DecodeChar(ch);
  if(ch==0) return 0;

  WindowHeight=WindowY2-WindowY1;

  if(ch=='\r') { // ignore '\r'
    return 0; 
  } else if(ch=='\n') { // line feed
    if(ScrollMode==MGLCD_SCROLL) { // scroll mode
      TextX=0;
      if(TextY>WindowHeight) {
        TextY=WindowHeight;
        scroll();
        ClearRestOfLine();
      } // if
      TextY++;
    } else { // non scroll mode
      if(NewLine) ClearRestOfLine();
      TextX=0;
      if(++TextY>WindowHeight) TextY=0;
      NewLine=true;
    } // if
    return 0;
  } else if(ch<' ' || (ch>=0x80+UserCharNum && ch<=0x9f)|| ch>0xdf) { // replace any unused code with space
    ch=' '; 
  }  // if

  // if in scroll mode and TextY>WindowHeight, scroll one line
  if(ScrollMode==MGLCD_SCROLL) {
    if(TextY>WindowHeight) {
      TextY=WindowHeight;
      scroll();
      NewLine=true;
    } else { 
      NewLine=false;
    } // if
  } // if 
  
  DisplayChar(ch);
  
  TextX++; // move cursor
  if(NewLine) {
    ClearRestOfLine(); // if in new line, clear rest of bottom line
    NewLine=false;
  } // if
  
  // adjust cursor
  if(TextX>WindowX2-WindowX1) {
    TextX=0;
    TextY++;
    if(ScrollMode==MGLCD_NON_SCROLL) {
      if(TextY>WindowY2-WindowY1) TextY=0;
      NewLine=true;
    } else {
      NewLine=false;
    } // if
  } // if
  
  return 0;
} // MGLCD_buffered<T>::PutChar

template<class T> int8_t MGLCD_buffered<T>::UserChars(const uint8_t uc[][5], uint8_t num, uint8_t mem)
{
  if(num==0 || num>32 || uc==NULL || (mem!=MGLCD_ROM && mem!=MGLCD_RAM)) return -1;
  
  UserCharNum=num;
  UserCharPtr=uc;
  UserCharMemoryType=mem;

  return 0;
} // MGLCD_buffered<T>::UserChars

template<class T> int8_t MGLCD_buffered<T>::SetPixel(uint8_t x, uint8_t y, uint8_t color)
{
  uint8_t mask;
  uint8_t data;
  
  if(x>=T::Width || y>=T::Height || color>1) return -1;
  
  if(InvertMode==MGLCD_INVERT) color=1-color;

  mask=1<<(y&7);
  
  data=ReadByteData(y>>3,x)&~mask;
  if(color) data|=mask;

  PageAddress(y>>3);
  ColumnAddress(x);
  WriteByteData(data);  
  
  return 0;
} // MGLCD_buffered<T>::PutPixel

template<class T> int8_t MGLCD_buffered<T>::GetPixel(uint8_t x, uint8_t y)
{
  if(x>=T::Width || y>=T::Height) return -1;

  return ((ReadByteData(y>>3,x)>>(y&7)) & 1) ^ (InvertMode==MGLCD_INVERT);
} // MGLCD_buffered<T>::GetPixel

template<class T> int8_t MGLCD_buffered<T>::Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
  boolean swap;
  int16_t dy;
  int16_t sum;
  uint8_t x,y;
  
  if(   x1>=T::Width || y1>=T::Height
     || x2>=T::Width || y2>=T::Height
     || color>1) return -1;

  if(InvertMode==MGLCD_INVERT) color=1-color;
  
  swap=abs(y2-y1)>abs(x2-x1);
  if(swap) {
    uint8_t temp;
    temp=x1; x1=y1; y1=temp;
    temp=x2; x2=y2; y2=temp;
  } // if
  if(x1>x2) {
    uint8_t temp;
    temp=x1; x1=x2; x2=temp;
    temp=y1; y1=y2; y2=temp;
  } // if
  
  dy=y2-y1;
  sum=(x2-x1)>>1;
  if(dy<0) sum=-sum;
  y=y1;
  if(swap) { // within +-45 deg. from vertical line 
    uint8_t mask;
    uint8_t page,OldPage;
    uint8_t data;
    uint8_t OldY;
    
    page=OldPage=x1>>3;
    PageAddress(page);
    mask=1<<(x1&7); 
    data=0;
    for(x=x1; x<=x2; x++) { 
      data|=mask;
      OldY=y;

      sum+=dy;
      if(sum>=x2-x1) {
        y++;
        sum-=x2-x1;
      } else if(sum<=x1-x2) {
        y--;
        sum+=x2-x1;
      } // if

      if(mask<0x80) {
        mask<<=1;
      } else {
        mask=1;
        page++;
      } // if
      
      if(y!=OldY || page!=OldPage || x==x2) {
        uint8_t b;

        b=ReadByteData(OldPage,OldY);
        data=color ? data|b : ~data&b;
        ColumnAddress(OldY);
        WriteByteData(data);
        data=0;
        if(page!=OldPage && x<x2) {
          PageAddress(page);
          OldPage=page;
        } // if
      } // if
    } // for x
  } else { // within +-45 deg. from horizontal line
    uint8_t mask;
    uint8_t page;
    boolean PageChanged;

    mask=1<<(y&7); 
    page=y>>3;
    PageAddress(page);
    ColumnAddress(x1);
    PageChanged=false;
    for(x=x1; x<=x2; x++) {
      if(PageChanged) {
        PageAddress(page);
        PageChanged=false;
      } // if
      
      uint8_t data=ReadByteData(page,x);
      if(color) {
        data= mask|data;
      } else {
        data=~mask&data;
      } // if
      WriteByteData(data);
      
      sum+=dy;
      if(sum>=x2-x1) {
        sum-=x2-x1;
        if(++y&7) {
          mask<<=1;
        } else { 
          mask=1;
          page++;
          PageChanged=true;
        } // if 
      } else if(sum<=x1-x2) {
        if(y--&7) {
          mask>>=1;
        } else {
          mask=0x80;
          page--; 
          PageChanged=true;
        } // if
        sum+=x2-x1;
      } // if
    } // for x
  } //if
    
  return 0;
} // MGLCD_buffered<T>::Line

template<class T> int8_t MGLCD_buffered<T>::Rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
  int8_t result;
  
  result=0;
  if(Line(x1,y1,x2,y1,color)) result=-1;
  if(Line(x2,y1,x2,y2,color)) result=-2;
  if(Line(x2,y2,x1,y2,color)) result=-3;
  if(Line(x1,y2,x1,y1,color)) result=-4;
  return result;
} // MGLCD_buffered<T>::Rect

template<class T> int8_t MGLCD_buffered<T>::FillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
  uint8_t page,EndPage,PageY;
  uint8_t data,mask;
  uint8_t i;
  
  // error check
  if(   x1>=T::Width || y1>=T::Height
     || x2>=T::Width || y2>=T::Height
     || color>1) return -1;

  // swap x1 and x2 if x1>x2
  if(x1>x2) {
    unsigned temp;
    temp=x1; x1=x2; x2=temp;
  } // if

  // swap y1 and y2 if y1>y2  
  if(y1>y2) {
    unsigned temp;
    temp=y1; y1=y2; y2=temp;
  }
  
  // fill rectangle
  EndPage=y2>>3;
  PageY=y1&0xf8;
  for(page=y1>>3; page<=EndPage; page++) {
    // get byte mask
    {
      uint8_t m;
      
      mask=0;
      m=0x80;
      i=8;
      while(i--) {
        if(i>=y1-PageY) {
            if(i<=y2-PageY) {
              mask|=m;
            } // if
        } else {
          break;
        } // if
        
        m>>=1;
      } // while
    }
    
    // write data for 1 page
    PageAddress(page);
    ColumnAddress(x1);
        
    for(i=x1; i<=x2; i++) {
      // send 1 byte
      data=ReadByteData(page,i);
      if(color) {
        data|= mask;
      } else {
        data&=~mask;
      } // if
      WriteByteData(data);
    } // for i
    
    PageY+=8;
  } // for page
  
  return 0;
} // MGLCD_buffered<T>::FillRect

template<class T> int8_t MGLCD_buffered<T>::RoundRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t color)
{
  int16_t x;
  int16_t y;
  int16_t err;
  int16_t old_err;
  
  if(x1>x2) {
    uint8_t temp;
    temp=x1; x1=x2; x2=temp;
  } // if
  
  if(y1>y2) {
    uint8_t temp;
    temp=y1; y1=y2; y2=temp;
  } // if
  
  if(x1>=T::Width || y2>=T::Height || 2*r>x2-x1 || 2*r>y2-y1 || color>1) return -1;
  
  x=0;
  y=-r;
  err=2-2*r;

  do{
    if(x) {
      SetPixel(x1+r-x,y1+r+y,color); 
      SetPixel(x2-r+x,y1+r+y,color); 
      SetPixel(x1+r-x,y2-r-y,color); 
      SetPixel(x2-r+x,y2-r-y,color);
    } // if 
    if ((old_err=err)<=x)   err+=++x*2+1;
    if (old_err>y || err>x) err+=++y*2+1;    
  } while(y<0);
   
  Line(x1+r,y1  ,x2-r,y1  ,color);
  Line(x1+r,y2  ,x2-r,y2  ,color);
  Line(x1  ,y1+r,x1  ,y2-r,color);
  Line(x2  ,y1+r,x2  ,y2-r,color);  
  return 0;
} // MGLCD_buffered<T>::RoundRect
  
// This circle routine's algorithm is based on http://free.pages.at/easyfilter/bresenham.html .
template<class T> int8_t MGLCD_buffered<T>::Circle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
{
  int16_t x;
  int16_t y;
  int16_t err;
  int16_t old_err;
  
  if(x0-r<0 || x0+r>=T::Width || y0-r<0 || y0+r>T::Height || color>1) return -1;

  x=0;
  y=-r;
  err=2-2*r;
  do{
    SetPixel(x0-x,y0+y,color); 
    SetPixel(x0-y,y0-x,color); 
    SetPixel(x0+x,y0-y,color); 
    SetPixel(x0+y,y0+x,color); 
    if ((old_err=err)<=x)   err+=++x*2+1;
    if (old_err>y || err>x) err+=++y*2+1;    
  } while(y<0);
  
  return 0;
} // MGLCD_buffered<T>::Circle

// This circle routine's algorithm is based on http://free.pages.at/easyfilter/bresenham.html .
template<class T> int8_t MGLCD_buffered<T>::FillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
{
  int16_t x;
  int16_t y;
  int16_t err;
  int16_t old_err;
  boolean ChangeX;
  
  if(x0-r<0 || x0+r>=T::Width || y0-r<0 || y0+r>T::Height || color>1) return -1;
  
  x=0;
  y=-r;
  err=2-2*r;
  ChangeX=true;
  do{
    if(ChangeX) {
 
      Line(x0-x,y0-y,x0-x,y0+y,color);
      Line(x0+x,y0-y,x0+x,y0+y,color);
    } // if
    ChangeX=(old_err=err)<=x;
    if (ChangeX)            err+=++x*2+1;
    if (old_err>y || err>x) err+=++y*2+1;
  } while(y<=0);
    
  return 0;
} // MGLCD_buffered<T>::FillCircle

template<class T> int8_t MGLCD_buffered<T>::DrawBitmap(const uint8_t bmp[], uint8_t x, uint8_t y, uint8_t mem)
{
  uint8_t BmpWidth,BmpHeight,PageNum;
  uint8_t xx,page;
  uint8_t LcdData,BmpData;
  uint8_t ValidBits,mask;
  uint8_t col;
  
  if(mem!=MGLCD_ROM && mem!=MGLCD_RAM) return -1;
  if(mem==MGLCD_ROM) {
    BmpWidth =pgm_read_byte_near(bmp+0);
    BmpHeight=pgm_read_byte_near(bmp+1);
  } else { // mem==MGLCD_RAM
    BmpWidth =bmp[0];
    BmpHeight=bmp[1];
  } // if
  if(x+BmpWidth>T::Width || y+BmpHeight>T::Height) return -2;
  PageNum=(BmpHeight+7)>>3;
  for(page=0; page<PageNum; page++) {
    ValidBits=((page==PageNum-1) ? ((1<<((BmpHeight-1)&7)+1)-1) : 0xff);
    for(xx=0; xx<BmpWidth; xx++) {
      BmpData=mem==MGLCD_ROM ? pgm_read_byte_near(bmp+page*BmpWidth+xx+2) : bmp[page*BmpWidth+xx+2];
      if(InvertMode==MGLCD_INVERT) BmpData=~BmpData;
      col=x+xx;
      mask=ValidBits<<(y&7);
      LcdData=(ReadByteData(page+(y>>3),col)&(~mask)) | ((BmpData<<(y&7))&mask);
      PageAddress(page+(y>>3));
      ColumnAddress(col);
      WriteByteData(LcdData);
      if((y&7)>0 && (mask=ValidBits>>(8-(y&7)))!=0) {
        LcdData=(ReadByteData(page+(y>>3)+1,col)&(~mask)) | ((BmpData>>(8-(y&7)))&mask);
        PageAddress(page+(y>>3)+1);
        ColumnAddress(col);
        WriteByteData(LcdData);
      } // if
    } // for xx
  } // page
  return 0;
} // MGLCD_buffered<T>::DrawBitmap

template<class T> size_t MGLCD_buffered<T>::write(uint8_t ch)
{
  PutChar(ch);
  return 1;
} // MGLCD_buffered<T>::write

#endif // #ifdef MGLCD_BUFFERED_H
