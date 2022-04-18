#ifndef ROTERY_ENCODER_H
#define ROTERY_ENCODER_H

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


//rotary encoder state variables
extern int lastStateCLK; //prev state of rotary encoder clock
//unsigned long lastButton = 0; //debounce rot switch
extern int btnState;

extern unsigned char brightness;
extern int colorIncrement;
//need to add a default color temperature



void RotaryEncoder_Init(void);
void ChangeBrightness(void);
void ChangeColor(void);

#endif