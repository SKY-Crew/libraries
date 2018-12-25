/* 
  MGLCD_direct.h
  
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

#ifndef MGLCD_DIRECT_H
#define MGLCD_DIRECT_H

#include <arduino.h>
#include <Print.h>
#include <MGLCD.h>
#include <utility/MGLCD_font.h>
#include <utility/MGLCD_common.h>

#ifdef ARDUINO_ARCH_AVR

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

// Constants for WriteData/ReadData
#define MGLCD_CS1     2
#define MGLCD_CS2     4
#define MGLCD_CS1_CS2 6
#define MGLCD_COMMAND 0
#define MGLCD_DATA    1
#define MGLCD_CS1_COMMAND     (MGLCD_CS1     | MGLCD_COMMAND)
#define MGLCD_CS1_DATA        (MGLCD_CS1     | MGLCD_DATA   )
#define MGLCD_CS2_COMMAND     (MGLCD_CS2     | MGLCD_COMMAND)
#define MGLCD_CS2_DATA        (MGLCD_CS2     | MGLCD_DATA   )
#define MGLCD_CS1_CS2_COMMAND (MGLCD_CS1_CS2 | MGLCD_COMMAND)
#define MGLCD_CS1_CS2_DATA    (MGLCD_CS1_CS2 | MGLCD_DATA   )

// Default pins
#define STANDARD_A0_DI_PIN_OUT_REG  PORTC
#define STANDARD_A0_DI_PIN_BIT      3

#define STANDARD_CS1_E1_PIN_OUT_REG PORTC
#define STANDARD_CS1_E1_PIN_BIT     0

#define STANDARD_CS2_E2_PIN_OUT_REG PORTC
#define STANDARD_CS2_E2_PIN_BIT     1

#define STANDARD_E_PIN_OUT_REG   PORTC
#define STANDARD_E_PIN_BIT       4

#define STANDARD_RW_PIN_OUT_REG  PORTC
#define STANDARD_RW_PIN_BIT      2

// constants for TBus
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
#define PORT_NUM 3
#elif defined(__AVR_ATmega32U4__)
#define PORT_NUM 5
#else
#define PORT_NUM 11
#endif
#define BUS_WIDTH 8

// constants for TLcdPinAssignTable
#define MGLCD_UNUSED_PIN 255

#define MGLCD_CHAR_CODE_STRAIGHT 

// type definition
typedef struct{
  uint8_t A0_DI,CS1_E1,CS2_E2,E,RW,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7;
} TLcdPinAssignTable;

typedef struct{
  uint8_t PortNum;
  uint8_t mask[BUS_WIDTH];
  volatile uint8_t *ModeReg[BUS_WIDTH];
  volatile uint8_t *OutReg [BUS_WIDTH];
  volatile uint8_t *InReg  [BUS_WIDTH];
  uint8_t RegMask[PORT_NUM];
  volatile uint8_t *NModeReg[PORT_NUM];
  volatile uint8_t *NOutReg [PORT_NUM];
} TBus;

typedef struct{
  PortBit A0_DIPin;
  PortBit CS1_E1Pin;
  PortBit CS2_E2Pin;
  PortBit EPin;
  PortBit RWPin;
  TBus LcdBus;
} TPinSet;

template<class T> class MGLCD_direct : public Print
{
private:
  // This class's original member variables
  T specific;
  int16_t ConstructionError;
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

  // This class's original member functions
  void ScrollLine(uint8_t cs, uint8_t page, uint8_t x1, uint8_t x2);
  void scroll(void);

public:
  // This class's original member variables
  static const uint8_t DisplayOnCommand     = T::DisplayOnCommand    ;
  static const uint8_t DisplayOffCommand    = T::DisplayOffCommand   ;
  static const uint8_t DisplayLineCommand   = T::DisplayLineCommand  ;
  static const uint8_t PageAddressCommand   = T::PageAddressCommand  ;
  static const uint8_t ColumnAddressCommand = T::ColumnAddressCommand;


  // constructor
  explicit MGLCD_direct(const TLcdPinAssignTable &t);
  
  // destructor
  ~MGLCD_direct(void) {}
  
  // This class's original member functions
  uint8_t ReadData(uint8_t ctrl) { return specific.ReadData(ctrl,specific.PinSet); }
  void WriteData(uint8_t ctrl, uint8_t data) { specific.WriteData(ctrl,data,specific.PinSet); }
  void DisplayChar(uint8_t ch);
   
  // Member functions compatible with other classes
  int16_t GetConstructionError(void) { return ConstructionError; }
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
}; // MGLCD_direct

// Prototype declaration for functions in "MGLCD_direct.cpp"
void SetBusInputPullupMode(TBus const &LcdBus);
int16_t AssignBus(TBus &b, const uint8_t db[], boolean &StandardConnection);

template<class T> MGLCD_direct<T>::MGLCD_direct(const TLcdPinAssignTable &t)
  : UserCharNum(0), DecodeChar(MGLCD_CODE_STRAIGHT)
{
  boolean StandardConnection;
  uint8_t db[8];
  
  // assign pins
  if(specific.PinSet.A0_DIPin.AssignPin(t.A0_DI)) {
    ConstructionError=-1;
    return;
  } // if
  
  if(specific.PinSet.CS1_E1Pin.AssignPin(t.CS1_E1)) {
    ConstructionError=-2;
    return;
  } // if
  
  if(specific.PinSet.CS2_E2Pin.AssignPin(t.CS2_E2)) {
    ConstructionError=-3;
    return;
  }  // if
  
  if(T::EPinUsed && specific.PinSet.EPin.AssignPin(t.E)) {
    ConstructionError=-4;
    return;
  } // if
  
  if(specific.PinSet.RWPin.AssignPin(t.RW)) {
    ConstructionError=-5;
    return;
  } // if
  
  db[0]=t.DB0;
  db[1]=t.DB1;
  db[2]=t.DB2;
  db[3]=t.DB3;
  db[4]=t.DB4;
  db[5]=t.DB5;
  db[6]=t.DB6;
  db[7]=t.DB7;
  if(AssignBus(specific.PinSet.LcdBus, db, StandardConnection)) {
    ConstructionError=-6;
    return;
  } // if
     
  StandardConnection&=   (!T::EPinUsed || ((specific.PinSet.EPin     .OutReg==&STANDARD_E_PIN_OUT_REG     ) && (specific.PinSet.EPin     .mask==1<<STANDARD_E_PIN_BIT     )))
                      &&                   (specific.PinSet.RWPin    .OutReg==&STANDARD_RW_PIN_OUT_REG    ) && (specific.PinSet.RWPin    .mask==1<<STANDARD_RW_PIN_BIT    )
                      &&                   (specific.PinSet.A0_DIPin .OutReg==&STANDARD_A0_DI_PIN_OUT_REG ) && (specific.PinSet.A0_DIPin .mask==1<<STANDARD_A0_DI_PIN_BIT )
                      &&                   (specific.PinSet.CS1_E1Pin.OutReg==&STANDARD_CS1_E1_PIN_OUT_REG) && (specific.PinSet.CS1_E1Pin.mask==1<<STANDARD_CS1_E1_PIN_BIT)
                      &&                   (specific.PinSet.CS2_E2Pin.OutReg==&STANDARD_CS2_E2_PIN_OUT_REG) && (specific.PinSet.CS2_E2Pin.mask==1<<STANDARD_CS2_E2_PIN_BIT);
  
  //StandardConnection=false; // DEBUG (for testing non-standard connection)

  if(StandardConnection) {
    specific.ReadData =T::StandardReadData ;
    specific.WriteData=T::StandardWriteData;
  } else {
    specific.ReadData =T::GeneralReadData ;
    specific.WriteData=T::GeneralWriteData;
  } // if
  
  // set pin mode
  specific.PinSet.A0_DIPin .SetOutputMode();
  specific.PinSet.CS1_E1Pin.SetOutputMode();
  specific.PinSet.CS2_E2Pin.SetOutputMode();
  if(T::EPinUsed) specific.PinSet.EPin.SetOutputMode();
  specific.PinSet.RWPin    .SetOutputMode();
  SetBusInputPullupMode(specific.PinSet.LcdBus);
    
  ConstructionError=0;
  return;
} // MGLCD_direct<T>::MGLCD_direct

template<class T> void MGLCD_direct<T>::DisplayChar(uint8_t ch)
{
  uint8_t col;
  uint8_t cs;
  uint8_t data;

  // calculate col and cs
  col=(WindowX1+TextX)*MGLCD_FONT_WIDTH;
  if(col<T::HalfWidth) {
    cs=MGLCD_CS1;
  } else {
    cs=MGLCD_CS2;
    col-=T::HalfWidth;
  } // if
  
  // write one char
  WriteData(MGLCD_CS1_CS2_COMMAND, T::PageAddressCommand+WindowY1+TextY);
  WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+col);
  for(uint8_t i=0; i<MGLCD_FONT_WIDTH; i++) {
    if(col+i==T::HalfWidth) {
      cs=MGLCD_CS2;
      WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand);
    } // if  
    
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
    WriteData(cs|MGLCD_DATA, data);
  } // for i
} // MGLCD_direct<T>::DisplayChar

template<class T> int8_t MGLCD_direct<T>::Reset(uint8_t invert, uint8_t scroll)
{
  specific.Reset();
  if(SetTextWindow(0,0,T::ColumnNum-1,T::PageNum-1)!=0) return -1;
  if(SetInvertMode(invert)!=0) return -2;
  if(SetScrollMode(scroll)!=0) return -3;

  ClearScreen();
  
  return 0;
} // MGLCD_direct<T>::Reset

template<class T> int8_t MGLCD_direct<T>::ClearScreen(void)
{
  uint8_t i,j;
  uint8_t data;
  	
  data=InvertMode==MGLCD_INVERT ? 0xff : 0;
  i=T::PageNum;
  while(i--) {
    WriteData(MGLCD_CS1_CS2_COMMAND, T::PageAddressCommand+i);
    WriteData(MGLCD_CS1_CS2_COMMAND, T::ColumnAddressCommand);
    j=T::HalfWidth;
    while(j--) WriteData(MGLCD_CS1_CS2_DATA, data);
  } // while
  TextX=TextY=0;
  NewLine=false;
  
  return 0;
} // MGLCD_direct<T>::ClearScreen

template<class T> int8_t MGLCD_direct<T>::SetTextWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  if(x1>x2 || y1>y2 || x2>=T::ColumnNum || y2>=T::PageNum) return -1;
  
  WindowX1=x1; WindowY1=y1;
  WindowX2=x2; WindowY2=y2;
  
  TextX=TextY=0;  
  NewLine=true;
  
  return 0;
} // MGLCD_direct<T>::SetTextWindow

template<class T> int8_t MGLCD_direct<T>::SetInvertMode(uint8_t mode)
{
  if(mode!=MGLCD_NON_INVERT && mode!=MGLCD_INVERT) return -1;
  InvertMode=mode;
  
  return 0;
} // MGLCD_direct<T>::SetInvertMode

template<class T> int8_t MGLCD_direct<T>::SetScrollMode(uint8_t mode)
{
  if(mode!=MGLCD_NON_SCROLL && mode!=MGLCD_SCROLL) return -1;
  ScrollMode=mode;
  if(mode==MGLCD_NON_SCROLL && TextY>WindowY2-WindowY1) {
    TextY=0;
    NewLine=true;
  } // if
  
  return 0;
} // MGLCD_direct<T>::SetScrollMode

template<class T> int8_t MGLCD_direct<T>::SetCodeMode(PDecodeFunc CodeMode)
{ 
  if(CodeMode==NULL) return -1;
  DecodeChar=CodeMode; 
  return 0;
} // MGLCD_direct<T>::SetCodeMode

template<class T> int8_t MGLCD_direct<T>::Locate(uint8_t x, uint8_t y)
{
  if(x>T::ColumnNum || y>=T::PageNum || (x==T::ColumnNum && y!=0)) return -1;
  TextX=x;
  TextY=y;
  NewLine=false;
  
  return 0;
} // MGLCD_direct<T>::Locate

template<class T> void MGLCD_direct<T>::ScrollLine(uint8_t cs, uint8_t page, uint8_t x1, uint8_t x2)
{
  uint8_t buf[16];
  uint8_t cnt1, cnt2;

  for(; x1<=x2; x1+=sizeof buf) {
    cnt1=x2-x1+1;
    if(cnt1>sizeof buf) cnt1=sizeof buf;
    cnt2=cnt1;
    WriteData(cs|MGLCD_COMMAND, T::PageAddressCommand+page+1);
    WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+x1  );
    ReadData(cs|MGLCD_DATA);
    while(cnt1) buf[--cnt1]=ReadData(cs|MGLCD_DATA);
    WriteData(cs|MGLCD_COMMAND, T::PageAddressCommand+page);
    WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+x1);
    while(cnt2) WriteData(cs|MGLCD_DATA, buf[--cnt2]);
  } // for x1
} // MGLCD_direct<T>::ScrollLine

template<class T> void MGLCD_direct<T>::scroll(void)
{
  uint8_t xstart, xend;
  uint8_t i;
        
  xstart=WindowX1*MGLCD_FONT_WIDTH; 
  xend  =(WindowX2+1)*MGLCD_FONT_WIDTH-1;
  for(i=WindowY1; i<WindowY2; i++) {
    if(xend<T::HalfWidth) { // text window is within left half of the screen
      ScrollLine(MGLCD_CS1, i, xstart, xend);
    } else if(xstart>=T::HalfWidth) { // text window is within right half of the screen
      ScrollLine(MGLCD_CS2, i, xstart-T::HalfWidth, xend-T::HalfWidth);
    } else { // text windows corosses center line
      ScrollLine(MGLCD_CS1, i, xstart, T::HalfWidth-1);
      ScrollLine(MGLCD_CS2, i, 0, xend-T::HalfWidth);
    } // if
  } // for i
} // MGLCD_direct<T>::scroll

template<class T> int8_t MGLCD_direct<T>::ClearRestOfLine(void)
{
  int16_t i;
  uint8_t data;
  uint8_t cs;
  
  if(TextY>WindowY2-WindowY1) return 0;
  WriteData(MGLCD_CS1_CS2_COMMAND, T::PageAddressCommand+WindowY1+TextY);
  data=InvertMode==MGLCD_INVERT ? 0xff : 0;
  i=(WindowX1+TextX)*MGLCD_FONT_WIDTH;
  if(i<T::HalfWidth) {
    cs=MGLCD_CS1;
    WriteData(MGLCD_CS1|MGLCD_COMMAND, T::ColumnAddressCommand+i);
  } else {
    cs=MGLCD_CS2;
    WriteData(MGLCD_CS2|MGLCD_COMMAND, T::ColumnAddressCommand+i-T::HalfWidth);
  } // if
  do {
    if(i==T::HalfWidth) {
      WriteData(MGLCD_CS2_COMMAND, T::ColumnAddressCommand);
      cs=MGLCD_CS2;
    } // if
    WriteData(cs|MGLCD_DATA, data); 
  } while(++i<(WindowX2+1)*MGLCD_FONT_WIDTH);
  
  return 0;
} // MGLCD_direct<T>::ClearRestOfLine

template<class T> int8_t MGLCD_direct<T>::PutChar(uint8_t ch)
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
} // MGLCD_direct<T>::PutChar

template<class T> int8_t MGLCD_direct<T>::UserChars(const uint8_t uc[][5], uint8_t num, uint8_t mem)
{
  if(num==0 || num>32 || uc==NULL || (mem!=MGLCD_ROM && mem!=MGLCD_RAM)) return -1;
  
  UserCharNum=num;
  UserCharPtr=uc;
  UserCharMemoryType=mem;

  return 0;
} // MGLCD_direct<T>::UserChars

template<class T> int8_t MGLCD_direct<T>::SetPixel(uint8_t x, uint8_t y, uint8_t color)
{
  uint8_t mask;
  uint8_t cs;
  uint8_t data;
  
  if(x>=T::Width || y>=T::Height || color>1) return -1;
  
  if(InvertMode==MGLCD_INVERT) color=1-color;

  mask=1<<(y&7);
  
  if(x<T::HalfWidth) {
    cs=MGLCD_CS1;
  } else {
    cs=MGLCD_CS2;
    x-=T::HalfWidth;
  } // if

  WriteData(cs|MGLCD_COMMAND, T::PageAddressCommand  +(y>>3));  
  WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+x     );
  ReadData(cs|MGLCD_DATA);
  data=(ReadData(cs|MGLCD_DATA)&~mask)|(color ? mask : 0);
  WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+x     );
  WriteData(cs|MGLCD_DATA, data);  
  
  return 0;
} // MGLCD_direct<T>::PutPixel

template<class T> int8_t MGLCD_direct<T>::GetPixel(uint8_t x, uint8_t y)
{
  uint8_t cs;
  
  if(x>=T::Width || y>=T::Height) return -1;

  if(x<T::HalfWidth) {
    cs=MGLCD_CS1;
  } else {
    cs=MGLCD_CS2;
    x-=T::HalfWidth;
  } // if
  WriteData(cs|MGLCD_COMMAND, T::PageAddressCommand  +(y>>3));  
  WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+x     );
  ReadData(cs|MGLCD_DATA);
  return ((ReadData(cs|MGLCD_DATA)>>(y&7)) & 1) ^ (InvertMode==MGLCD_INVERT);
} // MGLCD_direct<T>::GetPixel

template<class T> int8_t MGLCD_direct<T>::Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
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
    uint8_t cs;
    uint8_t page,OldPage;
    uint8_t data;
    int OldY;
    
    page=OldPage=x1>>3;
    WriteData(MGLCD_CS1_CS2_COMMAND, T::PageAddressCommand+page);
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
        if(OldY<T::HalfWidth) {
          cs=MGLCD_CS1;
        } else {
          cs=MGLCD_CS2;
          OldY-=T::HalfWidth;
        } // if
        if(data!=0xff) {
          uint8_t b;
            
          WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+OldY);
          ReadData(cs|MGLCD_DATA);
          b=ReadData(cs|MGLCD_DATA);
          data=color ? data|b : ~data&b;
        } else { 
          if(!color) data=~data;
        } // if
        WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+OldY);
        WriteData(cs|MGLCD_DATA, data);  
        data=0;
        if(page!=OldPage && x<x2) {
          WriteData(MGLCD_CS1_CS2_COMMAND, T::PageAddressCommand+page);
          OldPage=page;
        } // if
      } // if
    } // for x
  } else { // within +-45 deg. from horizontal line
    uint8_t mask;
    uint8_t cs;
    uint8_t page,OldPage;
    uint8_t buf1[22];
    uint8_t buf2[sizeof buf1];
    uint8_t BufIndex;
    uint8_t i;

    cs=x1<T::HalfWidth ? MGLCD_CS1 : MGLCD_CS2;
    mask=1<<(y&7); 
    page=OldPage=y>>3;
    WriteData(MGLCD_CS1_CS2_COMMAND, T::PageAddressCommand+page);
    BufIndex=0;
    for(x=x1; x<=x2; x++) {
      buf1[BufIndex++]=mask;
      
      sum+=dy;
      if(sum>=x2-x1) {
        sum-=x2-x1;
        if(++y&7) {
          mask<<=1;
        } else { 
          mask=1;
          page++;
        } // if 
      } else if(sum<=x1-x2) {
        if(y--&7) {
          mask>>=1;
        } else {
          mask=0x80;
          page--; 
        } // if
        sum+=x2-x1;
      } // if
      if(BufIndex==sizeof buf1 || page!=OldPage || x==T::HalfWidth-1 || x==x2) {
        WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+x-BufIndex+1-(cs==MGLCD_CS2 ? T::HalfWidth : 0));
        ReadData(cs|MGLCD_DATA);
        for(i=0; i<BufIndex; i++) buf2[i]=ReadData(cs|MGLCD_DATA);
        WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+x-BufIndex+1-(cs==MGLCD_CS2 ? T::HalfWidth : 0));
        if(color) {
          for(i=0; i<BufIndex; i++) WriteData(cs|MGLCD_DATA, buf1[i]|buf2[i]);
        } else {
          for(i=0; i<BufIndex; i++) WriteData(cs|MGLCD_DATA,~buf1[i]&buf2[i]);
        } // if
        BufIndex=0;
        if(x==T::HalfWidth-1) cs=MGLCD_CS2;
        if(page!=OldPage && x<x2) { 
          WriteData(MGLCD_CS1_CS2_COMMAND, T::PageAddressCommand+page);
          OldPage=page;
        } // if  
      } // if
    } // for x
  } //if
    
  return 0;
} // MGLCD_direct<T>::Line

template<class T> int8_t MGLCD_direct<T>::Rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
  int8_t result;
  
  result=0;
  if(Line(x1,y1,x2,y1,color)) result=-1;
  if(Line(x2,y1,x2,y2,color)) result=-2;
  if(Line(x2,y2,x1,y2,color)) result=-3;
  if(Line(x1,y2,x1,y1,color)) result=-4;
  return result;
} // MGLCD_direct<T>::Rect

template<class T> int8_t MGLCD_direct<T>::FillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
  uint8_t page,EndPage,PageY;
  uint8_t data,mask;
  uint8_t cs;
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
    WriteData(MGLCD_CS1_CS2_COMMAND, T::PageAddressCommand+page);
    if(x1<T::HalfWidth) {
      cs=MGLCD_CS1; 
      WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+x1);
    } else {
      cs=MGLCD_CS2;
      WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+x1-T::HalfWidth);
    } // if
        
    for(i=x1; i<=x2; i++) {
      // change cs when i=HalfWidth
      if(i==T::HalfWidth) {
        cs=MGLCD_CS2;
        WriteData(MGLCD_CS2_COMMAND, T::ColumnAddressCommand);
      } // if
      
      // send 1 byte
      ReadData(cs|MGLCD_DATA);
      if(color) {
        data=ReadData(cs|MGLCD_DATA)|mask;
      } else {
        data=ReadData(cs|MGLCD_DATA)&~mask;
      } // if
      WriteData(cs|MGLCD_COMMAND, T::ColumnAddressCommand+(cs==MGLCD_CS1 ? i : i-T::HalfWidth));
      WriteData(cs|MGLCD_DATA, data);      
    } // for i
    
    PageY+=8;
  } // for page
  
  return 0;
} // MGLCD_direct<T>::FillRect

template<class T> int8_t MGLCD_direct<T>::RoundRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t color)
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
} // MGLCD_direct<T>::RoundRect
  
// This circle routine's algorithm is based on http://free.pages.at/easyfilter/bresenham.html .
template<class T> int8_t MGLCD_direct<T>::Circle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
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
} // MGLCD_direct<T>::Circle

// This circle routine's algorithm is based on http://free.pages.at/easyfilter/bresenham.html .
template<class T> int8_t MGLCD_direct<T>::FillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
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
} // MGLCD_direct<T>::FillCircle

template<class T> int8_t MGLCD_direct<T>::DrawBitmap(const uint8_t bmp[], uint8_t x, uint8_t y, uint8_t mem)
{
  uint8_t BmpWidth,BmpHeight,PageNum;
  uint8_t xx,page;
  uint8_t LcdData,BmpData;
  uint8_t ValidBits,mask;
  uint8_t cs;
  uint8_t ColumnCommand;
  
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
      if(x+xx<T::HalfWidth) {
        cs=MGLCD_CS1;
        ColumnCommand=T::ColumnAddressCommand+x+xx;
      } else {
        cs=MGLCD_CS2;
        ColumnCommand=T::ColumnAddressCommand+x+xx-T::HalfWidth;
      } // if
      mask=ValidBits<<(y&7);
      WriteData(cs|MGLCD_COMMAND,T::PageAddressCommand+page+(y>>3));
      WriteData(cs|MGLCD_COMMAND,ColumnCommand);
      ReadData(cs|MGLCD_DATA);
      LcdData=(ReadData(cs|MGLCD_DATA)&(~mask)) | (( BmpData<<(y&7))&mask);
      WriteData(cs|MGLCD_COMMAND,ColumnCommand);
      WriteData(cs|MGLCD_DATA,LcdData);
      if((y&7)>0 && (mask=ValidBits>>(8-(y&7)))!=0) {
        WriteData(cs|MGLCD_COMMAND,T::PageAddressCommand+page+(y>>3)+1);
        WriteData(cs|MGLCD_COMMAND,ColumnCommand);
        ReadData(cs|MGLCD_DATA);
        LcdData=(ReadData(cs|MGLCD_DATA)&(~mask)) | (( BmpData>>(8-(y&7)))&mask);
        WriteData(cs|MGLCD_COMMAND,ColumnCommand);
        WriteData(cs|MGLCD_DATA,LcdData);
      } // if
    } // for xx
  } // page
  return 0;
} // MGLCD_direct<T>::DrawBitmap

template<class T> size_t MGLCD_direct<T>::write(uint8_t ch)
{
  PutChar(ch);
  return 1;
} // MGLCD_direct<T>::write

#endif // #ifdef ARDUINO_ARCH_AVR
#endif // #ifndef MGLCD_DIRECT_H

