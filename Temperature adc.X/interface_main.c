// PIC16F886 Configuration Bit Settings
// 'C' source line config statements


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "LCD.h"
#include "I2C_Master.h"
#include "Configs.h"
#include "Temp.h"
#include "RTC.h"
#include "DisplayClock.h"
#include "Pinout.h"
#include "LED.h"
#include "RotaryEncoder.h"
#include "PhotoResistor.h"

// Display Modes
#define MAX_COUNT_DISPLAY 3
#define MAX_COUNT_LED 2
#define MODE_CLOCK 1
#define MODE_TEMPERATURE 2

// LED Modes
#define MODE_CONTROL 1
#define MODE_AMBIENT 2
unsigned short result;

unsigned short alarmTime;

void main (void){
    
    TRISB = 0b00110000; //Set trise bit otherwise, will not reset after press
    ANSEL = 0;
    ANSELH = 0x00;
    PORTB = 0b00110000; //init PORTB as outputs except RB5 as input (button)
    TRISE = 0x0;
    
    I2C_Init();
    LCD_Init();
    LED_Init();
    RotaryEncoder_Init();
    
    RTC_Clock_Write(0x00, 0x14, 0x10, hour_12_AM);
    RTC_Calendar_Write(0x1, 0x22, 0x04, 0x18);

    short int displayMode = 0; // sets in test mode to begin
    short int ledMode = 1; //sets to manual control to begin
    
    while(1){
        
        //LED Control Modes
        if(ledMode == MODE_CONTROL)
        {
            // Rotary Encoder functions
            ChangeBrightness();
            ChangeColor();
            
            if(BUTTON3 == 0)
            {
                while(BUTTON3 == 0);
                ledMode = ledMode + 1; 
            }
        }
        else if(ledMode == MODE_AMBIENT)
        {
            ReadPhoto();
            
            // TODO: now what?
            
            if(BUTTON3 == 0)
            {
                while(BUTTON3 == 0);
                ledMode = ledMode + 1; 
            }
        }
        
        //LCD Display modes
        if (displayMode == 0)
        {
            char buffer3[16]; 
            LCD_Clear(); 
            sprintf(buffer3, "%s", "Hello There");
            LCD_Write_String(buffer3);
            __delay_ms(100); 
            
            if(BUTTON1 == 0 && BUTTON2 == 0)
            {
                while(BUTTON1 == 0 && BUTTON2 == 0);
                displayMode = displayMode+1; 
            }
        }
        //using this mode to test the photo resistor; 
        else if (displayMode == MODE_CLOCK)
        {
            LCD_Clear();
            printClock(); 
            if(BUTTON1 == 0 && BUTTON2 == 0)
            {
                while(BUTTON1 == 0 && BUTTON2 == 0);
                displayMode = displayMode+1; 

            }
        }
        else if (displayMode == MODE_TEMPERATURE)
        {

            ANSELbits.ANS3 = 1; // A0 Analog Input

            // ADC Control Register 0
            ADCON0bits.ADCS = 0b11; // ADC Conversion Clock Select, FRC (internal oscillator)
            ADCON0bits.CHS = 0b0011; // Analog Channel Select, AN3

            // ADC Control Register 1
            ADCON1bits.VCFG1 = 0; // Voltage Ref, VSS
            ADCON1bits.VCFG0 = 0; // Voltage Ref, VDD
            ADCON1bits.ADFM = 1; // ADC Result Format, Right Justified

            // Peripheral Interrupt request register
            PIR1bits.ADIF = 0; // ADC interrupt flag

            // Peripheral Interrupt Enables
            PIE1bits.ADIE = 1; // ADC Interrupt enable bit

            ADCON0bits.ADON = 1; // ADC Enable Bit

            LCD_Clear();
            Update_Celsius(); // See Temp.c
            Update_Farenheit(); //See Temp.c
            DisplayTemp();  //See Temp.C
            
            if(BUTTON1 == 0 && BUTTON2 == 0)
            {
                while(BUTTON1 == 0 && BUTTON2 == 0);
                displayMode = displayMode+1; 
            }
        }
        
       __delay_ms(10);
       if(displayMode >= MAX_COUNT_DISPLAY)
       {
           displayMode = 0; 
       }
       if(ledMode >= MAX_COUNT_LED)
       {
           ledMode = 1; 
       }
        
    }
    
}
