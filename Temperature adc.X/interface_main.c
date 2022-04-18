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
#include "Pinout.h"
#include "LED.h"
#include "RotaryEncoder.h"
#include "PhotoResistor.h"

#define MAX_COUNT_DISPLAY 6

// Display Modes
#define MODE_CLOCK 1
#define MODE_TEMPERATURE 2
#define MODE_LED 3
#define MODE_AMBIENT 4
#define MODE_SPEAKER 5

unsigned short result;

unsigned short alarmTime;

#define device_id_write 0xD0
#define device_id_read 0xD1

int sec=0,min=0,hour=0;
int Day=0,Date=0,Month=0,Year=0;
char secs[10]={0},mins[10]={0},hours[10]={0};
char date[10]={0},month[10]={0},year[10]={0};
char Clock_type = 0x06;
char AM_PM = 0x05;
char days[7] = {'S','M','T','W','t','F','s'};

void main (void){
    TRISB = 0x0;
    TRISC = 0x0;
    ANSEL = 0x0;
    ANSELH = 0x00;
    
    TRISB = 0b00110000; //Set trise bit otherwise, will not reset after press
    PORTB = 0b00110000; //init PORTB as outputs except RB5 as input (button)
    TRISE = 0x0;
    
    I2C_Init();
    LCD_Init();
    LED_Init();
    RotaryEncoder_Init();
    LCD_Clear();
    __delay_ms(10);
    
    RTC_Clock_Write(0x00, 0x14, 0x10, hour_12_AM);
    RTC_Calendar_Write(0x1, 0x22, 0x04, 0x18);
    
    short int displayMode = 0; // sets in test mode to begin
    
    while(1){
        
        LCD_Clear();
        

        
        //LCD Display modes
        if (displayMode == 0)
        {
            char buffer3[16]; 
            sprintf(buffer3, "%s", "Hello There");
            LCD_String_xy(0,0,buffer3);
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
            RTC_Read_Clock(0);              /*gives second,minute and hour*/
            I2C_Stop();
            if(hour & (1<<Clock_type)){     /* check clock is 12hr or 24hr */

                if(hour & (1<<AM_PM)){      /* check AM or PM */
                    LCD_String_xy(1,13,"PM");
                }
                else{
                    LCD_String_xy(1,13,"AM");
                }

                hour = hour & (0x1f);
                sprintf(secs,"%x ",sec);   /*%x for reading BCD format from RTC DS1307*/
                sprintf(mins,"%x:",min);    
                sprintf(hours,"Tim:%x:",hour);
                LCD_String_xy(0,0,hours);            
                LCD_Write_String(mins);
                LCD_Write_String(secs);
            }
            else{

                hour = hour & (0x3f);
                sprintf(secs,"%x ",sec);   /*%x for reading BCD format from RTC DS1307*/
                sprintf(mins,"%x:",min);    
                sprintf(hours,"Tim:%x:",hour);  
                LCD_String_xy(0,0,hours);            
                LCD_Write_String(mins);
                LCD_Write_String(secs); 
            }

            RTC_Read_Calendar(3);        /*gives day, date, month, year*/        
            I2C_Stop();
            sprintf(date,"Cal:%x-",Date);
            sprintf(month,"%x-",Month);
            sprintf(year,"%x ",Year);
            LCD_String_xy(2,0,date);
            LCD_Write_String(month);
            LCD_Write_String(year);

        /* find day */
            switch(days[Day])
            {
                case 'S':
                        LCD_Write_String("Sun");
                        break;
                case 'M':
                        LCD_Write_String("Mon");
                        break;                
                case 'T':
                        LCD_Write_String("Tue");
                        break;                
                case 'W':   
                        LCD_Write_String("Wed");
                        break;                
                case 't':
                        LCD_Write_String("Thu");
                        break;
                case 'F':
                        LCD_Write_String("Fri");
                        break;                
                case 's':
                        LCD_Write_String("Sat");
                        break;
                default:                         
                        LCD_Write_String("Inv");
                        break;

            }
            
            __delay_ms(10);
            
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

//            LCD_Clear();
            Update_Celsius(); // See Temp.c
            Update_Farenheit(); //See Temp.c
            DisplayTemp();  //See Temp.C
            
            if(BUTTON1 == 0 && BUTTON2 == 0)
            {
                while(BUTTON1 == 0 && BUTTON2 == 0);
                displayMode = displayMode+1; 
            }
        }
        else if (displayMode == MODE_LED) {
            // Rotary Encoder functions
            ChangeBrightness();
    //            ChangeColor();
            
            char buffer3[16]; 
            sprintf(buffer3, "Light:%d", brightness);
            LCD_String_xy(0,0,buffer3);
            __delay_ms(100); 

            if(BUTTON1 == 0 && BUTTON2 == 0)
            {
                while(BUTTON1 == 0 && BUTTON2 == 0);
                displayMode = displayMode+1; 
            }
        }
        
        else if (displayMode == MODE_AMBIENT) {
            ReadPhoto();

            unsigned tmp = 1024 - photo_result;
            
            char buffer3[16]; 
            sprintf(buffer3, "Photo:%d", tmp);
            LCD_String_xy(0,0,buffer3);
            __delay_ms(100); 
            
            // TODO: properly scale
            CCPR1L = tmp;

            if(BUTTON1 == 0 && BUTTON2 == 0)
            {
                while(BUTTON1 == 0 && BUTTON2 == 0);
                displayMode = displayMode+1; 
            }
        }
        else if (displayMode == MODE_SPEAKER) {
            
            // TODO: needs to write to DAC
            
            continue;
            
            ADCON0bits.GO = 1;

            while(ADCON0bits.GO == 1); // Wait till conversion is done

            // Read results, stored in two registers (10 bit resolution)
            result = 0x0000;
            result = ADRESH;
            result = result << 8;
            result = result | ADRESL;
            temperature_f = .32640625*result -67; 

            PIR1bits.ADIF = 0; // ADC interrupt flag
            
            unsigned tmp = 0;
            
            char buffer3[16]; 
            sprintf(buffer3, "Photo:%d", tmp);
            LCD_String_xy(0,0,buffer3);
            __delay_ms(100); 
            
            // TODO: properly scale
            CCPR1L = tmp;

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
        
    }
    
}
