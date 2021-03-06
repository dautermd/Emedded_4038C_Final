#ifndef TEMP_H
#define TEMP_H

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

extern unsigned short result;
extern int temperature_c, temperature_f; 
extern int Temp_Mode; // 0 (F), 1 (C))


void Update_Farenheit(void);
void Update_Celsius(void);
void DisplayTemp(void); 


#endif