/* 
 * Created:   2022
 * Processor: PIC18F45K22
 * Compiler:  MPLAB XC8
 */

#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "config.h"
#include "GLCD.h"
#define _XTAL_FREQ 8000000  

#define LOGIC_HIGH          1     //LOGIC high value  
#define LOGIC_LOW           0     //logic low value
#define CHAR_WIDTH          5
#define CHAR_HEIGHT         8
#define MAX_SNAKE_SIZE	    30
#define REFRESH_RATE_DELAY  150

/*LIMITS OF THE GAME ARENA*/
#define LIMIT_LEFT      5
#define LIMIT_RIGHT     120
#define LIMIT_BOTTOM    60
#define LIMIT_TOP       10

/*****************************************************************/
/*                    DEFITIONS OF TYPES                         */
/*****************************************************************/
/*Serpent movement directions*/
enum Direction {UP, DOWN, LEFT, RIGHT, NONE};
/*Starting direction*/
char DIR = NONE;
bool posOk = true;

/*Cell position in the 2D space*/
struct pos { char y; char x;};

/*Define types a cell can hold*/
enum Types {SERPENT, APPLE};

/*Apple design*/
const char apple[] = { 0b00110000, 0b01110000, 0b01111000, 0b01110100, 0b00110001 };

const char* str1 = "Gamer Over";
const char* str2 = "You Won";
const char* str3 = "You Lost";

/*Serpent object*/
struct Serpent
{
    struct pos _body[MAX_SNAKE_SIZE+1];

    char _head_idx; /*Index pointing to current head of serpent in the array*/
    char _tail_idx; /*Index pointing to current tail of serpent in the array*/

    char size;      /*Serpent size*/

};

/*****************************************************************/
/*              DEFITIONS OF SERPENT FUNCTIONS                   */
/*****************************************************************/

/*Increase serpent body size*/
void append(struct Serpent* _serp, struct pos const* target)
{
    /*If setting up serpent body*/
    if (_serp->size == 0)
    {
        _serp->_head_idx = _serp->_tail_idx = 0;
        _serp->_body[_serp->_head_idx] = *target;
        _serp->size += 1;
        return;
    }
     /*Serpent is not empty, increase size*/
    for (char it = _serp->_tail_idx; it <= _serp->_tail_idx; ++it)
        _serp->_body[it + 1] = _serp->_body[it];
    _serp->_body[_serp->_head_idx] = *target;

    _serp->size += 1;
    _serp->_tail_idx += 1;

    if (_serp->size == MAX_SNAKE_SIZE) DIR = NONE;
}

/*Returns true if the poition is insidee the arena*/
bool pos_ok(struct Serpent const* _serp)
{
    /*Serpent reached limits of arena*/
    if ((_serp->_body[_serp->_head_idx].x == LIMIT_TOP) || (_serp->_body[_serp->_head_idx].x == LIMIT_BOTTOM)) return false;
    if ((_serp->_body[_serp->_head_idx].y == LIMIT_LEFT) || (_serp->_body[_serp->_head_idx].y == LIMIT_RIGHT)) return false;

    /*Serpent may only bit body only once it reaches certain lenght*/
    if (_serp->size < 4) return true;

    for (char it = 3; it <= (_serp->_tail_idx); ++it)
        /*Serpent bits body*/
        if (_serp->_body[it].x == _serp->_body[_serp->_head_idx].x
            && _serp->_body[it].y == _serp->_body[_serp->_head_idx].y) return false;

    return true;
   
}

/*Move serpent to the current direction*/
void advance(struct Serpent* _serp)
{
    for (char it = _serp->_head_idx; it <= _serp->_tail_idx; ++it)
        _serp->_body[it+1] = _serp->_body[it];

    switch (DIR)
    {
    case UP:    _serp->_body[_serp->_head_idx].x -= 1; break;
    case DOWN:  _serp->_body[_serp->_head_idx].x += 1; break;
    case RIGHT: _serp->_body[_serp->_head_idx].y += 1; break;
    case LEFT:  _serp->_body[_serp->_head_idx].y -= 1; break;
    }
}


/*****************************************************************/
/*       SETUP PIC18 FOR INPUT/OUTPUT, UI, GLCD AND OTHERS       */
/*****************************************************************/
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

/*Print starting user interface elements*/
void setupUI(void)
{
    for (int i = LIMIT_LEFT; i <= LIMIT_RIGHT; ++i) SetDot(LIMIT_TOP, i);
    for (int i = LIMIT_LEFT; i <= LIMIT_RIGHT; ++i) SetDot(LIMIT_BOTTOM, i);
    for (int i = LIMIT_BOTTOM; i >= LIMIT_TOP; --i) SetDot(i, LIMIT_LEFT);
    for (int i = LIMIT_BOTTOM; i >= LIMIT_TOP; --i) SetDot(i, LIMIT_RIGHT);

    for (int i = 0; i < CHAR_WIDTH; ++i) writeByte(0, LIMIT_LEFT+i, apple[i]);
    const char* str = "apples:";
    writeTxt(0, 3, str);
}

