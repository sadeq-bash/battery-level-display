/*
 * battery_level.c
 *
 * Created: 8/14/2023 2:41:44 AM
 * Author: LENOVO
 */

#include <io.h>
#include <mega16a.h>
#include <alcd.h>
//#include <lcdstk.h>
#include <delay.h>
//---------seven seg nums---------------
char nums[10] = {
0x00, 0x06, 0x5B, 0x4F,
0x66, 0x6D, 0x7D, 0x07,
0x7F, 0x6F};
int k;
typedef unsigned char byte;
//--------------------lcd---------------
flash unsigned char state0[8]={14,27,17,17,17,17,17,31}; //battery charge = 0
flash unsigned char state1[8]={14,27,17,17,17,17,31,31}; //battery charge = 20%
flash unsigned char state2[8]={14,27,17,17,17,31,31,31}; //battery charge = 40%
flash unsigned char state3[8]={14,27,17,17,31,31,31,31}; //battery charge = 60%
flash unsigned char state4[8]={14,27,17,31,31,31,31,31}; //battery charge = 80%
flash unsigned char state5[8]={14,31,31,31,31,31,31,31}; //battery full

unsigned char _0, _1, _2, _3, _4, _5;

/* function used to define user characters */
void define_char(unsigned char flash *pc, unsigned char char_code)
{
    unsigned char i,a;
    a=(char_code<<3) | 0x40;
    for (i=0; i<8; i++) lcd_write_byte(a++,*pc++);
}
void delete_char(int x, int y)
{
    lcd_gotoxy(x,y);
    lcd_putchar(' ');
}
void main(void)
{
    DDRA = 0xFF; //lcd
    DDRB = 0xFF;      //seven segment 
    PORTB = 0xFF;
    //-------------------------------
    DDRD.0 = 0;
    PORTD.0 = 1;
    DDRD.2 = 0;  //INT0
    PORTD.2 = 1;
    DDRD.3 = 0;  //INT1
    PORTD.3 = 1;
    //--------------interrupt setup----------------- 
    SREG = 0x80;   //global interrupt enable
    GICR = 0xC0;   //activate INT0, INT1
    MCUCR = 0x0A;  //falling edge of signal
    //-------------------------------  
    lcd_init(16);
    define_char(state0, _0);
    define_char(state1, _1);
    define_char(state2, _2);
    define_char(state3, _3);
    define_char(state4, _4);
    define_char(state5, _5);
    
    lcd_gotoxy(2, 0);
    lcd_puts("#charger#");
    delay_ms(400);
    
    
while (1)
    {  
        lcd_clear();
        if(PIND.0 == 0)
        {
            lcd_gotoxy(2, 0); 
            lcd_puts("charging off");
            delay_ms(500);
        }  
        else
        {
            lcd_gotoxy(0, 0);
            lcd_puts("charging");
            lcd_gotoxy(14, 0);
            lcd_putchar(_0);
            delay_ms(100);
            lcd_gotoxy(14, 0);//delete_char(9, 0);
            lcd_putchar(_1);
            delay_ms(100);
            lcd_gotoxy(14, 0);//delete_char(9, 0);
            lcd_putchar(_2);
            delay_ms(100);
            lcd_gotoxy(14, 0);//delete_char(9, 0);
            lcd_putchar(_3);
            delay_ms(100);
            lcd_gotoxy(14, 0);//delete_char(9, 0);
            lcd_putchar(_4);
            delay_ms(100);
            lcd_gotoxy(14, 0);//delete_char(9, 0);
            lcd_putchar(_5);
            delay_ms(100); 
           
        }
    // Please write your application code here

    }
}

//interrupt service routines
interrupt [2] void func0(void)
{
   lcd_gotoxy(0,1);
   lcd_puts(" INTERRUPT 0 ");
   for(k=0; k<5; k++)
   {
       PORTB = ~nums[2*k];
       delay_ms(300);
   }
   PORTB = 0xFF;       //common anode
}

interrupt [3] void func1(void)
{
   lcd_gotoxy(0,1);
   lcd_puts(" INTERRUPT 1 ");
   for(k=0; k<5; k++)
   {
       PORTB = ~nums[2*k+1];
       delay_ms(300);
   }
   PORTB = 0xFF;
       
}
// battery level display =
//atmega16a + 2 low level interrrupts and
//a 7seg counter + an alphanumeric lcd 
//special char display
// 

/*LDR R11, 0B11000101