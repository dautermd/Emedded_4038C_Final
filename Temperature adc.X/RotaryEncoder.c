#include "RotaryEncoder.h"

//rotary encoder state variables
int lastStateCLK; //prev state of rotary encoder clock
//unsigned long lastButton = 0; //debounce rot switch
int btnState;

unsigned char brightness = 20;
int colorIncrement = 2;
//need to add a default color temperature

void RotaryEncoder_Init(){
    
    T2CON = 0x04; //No pre/post-scaler config
    PR2 = 101; //PWM Freq/Cycle time
    
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB7 = 1;
    
    PSTRCONbits.STRB = 1; //Steering Enable bit B
    PSTRCONbits.STRC = 1; //Steering Enable bit C
    PSTRCONbits.STRD = 1; //Steering Enable bit D
    
    //P1C = RB1, P1B = RB2, P1D = RB4
    CCP1CONbits.P1M = 0x00; //Single output; P1A modulated; P1B, P1C, P1D assigned as port pins
    CCP1CONbits.CCP1M = 0b1100; //Configure P1B, P1C, & P1D (RGB LEDs)
        
}

void ChangeBrightness(){
    //initial conditions
    lastStateCLK = Encoder_CLK;
    CCPR1L = brightness; 
    
    if(Encoder_CLK != lastStateCLK && Encoder_CLK == 1){
        //counter clockwise
        if(Encoder_DT != Encoder_CLK && brightness > 0){
            brightness = brightness - 10;
            CCPR1L = brightness;
        }
        //clockwise
        else if(Encoder_DT == Encoder_CLK && brightness < 100){
           brightness = brightness + 10;
           CCPR1L = brightness;
        }
    }
        
    lastStateCLK = Encoder_CLK;
    
}

void ChangeColor(){
    
    LED_Color(0);

    return;
   
    btnState = Encoder_SW;

    if (Encoder_SW == 1){ // Button press
        while(Encoder_SW == 1); // Delay
        switch(colorIncrement){
            case 1: // Coldest color temp
                CCPR1L = 100;
                colorIncrement++;
                break;
            case 2:
                CCPR1L = 80;
                colorIncrement++;
                break;
            case 3:
                CCPR1L = 60;
                colorIncrement++;
                break;
            case 4:
                CCPR1L = 20;
                colorIncrement++;
                break;
            case 5: // Warmest color temp
                CCPR1L = 0;
                colorIncrement = 1;
                break;
            default:
                colorIncrement = 1; // Prevent unknown state
                break;
        }
    }    
    
    
}

