#ifndef DISPLAY_CLOCK_H
#define DISPLAY_CLOCK_H

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

extern int sec,min,hour;
extern int Day,Date,Month,Year;
extern char secs[10],mins[10],hours[10];
extern char date[10],month[10],year[10];
extern char Clock_type;
extern char AM_PM;
extern char days[7];


void printClock(void);  


#endif