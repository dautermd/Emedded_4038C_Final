#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

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

extern unsigned int photo_result;

void PhotoResistor_Init(void);
void ReadPhoto(void);

#endif