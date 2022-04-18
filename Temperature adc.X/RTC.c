/*
 * PIC18F4550 interfacing with RTC DS1307
 * http://www.electronicwings.com
 */ 


#include <stdio.h>
#include "Configs.h"
#include "LCD.h"
#include "I2C_Master.h"
#include "RTC.h"

int sec=0,min=0,hour=0;
int Day=0,Date=0,Month=0,Year=0;

void RTC_Read_Clock(char read_clock_address)
{
    I2C_Start(device_id_write);
    I2C_Write(read_clock_address);     /* address from where time needs to be read*/
    I2C_Repeated_Start(device_id_read);
    sec = I2C_Read(0);                 /*read data and send ack for continuous reading*/
    min = I2C_Read(0);                 /*read data and send ack for continuous reading*/
    hour= I2C_Read(1);                 /*read data and send nack for indicating stop reading*/
    
}

void RTC_Read_Calendar(char read_calendar_address)
{   
    I2C_Start(device_id_write);
    I2C_Write(read_calendar_address); /* address from where time needs to be read*/
    I2C_Repeated_Start(device_id_read);
    Day = I2C_Read(0);                /*read data and send ack for continuous reading*/
    Date = I2C_Read(0);               /*read data and send ack for continuous reading*/
    Month = I2C_Read(0);              /*read data and send ack for continuous reading*/
    Year = I2C_Read(1);               /*read data and send nack for indicating stop reading*/
    I2C_Stop();
}

void RTC_Clock_Write(char sec, char min, char hour, char AM_PM)					/* function for clock */
{
    hour = (hour | AM_PM);     /* whether it is AM or PM */
	I2C_Start(device_id_write);/* start I2C comm. with device slave address*/	
	I2C_Write(0);			   /* write 0 location for sec value */
	I2C_Write(sec);			   /* write second value on 00 location */
	I2C_Write(min);			   /* write min value on 01 location */
	I2C_Write(hour);		   /* write hour value on 02 location */
	I2C_Stop();				   /* stop I2C communication */
}

void RTC_Calendar_Write(char day, char date, char month, char year)	/* function for calendar */
{
	I2C_Start(device_id_write);/* start I2C comm. with device slave address */
	I2C_Write(3);			   /* write on 3 location for day value */
	I2C_Write(day);			   /* write day value on 03 location */
	I2C_Write(date);		   /* write date value on 04 location */
	I2C_Write(month);		   /* write month value on 05 location */
	I2C_Write(year);		   /* write year value on 06 location */
	I2C_Stop();
}


