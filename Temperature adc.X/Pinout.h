#ifndef PINOUT_H
#define PINOUT_H

// Interface
#define BUTTON1 RE3
#define BUTTON2 RB5
#define BUTTON3 RB4

// Encoder
#define Encoder_SW PORTBbits.RB3 //encoder switch
#define Encoder_DT PORTBbits.RB0 //encoder dt
#define Encoder_CLK PORTBbits.RB7 //encoder clock

// LED
#define DATA RC3
#define LED RB0
#define SW1 !RB5




#endif