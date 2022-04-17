#ifndef LED_H
#define LED_H

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "Configs.h"


#define DATA RC3
#define LED RB0
#define SW1 !RB5

#define send(b) DATA=1; NOP(); NOP(); NOP(); DATA=b; NOP(); NOP(); NOP(); NOP(); DATA=0; NOP(); NOP(); NOP(); NOP();

void sendByte (unsigned char b);
void sendRGB (unsigned char r, unsigned char g, unsigned char b);

#endif
