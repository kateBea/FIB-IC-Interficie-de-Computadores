/* Main.c file generated by New Project wizard
 *
 * Created:   Sat Mar 12 2022
 * Processor: PIC18F45K22
 * Compiler:  MPLAB XC8
 */
#include <xc.h>
#include <stdbool.h>
#include "config.h"

#define LOGIC_HIGH 1
#define LOGIC_LOW 0
#define _XTAL_FREQ 8000000

#define DIS0 PORTAbits.RA0
#define DIS1 PORTAbits.RA1
#define DIS2 PORTAbits.RA2
#define DIS3 PORTAbits.RA3

enum status {START, STOP};

const char NUMBERS[] = {0b00111111, 0b00000110,
                        0b01011011, 0b01001111,
                        0b01100110, 0b01101101,
                        0b01111101, 0b00000111,
                        0b01111111, 0b01101111};

//Print number to 7SEG displays
//pointPos > 0
void imprimir_segments(int number, bool is_decimal, char pointPos)
{   
    for (int currDisplay = 0; currDisplay < 4; ++currDisplay)
    {
        int value = number % 10;
        //If decimal NUMBERS[] or 0b10000000 (8th bit is dp bit)
        //Otherwise just print value loaded from array
        if (is_decimal)
        {
            if (currDisplay == pointPos) LATD = NUMBERS[value] | 0x80;
            else LATD = NUMBERS[value];
        } 
        else LATD = NUMBERS[value];

        switch (currDisplay)
        {
            case 0: DIS0 = LOGIC_HIGH; break;
            case 1: DIS1 = LOGIC_HIGH; break;
            case 2: DIS2 = LOGIC_HIGH; break;
            case 3: DIS3 = LOGIC_HIGH; break;
        }
    
        __delay_us(500);

        DIS0 = DIS1 = DIS2 = DIS3 = 0x0;
        number = number / 10;
    }
}

//Muestra digito "digit" por la pantalla
//"display" del set de pantallas de 7-Segmentos
void displayDigit(char display, int digit)
{
        //If decimal NUMBERS[] or 0b10000000 (8th bit is dp bit)
        //Otherwise just print value loaded from array
        LATD = NUMBERS[digit];

        switch (display)
        {
            case 0: DIS0 = LOGIC_HIGH; break;
            case 1: DIS1 = LOGIC_HIGH; break;
            case 2: DIS2 = LOGIC_HIGH; break;
            case 3: DIS3 = LOGIC_HIGH; break;
        }
    
        __delay_us(50);

        DIS0 = DIS1 = DIS2 = DIS3 = 0x0;
}

//Muestra cuatro cifras de menor peso
//de un número decimal por pantalla 
//de 7-Segmentos
void displayNumber(int number)
{
    for (int currDisplay = 0; currDisplay < 4; ++currDisplay)
    {
        int value = number % 10;
        displayDigit(currDisplay, value);
        number = number / 10;
    }
}

// setup PORTs for I/O
void config_PIC(void)
{
    ANSELA = 0b00001111; // Set pins for digital I/O
    ANSELB = 0b00001111; // Set pins for digital I/O
    ANSELD = 0x11111111; // Set pins for digital I/O
    ANSELE = 0b00000111; // Set pins for digital I/O

    TRISBbits.RB0 = LOGIC_HIGH; // Set pin as input
    TRISBbits.RB1 = LOGIC_HIGH; // Set pin as input
    TRISBbits.RB2 = LOGIC_HIGH; // Set pin as input
    TRISBbits.RB3 = LOGIC_HIGH; // Set pin as input

    TRISAbits.RA0 = LOGIC_LOW; // Set pin as output
    TRISAbits.RA1 = LOGIC_LOW; // Set pin as output
    TRISAbits.RA2 = LOGIC_LOW; // Set pin as output
    TRISAbits.RA3 = LOGIC_LOW; // Set pin as output

    TRISEbits.RE0 = LOGIC_HIGH; // Set pin as input
    TRISEbits.RE1 = LOGIC_HIGH; // Set pin as input
    TRISEbits.RE2 = LOGIC_HIGH; // Set pin as input

    TRISD = 0x00; // Set all pins from PORTD as output
    LATD = 0x00; //Print 0 at all displays at start
    LATA = 0x00; //7SEG displays are turned off at start
}



void main(void)
{
    config_PIC();
    bool is_pressed = false;
    bool can_add = true;
    bool RB2_pressed = false;
    int contador = 0;
    // Write your code here
    while (1)
    {   
        //Previ: Funcionalitat apartat 3.1
        if (PORTBbits.RB0) {
            ++contador;
            if (contador > 9999) contador = 0;
        }

        //Previ: Funcionalitat apartat 3.2
        if (PORTBbits.RB1) is_pressed = true;
        else
        {
            is_pressed = false;
            can_add = true;
        }
        if (can_add && is_pressed) {
            can_add = false;
            ++contador;
        }

        //Previ: Funcionalitat apartat 3.3
        if (PORTBbits.RB2) RB2_pressed = true;
        else if (!PORTBbits.RB2 && RB2_pressed)
        {
            RB2_pressed = false;
            --contador;
            if (contador < 0) contador = 0;
        }

        imprimir_segments(contador, false);
    }
}