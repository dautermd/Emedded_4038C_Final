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
//#include "DisplayClock.h"


#define device_id_write 0xD0
#define device_id_read 0xD1

#define MAX_COUNT 3
#define MODE_CLOCK 1
#define MODE_TEMPERATURE 2
unsigned short result; 

int sec=0,min=0,hour=0;
int Day=0,Date=0,Month=0,Year=0;
char secs[10],mins[10],hours[10];
char date[10],month[10],year[10];
char Clock_type = 0x06;
char AM_PM = 0x05;
char days[7] = {'S','M','T','W','t','F','s'};

void displayClock() 
{
 


            RTC_Read_Clock(0);              /*gives second,minute and hour*/
            I2C_Stop();
            if(hour & (1<<Clock_type)){     /* check clock is 12hr or 24hr */

                if(hour & (1<<AM_PM)){      /* check AM or PM */
                    LCD_Set_Cursor('1','14'); 
                    LCD_Write_String("PM");
                }
                else{
                    LCD_Set_Cursor('1','14'); 
                    LCD_Write_String("AM");
                }

                hour = hour & (0x1f);
                sprintf(secs,"%x ",sec);   /*%x for reading BCD format from RTC DS1307*/
                sprintf(mins,"%x:",min);    
                sprintf(hours,"Tim:%x:",hour);  
                LCD_Write_String(hours);            
                LCD_Write_String(mins);
                LCD_Write_String(secs);
            }
            else{

                hour = hour & (0x3f);
                sprintf(secs,"%x ",sec);   /*%x for reading BCD format from RTC DS1307*/
                sprintf(mins,"%x:",min);    
                sprintf(hours,"Tim:%x:",hour);  
                LCD_Write_String(hours);            
                LCD_Write_String(mins);
                LCD_Write_String(secs); 
            }

            RTC_Read_Calendar(3);        /*gives day, date, month, year*/        
            I2C_Stop();
            sprintf(date,"Cal %x-",Date);
            sprintf(month,"%x-",Month);
            sprintf(year,"%x ",Year);
            LCD_Set_Cursor('2','0'); 
            LCD_Write_String(date);
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
            
           
        
    //put display code for clock here
}

void LEDControl()
{
    //put LED control code here
}
 
void interface(){
    
    TRISB = 0b00110000; //Set trise bit otherwise, will not reset after press
    ANSEL = 0;
    ANSELH = 0x00;
    PORTB = 0b00110000; //init PORTB as outputs except RB5 as input (button)
    TRISE = 0x0;
    
    I2C_Init();
    LCD_Init();
    
    while(1){
        
        int displayMode = 0;

        
        LEDControl(); 
        while(displayMode == 0)
        {
            RB0 = 1;
            char buffer3[16]; 
            LCD_Clear(); 
            sprintf(buffer3, "%s", "Hello There");
            LCD_Write_String(buffer3);
            __delay_ms(100); 

        
            if(RE3 == 0 && RB5 == 0)
            {
                while(RE3== 0 && RB5 == 0);
                displayMode = displayMode+1; 
            }
        }
        //using this mode to test the photo resistor; 
        while(displayMode == MODE_CLOCK)
        {
            LCD_Clear();
            RB0=1; 
            displayClock(); 
            if(RE3 == 0 && RB5 == 0)
            {
                while(RE3== 0 && RB5 == 0);
                displayMode = displayMode+1; 

            }
        }
        while(displayMode == MODE_TEMPERATURE)
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
            Display();  //See Temp.C
            if(RE3 == 0 && RB5 == 0)
            {
                while(RE3== 0 && RB5 == 0);
                displayMode = displayMode+1; 
            }
        }
        
       
        
    }
    
}

void main(void){ 

    interface();

    return;
}