/*Setup start point of serpent and apple*/
void setupGame(struct Serpent* _serp, struct pos* _apple)
{
    /*Spawn apple at random position in the screen*/
    _apple->x = (rand() % ((LIMIT_BOTTOM - LIMIT_TOP - 10) + 1)) + LIMIT_TOP;
    _apple->y = (rand() % ((LIMIT_RIGHT - LIMIT_LEFT - 10) + 1)) + LIMIT_LEFT;

    /*Setup serpent*/
    struct pos start_position;
    start_position.x = 25;
    start_position.y = 65;
    append(_serp, &start_position); 
}

//MAIN FUNCTION
void main(void)
{
    configPIC();            //setup PIC18 PORTS
    GLCDinit();		        //Initialize GLCD
    clearGLCD(0,7,0,127);   //Clear screen
    setStartLine(0);        //Set start line
    setupUI();              //print user interface

    bool pressed;
    bool can_move;
    bool can_go_down;
    pressed     = false;    //boolean to implement button_up rising edge
    can_move    = true;     //boolean to implement button_up rising edge
    can_go_down = false;    //boolean to implement button_down falling edge
   
    struct pos _apple;
    struct Serpent _serp;
    _serp.size = 0;
    setupGame(&_serp, &_apple);
    SetDot(_apple.x, _apple.y);
    SetDot(_serp._body[_serp._head_idx].x, _serp._body[_serp._head_idx].y);


    /*number of apples eaten*/
    int apple_count = 0;

    /*MAIN LOOP*/
    while (true)
    {   
        /*Top button*/
        if (PORTAbits.RA0 && !pressed) pressed = true;
        else if (pressed && can_move) 
        {
            if (DIR != DOWN) DIR = UP;
            can_move = false;

            /*code here*/
        }
        else if (!PORTAbits.RA0) {pressed = false; can_move = true;}

        /*Bottom button*/
        if (PORTAbits.RA1) can_go_down = true;
        else if (!PORTAbits.RA1 && can_go_down)
        {
	       if (DIR != UP) DIR = DOWN;
	       can_go_down = false;

           /*code here*/
        }

        /*Left Button*/
        if (PORTAbits.RA3) 
        {
            if (DIR != RIGHT) DIR = LEFT;

            /*code here*/
        }

        /*Right Button*/
        if (PORTAbits.RA2) 
        {
            if (DIR != LEFT) DIR = RIGHT;

            /*code here*/
        }

        char pos_ret = true;
	    /*advance serpent if no collision detection*/
        if ((pos_ret = pos_ok(&_serp)) && (DIR != NONE))
        {
            if ((_serp._body[_serp._head_idx].x == _apple.x) && ((_serp._body[_serp._head_idx].y == _apple.y)))
            {
                /*apple eaten*/
                append(&_serp, &_apple);
                _apple.x = (rand() % ((LIMIT_BOTTOM - LIMIT_TOP) + 1)) + LIMIT_TOP - 10;
                _apple.y = (rand() % ((LIMIT_RIGHT - LIMIT_LEFT) + 1)) + LIMIT_LEFT - 10;
                ++apple_count;
                writeNum(0, 11, apple_count);
                SetDot(_apple.x, _apple.y);
            }
            else advance(&_serp);

            ClearDot(_serp._body[_serp._tail_idx+1].x, _serp._body[_serp._tail_idx+1].y);
            SetDot(_serp._body[_serp._head_idx].x, _serp._body[_serp._head_idx].y);
            __delay_ms(REFRESH_RATE_DELAY);
        }
        else if ((!pos_ret) && (DIR != NONE))
        {
            /*Game over*/

            /*Print game over (loser) text*/
            for (int i = 1; i < _serp.size; ++i)
                ClearDot(_serp._body[i].x, _serp._body[i].y);
            writeTxt(3, 7, str1);
            writeTxt(4, 8, str3);

        }
        else if ((!pos_ret) && (DIR == NONE))
        {
            /*Game won. Reached max size and pos_ok() == true*/
            for (int i = 0; i < _serp.size; ++i)
                ClearDot(_serp._body[i].x, _serp._body[i].y);
            writeTxt(3, 7, str1);
            writeTxt(4, 8, str2);
            /*Print game over (winner) text*/
        }

    }
}