#include "DisplayClock.h"





void printClock() 
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
    
//    __delay_ms(100);
    
//    LCD_Set_Cursor(1,1);
}
