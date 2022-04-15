#ifndef TEMP_H
#define TEMP_H

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "LCD.h"
#include "I2C_Master.h"
#include "Configs.h"

extern unsigned short result;
extern int temperature_c, temperature_f; 
extern int Temp_Mode; // 0 (F), 1 (C))


void Update_Farenheit();
void Update_Celsius();


#endif