#ifndef LED_H
#define LED_H

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


#define send(b) DATA=1; NOP(); NOP(); NOP(); DATA=b; NOP(); NOP(); NOP(); NOP(); DATA=0; NOP(); NOP(); NOP(); NOP();

void LED_Init(void);
void LED_Color(unsigned char brt);
void sendByte (unsigned char b);
void sendRGB (unsigned char r, unsigned char g, unsigned char b);

#endif