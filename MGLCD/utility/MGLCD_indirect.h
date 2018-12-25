/* 
  MGLCD_indirect.h
  
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

#ifndef MGLCD_INDIRECT_H
#define MGLCD_INDIRECT_H

#include <arduino.h>
#include <Print.h>
#include <MGLCD.h>

#if MGLCD_VERSION != 42
#error versions of this file and header file are different.
#endif

#define MGLCD_COMMUNICATION_ERROR (-128)

// Constats for Backlight function
#define MGLCD_BACKLIGHT_OFF         0
#define MGLCD_BACKLIGHT_ON          1
#define MGLCD_GET_BACKLIGHT_STATE 127

// Constants for Contrast function
#define MGLCD_MAX_CONTRAST          15
#define MGLCD_GET_CURRENT_CONTRAST 126
#define MGLCD_GET_DEFAULT_CONTRAST 127
#define MGLCD_NOT_SET_DEFAULT        0
#define MGLCD_SET_DEFAULT            1

// Constants for ReadButton function
#define MGLCD_BUTTON_OFF       0
#define MGLCD_BUTTON_ON        1
#define MGLCD_GET_BUTTON_NUM 127

// Constants for Led function
#define MGLCD_LED_OFF         0
#define MGLCD_LED_ON          1
#define MGLCD_LED_BLINK       2
#define MGLCD_LED_TOGGLE      3
#define MGLCD_GET_LED_NUM   126
#define MGLCD_GET_LED_STATE 127

class SoftwareSerial;

template<class T> inline void FlushWriteBuffer(T *interface) {}
template<> inline void FlushWriteBuffer(HardwareSerial *interface){ interface->flush(); }

template<class T> class MGLCD_indirect : public Print
{
public:
  static const int MaxCommandSize = 16;
  static const int MaxCommandResultSize = 10;
  static const int PreferredBaudRateNum = 14;
  static const unsigned long DefaultBaudRate = 1200;
  
  enum PortCommand {
    SCPing,
    SCChangeBaudRate,
    SCReset,
    SCClearScreen,
    SCSetTextWindow,
    SCSetInvertMode,
    SCSetScrollMode,
    SCSetCodeMode,
    SCGetCodeMode,
    SCLocate,
    SCGetX,
    SCGetY,
    SCClearRestOfLine,
    SCUserChar,
    SCSetPixel,
    SCGetPixel,
    SCLine,
    SCRect,
    SCFillRect,
    SCRoundRect,
    SCCircle,
    SCFillCircle,
    SCBacklight,
    SCContrast,
    SCStartDrawBitmap,
    SCSendBitmap,
    SCReadButton,
    SCLed,
    SCMax
  };
  
  static PROGMEM const uint8_t ParamNum[SCMax][2];
  static PROGMEM const uint32_t PreferredBaudRate[PreferredBaudRateNum];

private:
  // This class's original member variables
  static const uint8_t XON  = 0x11; // XON character
  static const uint8_t XOFF = 0x13; // XOFF character
  static const uint8_t STX  = 0x02; // STX character
  static const uint8_t ETX  = 0x03; // ETX character
  static const uint8_t ACK  = 0x06; // ACK character
  static const uint8_t NAK  = 0x15; // NAK character
  static const uint8_t ERR  = 0x07; // character which means errors occured in command

  uint8_t command[MaxCommandSize];
  uint8_t CommandResult[MaxCommandResultSize];
  uint16_t BitBuffer;
  uint8_t ValidBitNum;
  uint32_t BaudRate;
  uint8_t PingCnt;
  
  // Member variables compatible with the other classes
  uint8_t WindowX1,WindowY1,WindowX2,WindowY2; 
  uint8_t InvertMode;
  uint8_t ScrollMode;
  uint8_t Width;     // Screen width
  uint8_t Height;    // Screen height
  uint8_t HalfWidth; // Half of Width
  uint8_t PageNum;   // Number of pages (character lines)
  uint8_t ColumnNum; // Number of character columns

  // This class's original member functions
  T *pInterface;
  void SendBits(uint8_t data, uint8_t BitNum);
  int16_t ReceiveBits(uint8_t BitNum, uint32_t StartMillis, uint16_t time_out);
  int8_t SendCommand(uint16_t time_out=1000);

public:
  // constructor
  explicit MGLCD_indirect(T *p, unsigned long baud=DefaultBaudRate);
  
  // destructor
  ~MGLCD_indirect(void) {}
  
  // This class's original member functions
  int8_t ChangeBaudRate(uint32_t NewBaudRate);
  int8_t Backlight(uint8_t OnOff);
  int8_t Contrast(uint8_t val, uint8_t option=MGLCD_SET_DEFAULT);
  int8_t ReadButton(uint8_t ButtonNo);
  int8_t WaitForButtons(int8_t b0   , int8_t b1=-1, int8_t b2=-1, int8_t b3=-1,
                        int8_t b4=-1, int8_t b5=-1, int8_t b6=-1, int8_t b7=-1);
  int8_t Led(uint8_t LedNo, uint8_t state=MGLCD_LED_OFF);
  
  // The same member functions compatible with the other classes
  int16_t GetConstructionError(void) { return 0; }
  int8_t Reset(uint8_t invert=MGLCD_NON_INVERT, uint8_t scroll=MGLCD_SCROLL);
  int8_t ClearScreen(void);
  uint8_t GetWidth(void) { return Width; }
  uint8_t GetHeight(void) { return Height; }
  uint8_t GetPageNum(void) { return PageNum; }
  uint8_t GetLineNum(void) { return PageNum; }
  uint8_t GetColumnNum(void) { return ColumnNum; }
  int8_t SetTextWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
  void GetTextWindow(uint8_t &x1, uint8_t &y1, uint8_t &x2, uint8_t &y2) { x1=WindowX1; y1=WindowY1; x2=WindowX2; y2=WindowY2; }
  int8_t SetInvertMode(uint8_t mode);
  uint8_t GetInvertMode(void) { return InvertMode; }
  int8_t SetScrollMode(uint8_t mode);
  uint8_t GetScrollMode(void) { return ScrollMode; }
  int8_t SetCodeMode(PDecodeFunc CodeMode);
  PDecodeFunc GetCodeMode(void);
  int8_t Locate(uint8_t x, uint8_t y);
  int16_t GetX(void);
  int16_t GetY(void);  
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
  size_t write(uint8_t); // inherit Print
}; // MGLCD_indirect

typedef MGLCD_indirect<HardwareSerial> MGLCD_serial;
typedef MGLCD_indirect<SoftwareSerial> MGLCD_SoftwareSerial;

typedef MGLCD_serial mglcd_serial; // for compatibility
typedef MGLCD_SoftwareSerial mglcd_SoftwareSerial; // for compatibility

template<class T> PROGMEM const uint8_t MGLCD_indirect<T>::ParamNum[SCMax][2] = {
// Left number is number of command parameter.  Right number is number of return parameter.
  { 0, 0 }, // SCPing
  { 4, 0 }, // SCChangeBaudRate
  { 4, 2 }, // SCReset
  { 0, 0 }, // SCClearScreen
  { 4, 0 }, // SCSetTextWindow
  { 1, 0 }, // SCSetInvertMode
  { 1, 0 }, // SCSetScrollMode
  { 1, 0 }, // SCSetCodeMode
  { 0, 1 }, // SCGetCodeMode
  { 2, 0 }, // SCLocate
  { 0, 1 }, // SCGetX
  { 0, 1 }, // SCGetY 
  { 0, 0 }, // SCClearRestOfLine,
  { 6, 0 }, // SCUserChar,
  { 3, 0 }, // SCSetPixel,
  { 2, 1 }, // SCGetPixel,
  { 5, 0 }, // SCLine
  { 5, 0 }, // SCRect
  { 5, 0 }, // SCFillRect
  { 6, 0 }, // SCRoundRect
  { 4, 0 }, // SCCircle
  { 4, 0 }, // SCFillCircle
  { 1, 1 }, // SCBacklight
  { 2, 1 }, // SCContrast
  { 4, 0 }, // SCStartDrawBitmap
  { MaxCommandSize-1,0 }, // SCSendBitmap
  { 1, 1 }, // SCReadButton
  { 2, 1 }, // SCLed
};

template<class T> PROGMEM const uint32_t MGLCD_indirect<T>::PreferredBaudRate[PreferredBaudRateNum] = {
     300,   600,  1200,  2400,  4800,
    9600, 14400, 19200, 28800, 38400,
   57600,115200,250000,500000
};

template<class T> void MGLCD_indirect<T>::SendBits(uint8_t data, uint8_t BitNum)
{
  BitBuffer=(BitBuffer<<BitNum)|data;
  ValidBitNum+=BitNum;
  while(ValidBitNum>=7) {
    pInterface->write(0x80|((BitBuffer>>(ValidBitNum-=7))&0x7f));
  } // while
} // MGLCD_indirect<T>::SendBits

template<class T> int16_t MGLCD_indirect<T>::ReceiveBits(uint8_t BitNum, uint32_t StartMillis, uint16_t time_out)
{
  uint8_t data;
  
  while(ValidBitNum<BitNum) {
    while(pInterface->available()==0) if(millis()-StartMillis>time_out) return MGLCD_COMMUNICATION_ERROR;
    data=pInterface->read();
    if((data&0x80)==0) return MGLCD_COMMUNICATION_ERROR;    
    BitBuffer=(BitBuffer<<7)|(data&0x7f);
    ValidBitNum+=7;
  } // while
  ValidBitNum-=BitNum;
  return (BitBuffer>>ValidBitNum)&((1<<BitNum)-1);
} // MGLCD_indirect<T>::ReceiveBits

template<class T> int8_t MGLCD_indirect<T>::SendCommand(uint16_t time_out)
{
  uint8_t parity;
  int16_t i;
  uint8_t CommandSize;
  uint32_t StartMillis;
  int16_t data;
  uint8_t CommandResultSize;
 
  if(command[0]>=SCMax) return -1; // Undefined command

  // Get and check command size
  CommandSize=(uint8_t)pgm_read_byte_near(&ParamNum[command[0]][0])+1; 
  if(CommandSize>MaxCommandSize) return -2;
    
  while(pInterface->available()) pInterface->read(); // Delete read buffer
    
  pInterface->write(STX);
  ValidBitNum=0;
  SendBits(command[0],6);
  parity=(command[0]>>4)^(command[0]&0xf);

  for(i=1; i<CommandSize; i++) {
    uint8_t b=command[i];
    SendBits(b,8);
    parity^=(b>>4)^(b&0xf);
  } // for i
  
  SendBits(parity,4);
  SendBits(0,6); // Flush BitBuffer
  pInterface->write(ETX);
  FlushWriteBuffer(pInterface);

  
  // Wait for first byte of command result
  StartMillis=millis();
  while(true) {
    data=pInterface->read();
    if(data==ACK) {
      return 0;
    } else if(data==NAK) {
      return MGLCD_COMMUNICATION_ERROR;
    } else if(data==ERR) {
      return -3;
    } else if(data==STX) {
      break;
    } else if(data==ETX) {
      return MGLCD_COMMUNICATION_ERROR;
    } // if
    if(millis()-StartMillis>time_out) return MGLCD_COMMUNICATION_ERROR;
  } // while
  
  // Get command result size
  CommandResultSize=(uint8_t)pgm_read_byte_near(&ParamNum[command[0]][1]);

  // Receive command result
  ValidBitNum=0;
  parity=0;
  for(i=0; i<CommandResultSize; i++) {
    if((data=ReceiveBits(8,StartMillis,time_out))<0) return MGLCD_COMMUNICATION_ERROR;
    CommandResult[i]=data;
    parity^=(data>>4)^(data&0xf);
  } // for i

  // Recieve parity
  if((data=ReceiveBits(4,StartMillis,time_out))<0 || data!=parity) return MGLCD_COMMUNICATION_ERROR;
  
  // Recieve ETX
  while(pInterface->available()==0) if(millis()-StartMillis>time_out) return MGLCD_COMMUNICATION_ERROR;
  if(pInterface->read()!=ETX) return MGLCD_COMMUNICATION_ERROR;
  
  return 0;
} // MGLCD_indirect<T>::SendCommand

template<class T> MGLCD_indirect<T>::MGLCD_indirect(T *p, uint32_t baud) : pInterface(p), PingCnt(0), BaudRate(baud)
{
} // MGLCD_indirect<T>::MGLCD_indirect

template<class T> int8_t MGLCD_indirect<T>::ChangeBaudRate(uint32_t NewBaudRate)
{
  for(int i=0; i<PreferredBaudRateNum+1; i++) {
    if(i>0) pInterface->begin(pgm_read_dword_near(PreferredBaudRate+(i-1)));
    command[0]=SCChangeBaudRate; 
    command[1]=(uint8_t)( NewBaudRate     &0xff);
    command[2]=(uint8_t)((NewBaudRate>> 8)&0xff);
    command[3]=(uint8_t)((NewBaudRate>>16)&0xff);
    command[4]=(uint8_t)((NewBaudRate>>24)&0xff);
    if(SendCommand(100)==0) {
      delay(36);
      pInterface->begin(NewBaudRate);
      BaudRate=NewBaudRate;
      return 0;
    } // if
  } // for i
  
  return MGLCD_COMMUNICATION_ERROR;
} // MGLCD_indirect<T>::ChangeBaudRate

template<class T> int8_t MGLCD_indirect<T>::Backlight(uint8_t OnOff)
{
  int8_t result;
  
  command[0]=SCBacklight;
  command[1]=OnOff;
  result=SendCommand();
  if(result!=0) return result;
  return CommandResult[0];
} // MGLCD_indirect<T>::Backlight

template<class T> int8_t MGLCD_indirect<T>::Contrast(uint8_t val, uint8_t option)
{
  int8_t result;
  
  command[0]=SCContrast;
  command[1]=val;
  command[2]=option;
  result=SendCommand();
  if(result!=0) return result;
  return CommandResult[0];
} // MGLCD_indirect<T>::Contrast

template<class T> int8_t MGLCD_indirect<T>::Reset(uint8_t invert, uint8_t scroll)
{
  int8_t result;

  // Set baud rate
  pInterface->begin(BaudRate);
  
  // Initialize port
  FlushWriteBuffer(pInterface);
  delay(36);

  // Change baud rate
  result=ChangeBaudRate(BaudRate);
  if(result!=0) return result;
  
  // Send reset comand
  command[0]=SCReset;
  command[1]=invert;
  command[2]=scroll;
  command[3]=MGLCD_VERSION&0xff;
  command[4]=MGLCD_VERSION>>8;
  result=SendCommand();
  if(result!=0) return result;

  // Initialize variables
  Width =CommandResult[0];
  Height=CommandResult[1];
  HalfWidth=Width/2;
  PageNum=Height/8;
  ColumnNum=Width/MGLCD_FONT_WIDTH;
  InvertMode=invert;
  ScrollMode=scroll;
  
  return 0;
} // MGLCD_indirect<T>::Reset

template<class T> int8_t MGLCD_indirect<T>::ClearScreen(void)
{
  command[0]=SCClearScreen;
  return SendCommand();
} // MGLCD_indirect<T>::ClearScreen

template<class T> int8_t MGLCD_indirect<T>::SetTextWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  int8_t result;
  
  command[0]=SCSetTextWindow;
  command[1]=x1;
  command[2]=y1;
  command[3]=x2;
  command[4]=y2;
  result=SendCommand();
  if(result==0) {
    WindowX1=x1; WindowY1=y1;
    WindowX2=x2; WindowY2=y2;
  } // if
  
  return result;
} // MGLCD_indirect<T>::SetTextWindow

template<class T> int8_t MGLCD_indirect<T>::SetInvertMode(uint8_t mode)
{
  int8_t result;

  command[0]=SCSetInvertMode;
  command[1]=mode;
  result=SendCommand();
  if(result==0) InvertMode=mode;
  return result;
} // MGLCD_indirect<T>::SetInvertMode

template<class T> int8_t MGLCD_indirect<T>::SetScrollMode(uint8_t mode)
{
  int8_t result;

  command[0]=SCSetScrollMode;
  command[1]=mode;
  result=SendCommand();
  if(result==0) ScrollMode=mode;
  return result;
} // MGLCD_indirect<T>::SetScrollMode

template<class T> int8_t MGLCD_indirect<T>::SetCodeMode(PDecodeFunc CodeMode)
{
  int8_t index;
  
  index=CodeModeToIndex(CodeMode);
  if(index<0) return -1;
  command[0]=SCSetCodeMode;
  command[1]=(uint8_t)index;
  return SendCommand();
} // MGLCD_indirect<T>::SetCodeMode

template<class T> PDecodeFunc MGLCD_indirect<T>::GetCodeMode(void)
{
  int8_t result;
  
  command[0]=SCGetCodeMode;
  result=SendCommand();
  if(result!=0) return NULL;
  return IndexToCodeMode(CommandResult[0]);
} // MGLCD_indirect<T>::GetCodeMode

template<class T> int8_t MGLCD_indirect<T>::Locate(uint8_t x, uint8_t y)
{
  command[0]=SCLocate;
  command[1]=x;
  command[2]=y;
  return SendCommand();
} // MGLCD_indirect<T>::Locate

template<class T> int16_t MGLCD_indirect<T>::GetX(void)
{
  int8_t result;
  
  command[0]=SCGetX;
  result=SendCommand();
  if(result!=0) return result;
  return CommandResult[0];
} // MGLCD_indirect<T>::GetX

template<class T> int16_t MGLCD_indirect<T>::GetY(void)
{
  int8_t result;
  
  command[0]=SCGetY;
  result=SendCommand();
  if(result!=0) return result;

  return CommandResult[0];
} // MGLCD_indirect<T>::GetY

template<class T> int8_t MGLCD_indirect<T>::ClearRestOfLine(void)
{
  command[0]=SCClearRestOfLine;
  return SendCommand();
} // MGLCD_indirect<T>::ClearRestOfLine

template<class T> int8_t MGLCD_indirect<T>::PutChar(uint8_t ch)
{
  int8_t result;

  // flush Rx buffer of LCD controller
  if(++PingCnt>=16) {
    PingCnt=0;
    command[0]=SCPing;
    result=SendCommand();
    if(result!=0) return result; 
  } // if
  
  if(ch==STX || ch==ETX || ch==ACK || ch==NAK || ch==XON || ch==XOFF) ch=' ';
  pInterface->write(ch);
  
  return 0;
} // MGLCD_indirect<T>::PutChar

template<class T> int8_t MGLCD_indirect<T>::UserChars(const uint8_t uc[][5], uint8_t num, uint8_t mem)
{
  uint8_t ChNo, x;
  int8_t result;
  
  if(uc==NULL || num==0 || num>32 || (mem!=MGLCD_ROM && mem!=MGLCD_RAM)) return -10;
  for(ChNo=0; ChNo<32; ChNo++) {
    command[0]=SCUserChar;
    command[1]=ChNo;
    if(ChNo<num) {
      for(x=0; x<5; x++) {
        command[x+2]=mem==MGLCD_ROM ? pgm_read_byte_near(&uc[ChNo][x]) : uc[ChNo][x];
      } // for x
    } else {
      command[2]=0;
      command[3]=0;
      command[4]=0;
      command[5]=0;
      command[6]=0;
    } // if
    result=SendCommand();
    
    if(result!=0) return result;
  } // for ChNo
  
  return 0;
} // MGLCD_indirect<T>::UserChars

template<class T> int8_t MGLCD_indirect<T>::SetPixel(uint8_t x, uint8_t y, uint8_t color)
{
  command[0]=SCSetPixel;
  command[1]=x;
  command[2]=y;
  command[3]=color;
  return SendCommand();
} // MGLCD_indirect<T>::SetPixel

template<class T> int8_t MGLCD_indirect<T>::GetPixel(uint8_t x, uint8_t y)
{
  int8_t result;
  
  command[0]=SCGetPixel;
  command[1]=x;
  command[2]=y;
  result=SendCommand();
  if(result!=0) return result;

  return (int8_t) CommandResult[0];
} // MGLCD_indirect<T>::GetPixel

template<class T> int8_t MGLCD_indirect<T>::Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
  command[0]=SCLine;
  command[1]=x1;
  command[2]=y1;
  command[3]=x2;
  command[4]=y2;
  command[5]=color;
  return SendCommand();
} // MGLCD_indirect<T>::Line

template<class T> int8_t MGLCD_indirect<T>::Rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{

  command[0]=SCRect;
  command[1]=x1;
  command[2]=y1;
  command[3]=x2;
  command[4]=y2;
  command[5]=color;
  return SendCommand();
} // MGLCD_indirect<T>::Rect

template<class T> int8_t MGLCD_indirect<T>::FillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
  command[0]=SCFillRect;
  command[1]=x1;
  command[2]=y1;
  command[3]=x2;
  command[4]=y2;
  command[5]=color;
  return SendCommand();
} // MGLCD_indirect<T>::FillRect

template<class T> int8_t MGLCD_indirect<T>::RoundRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t r, uint8_t color)
{
  command[0]=SCRoundRect;
  command[1]=x1;
  command[2]=y1;
  command[3]=x2;
  command[4]=y2;
  command[5]=r;
  command[6]=color;
  return SendCommand();
} // MGLCD_indirect<T>::RoundRect

template<class T> int8_t MGLCD_indirect<T>::Circle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
{
  command[0]=SCCircle;
  command[1]=x0;
  command[2]=y0;
  command[3]=r;
  command[4]=color;
  return SendCommand();
} // MGLCD_indirect<T>::Circle

template<class T> int8_t MGLCD_indirect<T>::FillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
{
  command[0]=SCFillCircle;
  command[1]=x0;
  command[2]=y0;
  command[3]=r;
  command[4]=color;
  return SendCommand();
} // MGLCD_indirect<T>::FillCircle

template<class T> int8_t MGLCD_indirect<T>::DrawBitmap(const uint8_t bmp[], uint8_t x, uint8_t y, uint8_t mem)
{
  uint8_t BmpWidth,BmpHeight;
  uint16_t DataNum;
  uint16_t DataIndex;
  uint8_t i;

  if(mem!=MGLCD_ROM && mem!=MGLCD_RAM) return -10;
  if(mem==MGLCD_ROM) {
    BmpWidth =pgm_read_byte_near(bmp+0);
    BmpHeight=pgm_read_byte_near(bmp+1);
  } else { // mem==MGLCD_RAM
    BmpWidth =bmp[0];
    BmpHeight=bmp[1];
  } // if
  if(x+BmpWidth>Width || y+BmpHeight>Height) return -11;

  command[0]=SCStartDrawBitmap;
  command[1]=BmpWidth;
  command[2]=BmpHeight;
  command[3]=x;
  command[4]=y;
  if(SendCommand()!=0) return MGLCD_COMMUNICATION_ERROR;

  DataNum=BmpWidth*((BmpHeight+7)/8)+2;
  for(DataIndex=2; DataIndex<DataNum; DataIndex+=MaxCommandSize-1) {
    command[0]=SCSendBitmap;
    for(i=0; i<MaxCommandSize-1; i++) {
      command[i+1]=DataIndex+i<DataNum ? (mem==MGLCD_ROM ? pgm_read_byte_near(&bmp[DataIndex+i]) : bmp[DataIndex+i]) : 0;
    } // for i
    if(SendCommand()!=0) return MGLCD_COMMUNICATION_ERROR;
  } // for DataIndex
  
  return 0;
} // MGLCD_indirect<T>::DrawBitmap

template<class T> int8_t MGLCD_indirect<T>::ReadButton(uint8_t ButtonNo)
{
  int8_t result;
  
  command[0]=SCReadButton;
  command[1]=ButtonNo;
  result=SendCommand();
  if(result!=0) return result;
  return CommandResult[0];
} // MGLCD_indirect<T>::ReadButton

template<class T> int8_t MGLCD_indirect<T>::WaitForButtons(int8_t b0, int8_t b1, int8_t b2, int8_t b3,
                                                           int8_t b4, int8_t b5, int8_t b6, int8_t b7)
{
  int8_t button[8];
  int8_t LastState[8];
  int8_t CurrentState[8];
  uint8_t WaitButtonNum;
  
  button[0]=b0;
  button[1]=b1;
  button[2]=b2;
  button[3]=b3;
  button[4]=b4;
  button[5]=b5;
  button[6]=b6;
  button[7]=b7;
  
  // Get WaitButtonNum
  WaitButtonNum=0;
  while(WaitButtonNum<8 && button[WaitButtonNum]>=0) WaitButtonNum++; 
  
  for(uint8_t i=WaitButtonNum; i<8; i++) {
    if(button[i]>=0) return -1;
  } // for i
  
  for(uint8_t i=0; i<WaitButtonNum; i++) {
    CurrentState[i]=-1;
  } // for i
  
  while(true) {
    for(uint8_t i=0; i<WaitButtonNum; i++) {
      LastState[i]=CurrentState[i];
      CurrentState[i]=ReadButton(button[i]);
      if(CurrentState[i]<0) return -1;
      if(LastState[i]==MGLCD_BUTTON_OFF && CurrentState[i]==MGLCD_BUTTON_ON) {
        delay(50);
        return button[i];
      } // if
    } // for i;

    delay(50);    
  } // while
} // MGLCD_indirect<T>::WaitForButtons

template<class T> int8_t MGLCD_indirect<T>::Led(uint8_t LedNo, uint8_t state)
{
  uint8_t result;
  
  command[0]=SCLed;
  command[1]=LedNo;
  command[2]=state;
  result=SendCommand();
  if(result!=0) return result;
  return CommandResult[0];
} // MGLCD_indirect<T>::Led

template<class T> size_t MGLCD_indirect<T>::write(uint8_t ch)
{
  PutChar(ch);
  return 1;
} // MGLCD_indirect<T>::write

#endif // #ifdef MGLCD_INDIRECT_H
