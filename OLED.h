/* Testing code for driving UG2828GDEAF01 1,5# OLED
 ZhangFeng, PRC.
 zjjszhangf@gmail.com
 http://vfdclock.jimdo.com
 09-12-2013
 */
#include <stdio.h>
#include <math.h>
#include <Arduino.h>

#define FASTIO 0
// 4-wire SPI(BS1=0; BS2=0)
#define RST_PIN 8
#define CS_PIN  9
#define DC_PIN 10
#define DIN_PIN 11
#define CLK_PIN 12
#define HVEN_PIN 13

#define DODELAY __asm__("nop\n\t""nop\n\t");
#define COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))

#ifndef FASTIO
// rst
#define OLED_RES_1 digitalWrite(RST_PIN, HIGH)
#define OLED_RES_0 digitalWrite(RST_PIN, LOW)
// cs
#define OLED_CS_1 digitalWrite(CS_PIN, HIGH)
#define OLED_CS_0 digitalWrite(CS_PIN, LOW)
// DC (Data or Command)
#define OLED_DC_1 digitalWrite(DC_PIN, HIGH)
#define OLED_DC_0 digitalWrite(DC_PIN, LOW)
// data in
#define OLED_DIN_1 digitalWrite(DIN_PIN, HIGH)
#define OLED_DIN_0 digitalWrite(DIN_PIN, LOW)
// clock
#define OLED_CLK_1 digitalWrite(CLK_PIN, HIGH)
#define OLED_CLK_0 digitalWrite(CLK_PIN, LOW)

// HVEN(optional)
#define OLED_HVEN_1 digitalWrite(HVEN_PIN, HIGH)
#define OLED_HVEN_0 digitalWrite(HVEN_PIN, LOW)
#else
// Fast IO version
// pin map please check: http://arduino.cc/en/Hacking/Atmega168Hardware
// pin11 = pb3; pin12 = pb4; pin15 = pb5
#define GPIO_Pin_0                 (0x01)  /* Pin 0 selected */
#define GPIO_Pin_1                 (0x02)  /* Pin 1 selected */
#define GPIO_Pin_2                 (0x04)  /* Pin 2 selected */
#define GPIO_Pin_3                 (0x08)  /* Pin 3 selected */
#define GPIO_Pin_4                 (0x10)  /* Pin 4 selected */
#define GPIO_Pin_5                 (0x20)  /* Pin 5 selected */
#define GPIO_Pin_6                 (0x40)  /* Pin 6 selected */
#define GPIO_Pin_7                 (0x80)  /* Pin 7 selected */

#define RES_PORT PORTB
#define CS_PORT PORTB
#define DC_PORT PORTB
#define DIN_PORT PORTB
#define CLK_PORT PORTB
#define HVEN_PORT PORTB
#define RES_PORTPIN GPIO_Pin_0
#define CS_PORTPIN GPIO_Pin_1
#define DC_PORTPIN GPIO_Pin_2
#define DIN_PORTPIN GPIO_Pin_3
#define CLK_PORTPIN GPIO_Pin_4
#define HVEN_PORTPIN GPIO_Pin_5

#define OLED_RES_1 RES_PORT |= (RES_PORTPIN)
#define OLED_RES_0 RES_PORT &= ~(RES_PORTPIN)
#define OLED_CS_1 CS_PORT |= (CS_PORTPIN)
#define OLED_CS_0 CS_PORT &= ~(CS_PORTPIN)
#define OLED_DC_1 DC_PORT |= (DC_PORTPIN)
#define OLED_DC_0 DC_PORT &= ~(DC_PORTPIN)
#define OLED_DIN_1 DIN_PORT |= (DIN_PORTPIN)
#define OLED_DIN_0 DIN_PORT &= ~(DIN_PORTPIN)
#define OLED_CLK_1 CLK_PORT |= (CLK_PORTPIN)
#define OLED_CLK_0 CLK_PORT &= ~(CLK_PORTPIN)
#define OLED_HVEN_1 HVEN_PORT |= (HVEN_PORTPIN)
#define OLED_HVEN_0 HVEN_PORT &= ~(HVEN_PORTPIN)
#endif

#define Max_Column	0x7f			// 128-1
#define Max_Row		0x7f			// 128-1
#define	Brightness	0x0F

