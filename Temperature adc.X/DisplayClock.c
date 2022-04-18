#include "DisplayClock.h"

#define device_id_write 0xD0
#define device_id_read 0xD1

int sec=0,min=0,hour=0;
int Day=0,Date=0,Month=0,Year=0;
char secs[10],mins[10],hours[10];
char date[10],month[10],year[10];
char Clock_type = 0x06;
char AM_PM = 0x05;
char days[7] = {'S','M','T','W','t','F','s'};

void printClock() 
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
        LCD_Set_Cursor('1', '0');
        LCD_Write_String(hours);            
        LCD_Write_String(mins);
        LCD_Write_String(secs);
    }
    else{

        hour = hour & (0x3f);
        sprintf(secs,"%x ",sec);   /*%x for reading BCD format from RTC DS1307*/
        sprintf(mins,"%x:",min);    
        sprintf(hours,"Tim:%x:",hour); 
        LCD_Set_Cursor('1','0');
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
    __delay_ms(100); 
}
