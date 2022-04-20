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

#define MAX_COUNT_DISPLAY 4

// Display Modes
#define MODE_CLOCK 1
#define MODE_TEMPERATURE 2
#define MODE_SPEAKER 3

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
    
    // Enhanced CCP1 Control Register
    // Select output mode
    CCP1CONbits.CCP1M3 = 1; //  Select Single Output Mode
    CCP1CONbits.CCP1M2 = 1; //  Select Single Output Mode
    CCP1CONbits.P1M = 0b00; // Single output; P1A modulated; P1B, P1C, P1D assigned as port pins    

    // Set duty cycle
    CCPR1L = 0; // 50% Duty Cycle

    // PWM Frequency, 
    PR2 = 0x65; // 19.61 kHz

    // Timer2 Control Register
    T2CONbits.TOUTPS = 0b0000; // Postscalar is 1:16
    T2CONbits.TMR2ON = 1; // timer2 on bit
    T2CONbits.T2CKPS = 0b0; // Prescalar is 16

    // Set global interrupt, and peripheral interrupt flags
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    // PSTRCON set steering
    PSTRCONbits.STRC = 1; // Set P1C as PWM waveform with polarity control from CCPxM<1:0>
    
    I2C_Init();
    LCD_Init();
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
        else if (displayMode == MODE_SPEAKER) {

            ANSELbits.ANS4 = 1; // A4 Analog Input

            // ADC Control Register 0
            ADCON0bits.ADCS = 0b11; // ADC Conversion Clock Select, FRC (internal oscillator)
            ADCON0bits.CHS = 0b0100; // Analog Channel Select, AN4

            // ADC Control Register 1
            ADCON1bits.VCFG1 = 0; // Voltage Ref, VSS
            ADCON1bits.VCFG0 = 0; // Voltage Ref, VDD
            ADCON1bits.ADFM = 1; // ADC Result Format, Right Justified

            // Peripheral Interrupt request register
            PIR1bits.ADIF = 0; // ADC interrupt flag

            // Peripheral Interrupt Enables
            PIE1bits.ADIE = 1; // ADC Interrupt enable bit

            ADCON0bits.ADON = 1; // ADC Enable Bit
            
            ADCON0bits.GO = 1;
            
            unsigned tmp = 0;

            while(ADCON0bits.GO == 1); // Wait till conversion is done

            // Read results, stored in two registers (10 bit resolution)
            tmp = 0x0000;
            tmp = ADRESH;
            tmp = tmp << 8;
            tmp = tmp | ADRESL;

            PIR1bits.ADIF = 0; // ADC interrupt flag
            
            CCPR1L = (tmp / 10); // 50% Duty Cycle
            
            char buffer3[16]; 
            sprintf(buffer3, "Speaker:%d", tmp);
            LCD_String_xy(0,0,buffer3);
            __delay_ms(100); 
            
            if(BUTTON1 == 0 && BUTTON2 == 0)
            {
                while(BUTTON1 == 0 && BUTTON2 == 0);
                displayMode = displayMode+1; 
                CCPR1L = 0;
            }
        }

                
       __delay_ms(10);
       if(displayMode >= MAX_COUNT_DISPLAY)
       {
           displayMode = 0; 
       }
        
    }
    
}