#define MAKECOLOR64k(r,g,b) (unsigned short int)((((unsigned short int)(r)>>3)<<11)|(((unsigned short int)(g)>>2)<<5)|((unsigned short int)(b)>>3))
#define OLED_WHITE MAKECOLOR64k(0xff,0xff,0xff)
#define OLED_YELLOW MAKECOLOR64k(0xff,0xff,0x0)
#define OLED_PURPLE MAKECOLOR64k(0xff,0x0,0xff)
#define OLED_CYAN MAKECOLOR64k(0x0,0xff,0xff)
#define OLED_RED MAKECOLOR64k(0xff,0x0,0x0)
#define OLED_GREEN MAKECOLOR64k(0x0,0xff,0x0)
#define OLED_BLUE MAKECOLOR64k(0x0,0x0,0xff)
#define OLED_BLACK MAKECOLOR64k(0x0,0x0,0x0)
// Line Color - CCCC,CBBB,BBBA,AAAA

#define Get_RED(c)    ((c & (0x0000001f << 11 )) >> 8)
#define Get_GREEN(c)  ((c & (0x0000001f << 5 )) >> 2)
#define Get_BLUE(c)   ((c & (0x0000001f << 0 )) <<3)

class OLED
{
unsigned char lastx;
unsigned char lasty;
unsigned short lastColour;
unsigned short lastFillColour;


void InitIOs(void);
void Init(void);
void Write_Command(unsigned char Data);

void Write_Data(unsigned char Data);
void EnableFill(unsigned char d);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Column_Address(unsigned char a, unsigned char b);

void Set_Row_Address(unsigned char a, unsigned char b);

void Set_Write_RAM(void);



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table(void);





//===============================================================================//
//  Graphic Acceleration(by hardware)
//===============================================================================//
void Draw_Line64k(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned short int col);
void Draw_Rectangle64k(unsigned char xLeft, unsigned char yTop, unsigned char xRight, unsigned char yBottom, unsigned short int colOutline, unsigned short int colFill);

void Draw_Circle64k(unsigned char x, unsigned char y, unsigned char r, unsigned short int colOutline, unsigned short int colFill);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End
//    c: Row Address of Start
//    d: Row Address of End
//    e: BBBBBGGG
//    f: GGGRRRRR
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block64k(unsigned char xLeft, unsigned char yTop, unsigned char xRight, unsigned char yBottom, unsigned short int col);

void Fill_Block64k_Slow(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned short int col);
public:
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Patterns
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_64k_Pattern_BigEndian(unsigned char *Data_Pointer, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

void Show_64k_Pattern_LittleEndia(unsigned char *Data_Pointer, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

void Show_256k_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
OLED(void);



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void uDelay(unsigned char l);

void setup(void);
unsigned short Colour(unsigned char r,unsigned char g,unsigned char b);
void SetColour(unsigned short int col) { lastColour = col; }
void SetColour(unsigned char r,unsigned char g,unsigned char b);
void SetFillColour(unsigned short int FillCol) { lastFillColour = FillCol; }
void SetFillColour(unsigned char r,unsigned char g,unsigned char b);
void Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
void LineTo(unsigned char x,unsigned char y);
void MoveTo(unsigned char x,unsigned char y);
void Circle(unsigned char x,unsigned char y,unsigned char r);
void Rectangle(unsigned char xLeft,unsigned char yTop, unsigned char xRight, unsigned char yBottom);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End
//    c: Row Address of Start
//    d: Row Address of End
//    e: BBBBBGGG
//    f: GGGRRRRR
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char xLeft, unsigned char yTop, unsigned char xRight, unsigned char yBottom);
void Clear_Window(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void SetFill(bool onoff);
/* path is a asciiz string comprised of sets of instructions,
 instruction set,, commas between instructions are optional(space saver!)
 Note, all x,y values are relative to lastx and lasty
    M=MoveTo(rx,ry)
    L=lineTo(rX,rY)
    C=circle(r)
    R=Rectangle(rx2,ry2)
    c=Colour(r,g,b)
    f=FillColour(r,g,b)
    all parameters are text values
    eg DrawPath("M0,0L100,100c40,20,70r40,40");
*/
void DrawPath(char *path);
char *IsRel(char *path,bool &result); // check to see if 'r' or 'R' is next char
char *GetValue(char *src,int &v) const; // helper function for DrawPath (exposed because it's useful)
void putch(char ch);
void putcxy(unsigned char x,unsigned char y,char ch);
void printxy(unsigned char x,unsigned char y,char *str);

};





































