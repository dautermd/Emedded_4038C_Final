#ifndef RTC_H
#define RTC_H

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


#define device_id_write 0xD0
#define device_id_read 0xD1
#define hour_12_PM 0x60
#define hour_12_AM 0x40

extern int sec,min,hour,Day,Date,Month,Year;

void RTC_Read_Clock(char read_clock_address);
void RTC_Read_Calendar(char read_calendar_address);
void RTC_Clock_Write(char sec, char min, char hour, char AM_PM);
void RTC_Calendar_Write(char day, char date, char month, char year);

#endif