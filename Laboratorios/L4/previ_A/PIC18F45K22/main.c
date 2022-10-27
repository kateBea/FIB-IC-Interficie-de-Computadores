/* 
 * Created:   2022
 * Processor: PIC18F45K22
 * Compiler:  MPLAB XC8
 */

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "config.h"
#include "GLCD.h"
#define _XTAL_FREQ 8000000  

#define LOGIC_HIGH      1     //LOGIC high value  
#define LOGIC_LOW       0     //logic low value
#define CASCADE_DELAY   50
#define CHAR_WIDTH      5
#define CHAR_HEIGHT     8

const char* nom1 = "Victor Cabre";
const char* nom2 = "Hugo Pelayo";
const char* LAB3 = "L4A GLCD";

int y_coordinate = 63;
int x_coordinate = 31;

/*Write s to screen at page "page" starting from column y*/
void writeTxt(byte page, byte y, char * s) {
   int i=0;
   while (*s!='\n' && *s!='\0') 
   {
      putchGLCD(page, y+i, *(s++));
      i++;
   };
}	

void configPIC(void)
{
   ANSELA = 0b00000000; // Set pins for digital I/O
   ANSELB = 0b00000000; // Set pins for digital I/O                  
   ANSELC = 0b00000000; // Set pins for digital I/O                 
   ANSELD = 0b00000000; // Set pins for digital I/O
   ANSELE = 0b00000000; // Set pins for digital I/O   
   	
   TRISD = 0x00;        // Set all pins from PORTD as output
   TRISB = 0x00;        // Set all pins from PORTD as output   

   TRISAbits.RA0 = LOGIC_HIGH; // Set pin as input
   TRISAbits.RA1 = LOGIC_HIGH; // Set pin as input
   TRISAbits.RA2 = LOGIC_HIGH; // Set pin as input
   TRISAbits.RA3 = LOGIC_HIGH; // Set pin as input
   
   PORTD = 0x00;        //Clear LATD
   PORTB = 0x00;        //Clear LATB
}

//str is not empty
void present(const char* str, byte end_page, byte start_col)
{
    for (int i = 0; i < strlen(str); ++i)
    {
        char output = str[i];
        byte current_page = -1;
        byte col2;

        while (++current_page <= end_page && output != ' ')
        {
            putchGLCD(current_page, start_col, output);
            __delay_ms(CASCADE_DELAY);
            col2    =  start_col*CHAR_WIDTH-1;

            if (current_page != end_page) clearGLCD(current_page, current_page, col2, col2+(CHAR_WIDTH));
        }
        ++start_col;
    }
}

void printCoordinates(void)
{   
    char _y[5];
    char _x[5];

    /*Format y_coordinate for output*/
    if (y_coordinate >= 100) sprintf(_y, "%d", y_coordinate);
    else 
    {
        if (y_coordinate >= 10) sprintf(_y, "%d " , y_coordinate);
        else                    sprintf(_y, "%d  ", y_coordinate);
    }

    /*Format x_coordinate for output*/
    if (x_coordinate < 10)  sprintf(_x, "%d ", x_coordinate);
    else                    sprintf(_x, "%d", x_coordinate);

    writeTxt(0, 22, _x);
    writeTxt(1, 22, _y);    
}

void main(void)
{
    configPIC();            //setup PIC18 PORTS
    GLCDinit();		        //Initialize GLCD
    clearGLCD(0,7,0,127);   //Clear screen
    setStartLine(0);        //Set start line
    present(nom1, 5, 6);
    present(nom2, 3, 6);
    present(LAB3, 1, 7);

    __delay_ms(1000);

    clearGLCD(1,5,25,90);   //Clear screen

    bool pressed, can_move, can_go_down;
    pressed     = false;    //boolean to implement button_up rising edge
    can_move    = true;     //boolean to implement button_up rising edge
    can_go_down = false;    //boolean to implement button_down falling edge

    putchGLCD(0, 20, 'x');
    putchGLCD(0, 21, '=');
    putchGLCD(1, 20, 'y');
    putchGLCD(1, 21, '=');

    /*MAIN LOOP*/
    while (true)
    {   
        ClearDot(x_coordinate, y_coordinate);
        /*Top button*/
        if (PORTAbits.RA0 && !pressed) pressed = true;
        else if (pressed && can_move) 
        {
            can_move = false;
            --x_coordinate;
            if (x_coordinate < 0) x_coordinate = 0;

        }
        else if (!PORTAbits.RA0) {pressed = false; can_move = true;}

        /*Bottom button*/
        if (PORTAbits.RA1) can_go_down = true;
        else if (!PORTAbits.RA1 && can_go_down)
        {
	    can_go_down = false;
            ++x_coordinate;
            if (x_coordinate > 63) x_coordinate = 63;
        }

        /*Left Button*/
        if (PORTAbits.RA3) 
        {
            --y_coordinate;
            if (y_coordinate < 0)  y_coordinate = 0;
        }

        /*Right Button*/
        if (PORTAbits.RA2) 
        {
            ++y_coordinate;
            if (y_coordinate > 127)     y_coordinate = 127;
        }
        SetDot(x_coordinate, y_coordinate);
        printCoordinates();
    }
}