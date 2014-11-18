/* Testing code for driving UG2828GDEAF01 1,5# OLED
 ZhangFeng, PRC.
 zjjszhangf@gmail.com
 http://vfdclock.jimdo.com
 09-12-2013
 */
#include "OLED.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED::uDelay(unsigned char l)
{
  while(l--)
  {
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  }; 
}

void OLED::Write_Command(unsigned char Data)
{
  unsigned char i;
  OLED_CS_0;
  OLED_DC_0;
  for (i=0; i<8; i++)
  {
    OLED_CLK_0;
    //((Data&0x80)>>7)?OLED_DIN_1:OLED_DIN_0;
    if((Data&0x80)>>7)
      OLED_DIN_1;
    else 
      OLED_DIN_0;
    Data = Data << 1;
    uDelay(1);
    OLED_CLK_1;
    uDelay(1);
  }
  OLED_CLK_0;
  OLED_DC_1;
  OLED_CS_1;
}

void OLED::Write_Data(unsigned char Data)
{
  unsigned char i;

  OLED_CS_0;
  OLED_DC_1;
  for (i=0; i<8; i++)
  {
    OLED_CLK_0;
    if((Data&0x80)>>7)
      OLED_DIN_1;
    else 
      OLED_DIN_0;
    Data = Data << 1;
    uDelay(1);
    OLED_CLK_1;
    uDelay(1);
  }
  OLED_CLK_0;
  OLED_DC_1;
  OLED_CS_1;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED::Set_Gray_Scale_Table(void)
{
  Write_Command(0xB8);
  Write_Data(0x01);			// Gray Scale Level 1
  Write_Data(0x03);			// Gray Scale Level 3
  Write_Data(0x05);			// Gray Scale Level 5
  Write_Data(0x07);			// Gray Scale Level 7
  Write_Data(0x0B);			// Gray Scale Level 9
  Write_Data(0x10);			// Gray Scale Level 11
  Write_Data(0x16);			// Gray Scale Level 13
  Write_Data(0x1A);			// Gray Scale Level 15
  Write_Data(0x1F);			// Gray Scale Level 17
  Write_Data(0x24);			// Gray Scale Level 19
  Write_Data(0x29);			// Gray Scale Level 21
  Write_Data(0x2E);			// Gray Scale Level 23
  Write_Data(0x33);			// Gray Scale Level 25
  Write_Data(0x38);			// Gray Scale Level 27
  Write_Data(0x3D);			// Gray Scale Level 29
  Write_Data(0x42);			// Gray Scale Level 31
  Write_Data(0x47);			// Gray Scale Level 33
  Write_Data(0x4C);			// Gray Scale Level 35
  Write_Data(0x51);			// Gray Scale Level 37
  Write_Data(0x56);			// Gray Scale Level 39
  Write_Data(0x5A);			// Gray Scale Level 41
  Write_Data(0x5E);			// Gray Scale Level 43
  Write_Data(0x62);			// Gray Scale Level 45
  Write_Data(0x65);			// Gray Scale Level 47
  Write_Data(0x68);			// Gray Scale Level 49
  Write_Data(0x6B);			// Gray Scale Level 51
  Write_Data(0x6E);			// Gray Scale Level 53
  Write_Data(0x71);			// Gray Scale Level 55
  Write_Data(0x74);			// Gray Scale Level 57
  Write_Data(0x77);			// Gray Scale Level 59
  Write_Data(0x7A);			// Gray Scale Level 61
  Write_Data(0x7D);			// Gray Scale Level 63
}

void OLED::Clear_Window(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
  Write_Command(0x8E);
  Write_Data(a);				// Column Address of Start
  Write_Data(c);				// Row Address of Start
  Write_Data(b);				// Column Address of End
  Write_Data(d);				// Row Address of End
  delay(10); // delay 5ms for its drawing 
}


void OLED::InitIOs(void)
{
  OLED_RES_1;
  OLED_CS_1;
  OLED_CLK_1;
  OLED_DIN_1; 
  OLED_DC_1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED::Init(void)
{
  unsigned char i;

  OLED_RES_0;
  for(i=0;i<200;i++)
  {
    delay(1); // delay
  }
  OLED_RES_1;

  // Display Off (0x00/0x01)
  Write_Command(0xAE);
  Write_Command(0xA0);
  Write_Data(0x74); // [0x74]: 64k Colors Mode; [0xB4]:262k Colors Mode;256 Colors Mode (0x34)
  Write_Command(0xA1);
  Write_Data(0x00);
  Write_Command(0xA2);
  Write_Data(0x80);
  Write_Command(0xA6);
  Write_Command(0xAD);
  Write_Data(0x8E);
  Write_Command(0xB0);
  Write_Data(0x05);
  Write_Command(0xB1);
  Write_Data(0x11);
  Write_Command(0xB3);
  Write_Data(0xF0);
  // gray table
  Set_Gray_Scale_Table();

  Write_Command(0xBB);
  Write_Data(0x1C);
  Write_Data(0x1C);
  Write_Data(0x1C);

  Write_Command(0xBE);
  Write_Data(0x3F);

  Write_Command(0xC1);
  Write_Data(0xDC);
  Write_Data(0xD2);
  Write_Data(0xFF);

  Write_Command(0xC7);
  Write_Data(0x0A);

  Write_Command(0xCA);
  Write_Data(0x7F);

  Write_Command(0xAF);
}

void OLED::EnableFill(unsigned char d)
{
  Write_Command(0x92);
  Write_Data(!!d);				// Default => 0x00
}

//===============================================================================//
//  Graphic Acceleration(by hardware)
//===============================================================================//
void OLED::Draw_Line64k(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned short int col)
{
  Write_Command(0x83);
  Write_Data(x1);				// Column Address of Start
  Write_Data(y1);				// Row Address of Start
  Write_Data(x2);				// Column Address of End
  Write_Data(y2);				// Row Address of End
  Write_Data(col>>8);				// Line Color - CCCCCBBB
  Write_Data(col&0xff);			// Line Color - BBBAAAAA
  delay(1); // delay 5ms for its drawing  
}

void OLED::Draw_Rectangle64k(unsigned char xLeft, unsigned char yTop, unsigned char xRight, unsigned char yBottom, unsigned short int colOutline, unsigned short int colFill)
{
  Write_Command(0x84);
  Write_Data(xLeft);
  Write_Data(yTop);
  Write_Data(xRight);
  Write_Data(yBottom);
  Write_Data(colOutline>>8);				// Line Color - CCCCCBBB
  Write_Data(colOutline&0xff);			// Line Color - BBBAAAAA
  Write_Data(colFill>>8);				// Line Color - CCCCCBBB
  Write_Data(colFill&0xff);			// Line Color - BBBAAAAA
  delay(2); // delay 5ms for its drawing  
}

void OLED::Draw_Circle64k(unsigned char x, unsigned char y, unsigned char r, unsigned short int colOutline, unsigned short int colFill)
{
  Write_Command(0x86);
  Write_Data(x);				// Column Address of Start
  Write_Data(y);				// Row Address of Start
  Write_Data(r);				// Radius
  Write_Data(colOutline>>8);				// Line Color - CCCCCBBB
  Write_Data(colOutline&0xff);			// Line Color - BBBAAAAA
  Write_Data(colFill>>8);				// Fill Color - CCCCCBBB
  Write_Data(colFill&0xff);			// Fill Color - BBBAAAAA
  delay(2); // delay 10ms for its drawing  
}

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
void OLED::Fill_Block64k(unsigned char xLeft, unsigned char yTop, unsigned char xRight, unsigned char yBottom, unsigned short int col)
{
  EnableFill(0x01);
  Draw_Rectangle64k(xLeft, yTop, xRight, yBottom, col, col);
  EnableFill(0x00);  
}
unsigned short OLED::Colour(unsigned char r,unsigned char g,unsigned char b)
{
    return MAKECOLOR64k(r,g,b);
}
void OLED::SetColour(unsigned char r,unsigned char g,unsigned char b)
{
    lastColour = MAKECOLOR64k(r,g,b);
}

void OLED::SetFillColour(unsigned char r,unsigned char g,unsigned char b)
{
    lastFillColour = MAKECOLOR64k(r,g,b);
}

void OLED::Fill_Block64k_Slow(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned short int col)
{
  unsigned char i,j;

  Set_Column_Address(x1,x2);
  Set_Row_Address(y1,y2);	
  Set_Write_RAM();

  for(i=0;i<(y2-y1+1);i++)
  {
    for(j=0;j<(x2-x1+1);j++)
    {
      Write_Data(col>>8);				// Line Color - CCCCCBBB
      Write_Data(col&0xff);			// Line Color - BBBAAAAA
    }
  }  
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED::Set_Column_Address(unsigned char a, unsigned char b)
{
  Write_Command(0x15);
  Write_Data(a);				// Default => 0x00
  Write_Data(b);				// Default => 0x83
}

void OLED::Set_Row_Address(unsigned char a, unsigned char b)
{
  Write_Command(0x75);
  Write_Data(a);				// Default => 0x00
  Write_Data(b);				// Default => 0x83
}

void OLED::Set_Write_RAM(void)
{
  Write_Command(0x5C);			// Enable MCU to Write into RAM
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Patterns
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED::Show_64k_Pattern_BigEndian(unsigned char *Data_Pointer, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  unsigned char *Src_Pointer;
  unsigned char i,j;

  Src_Pointer=Data_Pointer;
  Set_Column_Address(x1,x2);
  Set_Row_Address(y1,y2);
  Set_Write_RAM();

  for(i=0;i<(y2-y1+1);i++)
  {
    for(j=0;j<(x2-x1+1);j++)
    {
      Write_Data(*Src_Pointer);
      Src_Pointer++;
      Write_Data(*Src_Pointer);
      Src_Pointer++;
    }
  }
}

void OLED::Show_64k_Pattern_LittleEndia(unsigned char *Data_Pointer, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  unsigned char *Src_Pointer;
  unsigned char i,j;

  Src_Pointer=Data_Pointer;
  Set_Column_Address(x1,x2);
  Set_Row_Address(y1,y2);	
  Set_Write_RAM();

  for(i=0;i<(y2-y1+1);i++)
  {
    for(j=0;j<(x2-x1+1);j++)
    {
      Write_Data(*(Src_Pointer+1));
      Write_Data(*(Src_Pointer));
      Src_Pointer+=2;
    }
  }
}

void OLED::Show_256k_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
  unsigned char *Src_Pointer;
  unsigned char i,j;

  Src_Pointer=Data_Pointer;
  Set_Column_Address(a,b);
  Set_Row_Address(c,d);	
  Set_Write_RAM();

  for(i=0;i<(d-c+1);i++)
  {
    for(j=0;j<(b-a+1);j++)
    {
      Write_Data(*Src_Pointer);
      Src_Pointer++;
      Write_Data(*Src_Pointer);
      Src_Pointer++;
      Write_Data(*Src_Pointer);
      Src_Pointer++;
    }
  }
}



void OLED::Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
    if (x2>Max_Column) x2 = Max_Column;
    if (y2>Max_Row) y2 = Max_Row;
    Draw_Line64k(x1,y1,x2,y2,lastColour);
    lastx = x2;
    lasty = y2;
}
void OLED::SetFill(bool onoff)
{
    EnableFill(onoff ?  0x01 : 0x00);
}
char *OLED::GetValue(char *src,int &v) const
{
v = 0;
int m = 1;
    while(*src==' ') src++;
    if (*src=='-')  {
            src++;
            m = -1;
    }
    while(*src>='0' && *src<='9')   {
        v *= 10;
        v += *src - '0';
        src++;
    }
    v *=m; // set sign
    if (*src==',') src++;
    return src;
}
void diag(char *text,int v1)
{
Serial.print(text); Serial.print("("); Serial.print(v1); Serial.println(")");
}
void diag(char *text,int v1,int v2)
{
Serial.print(text); Serial.print("("); Serial.print(v1); Serial.print(","); Serial.print(v2); Serial.println(")");
}
void diag(char *text,int v1,int v2,int v3)
{
Serial.print(text); Serial.print("("); Serial.print(v1); Serial.print(","); Serial.print(v2); Serial.print(","); Serial.print(v3); Serial.println(")");
}

void diag(char *text,int v1,int v2,int v3,int v4)
{
Serial.print(text); Serial.print("("); 
		Serial.print(v1); Serial.print(","); 
	Serial.print(v2); Serial.print(","); Serial.print(v3); Serial.print(","); Serial.print(v4); Serial.println(")");
}
char *OLED::IsRel(char *path,bool &result) // check to see if 'r' or 'R' is next char
{
    while(*path==' ') path++;
    if (result = (*path=='r' || *path=='R'))
        path++;
    return path;
}

/* path is a asciiz string comprised of sets of instructions,
 instruction set,, commas between instructions are optional(space saver!)
 Note, all x,y values are relative to lastx and lasty
    M=MoveRel(rx,ry)
    m=MoveTo(rx,ry)
    L=lineRel(rX,rY)
    l=lineTo(rX,rY)
    C=circle(r)
    R=Rectangle(rx2,ry2)
    c=Colour(r,g,b)
    f=FillColour(r,g,b)
    w=wipescreen(x1,y1,x2,y2)
    W=WIPESCREEN()
    all parameters are text values
    eg DrawPath("M0,0L100,100c40,20,70r40,40");
*/
void OLED::DrawPath(char *path)
{
    int scale=1;
    bool Rel;
    int v1,v2,v3,v4;
Serial.println(path);
    while(*path)    {
        switch(*path++) {
        case ' ': break;
        case 'm':
        case 'M':
            path = IsRel(path,Rel);
            path= GetValue(path,v1);
            path= GetValue(path,v2);
            if (Rel) {
                diag("MoveRel",lastx+v1*scale,lasty+v2*scale);
                MoveTo(lastx+v1*scale,lasty+v2*scale);
            }
            else {
                diag("MoveTo",v1*scale,v2*scale);
                MoveTo(v1*scale,v2*scale);
            }
            break;
        case 'l':
        case 'L':
            path = IsRel(path,Rel);
            path= GetValue(path,v1);
            path= GetValue(path,v2);
            if (Rel) {
                diag("LineRel",lastx+v1*scale,lasty+v2*scale);
                LineTo((lastx+v1)*scale,(lasty+v2)*scale);
            }
            else {
                diag("LineTo",v1*scale,v2*scale);
                LineTo(v1*scale,v2*scale);
            }
            break;
        case 'C':        
            path= GetValue(path,v1);
            Circle(lastx*scale,lasty*scale,v1*scale);
            diag("Circle",lastx*scale,lasty*scale,v1*scale);
            Serial.println(*path);
            break;
        case 'R':
            path= GetValue(path,v1);
            path= GetValue(path,v2);
            diag("rectangle",lastx*scale,lasty*scale,lastx-v1*scale,lasty-v2*scale);
            Rectangle(lastx*scale,lasty*scale,lastx-v1*scale,lasty-v2*scale);
            break;
        case 'c':
            path= GetValue(path,v1);
            path= GetValue(path,v2);
            path= GetValue(path,v3);
            diag("SetColor",v1,v2,v3);
            SetColour(v1,v2,v3);
            break;
        case 'f':
            path= GetValue(path,v1);
            path= GetValue(path,v2);
            path= GetValue(path,v3);
	    diag("SetFillColor",v1,v2,v3);
            SetFillColour(v1,v2,v3);
            break;
	case 'w':
	    path = GetValue(path,v1);
	    path = GetValue(path,v2);
	    path = GetValue(path,v3);
	    path = GetValue(path,v4);
	    diag("Clear_Window",v1,v2,v3,v4);
	    Clear_Window(v1,v2,v3,v4);
	    break;
	case 'W':
	    diag("Clear_Window",0,0x83,0,0x83);
  		Clear_Window(0x00,0x83,0x00,0x83);
		break;
    case '?':
    default:
            Serial.println("'M(r)'oveto(x,y) 'L(r)'ineto(x,y) 'C'ircle(r),'R'ect(x,y)");
            Serial.println("'c'olour(r,g,b), 'f'illcolour(r,g,b),clear'w'indow(x1,y1,x2,y2), clearAll(W)indow ");
            diag("x,y=",lastx,lasty);
            diag("colour",Get_RED(lastColour),Get_GREEN(lastColour),Get_BLUE(lastColour));
            diag("fillcolour",Get_RED(lastFillColour),Get_GREEN(lastFillColour),Get_BLUE(lastFillColour));
        }

    }
}

void OLED::LineTo(unsigned char x,unsigned char y)
{
    Line(lastx,lasty,x,y);
}

void OLED::MoveTo(unsigned char x,unsigned char y)
{
    if (x>Max_Column) x = Max_Column;
    if (y>Max_Row) y = Max_Row;

    lastx = x;
    lasty = y;
}
void OLED::Circle(unsigned char x,unsigned char y,unsigned char r)
{
    Draw_Circle64k(x,y,r,lastColour,lastFillColour);
}

void OLED::Rectangle(unsigned char xLeft,unsigned char yTop, unsigned char xRight, unsigned char yBottom)
{
    Draw_Rectangle64k(xLeft,yTop,xRight,yBottom,lastColour,lastFillColour);
}
void OLED::Fill_Block(unsigned char xLeft, unsigned char yTop, unsigned char xRight, unsigned char yBottom)
{
    Fill_Block64k(xLeft,yTop,xRight,yBottom,lastFillColour);
}

void OLED::putcxy(unsigned char x,unsigned char  y, char ch)
{
    if (x>Max_Column) x = Max_Column;
    if (y>Max_Row) y = Max_Row;

    lastx = x;
    lasty = y;
    putch(ch);
}

void OLED::putch(char ch)
{
char Width=10;
unsigned char NextX = lastx+Width;
unsigned char NextY = lasty;

    switch(ch) {
    case '.':
            LineTo(lastx+1,NextY);
            break;
    case '_':
           LineTo(NextX,NextY);
           break;

    case 'a':
    case 'A':
            DrawPath("M2,0,L0,7,M1,-1,C2");//M2,2L0-5");break;
            break;
    case 'b': case'B':
        DrawPath("M2,0,L0,-8L4,0,L1,3L-3,0M3,0L1,1,L0,4L-5,0"); break;
    case 'c': case 'C': DrawPath("M8,0L-7,0L0,-7,L7,0"); break;
    case 'd': case 'D': DrawPath("M2,0L-8,0,L4,0,L1,1L0,6L-1,1L-4,0"); break;

    case ' ':
    default:
        break;
    }
    MoveTo(NextX,NextY);
}
void OLED::printxy(unsigned char x,unsigned char y, char *str)
{
    MoveTo(x,y);
    while(*str) {
        putch(*str);
        str++;
    }
}

OLED::OLED() {
}
void OLED::setup(void)
{
  // set the digital pin as output:
  pinMode(RST_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(DC_PIN, OUTPUT);
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(HVEN_PIN, OUTPUT);

  OLED_HVEN_1; //+12V on
  InitIOs();
  Init();
}






































