#include "LED.h"


void LED_Init() {
    
    TRISB |= (0 << 1);
    
}

void LED_Color(unsigned char brt) {
    sendRGB(brt,brt,brt); sendRGB(brt,brt,brt); sendRGB(brt,brt,brt);
    sendRGB(brt,0,0); sendRGB(0,brt,0); sendRGB(0,0,brt); sendRGB(brt,brt,0); sendRGB(brt,0,brt); sendRGB(0,brt,brt); sendRGB(brt,brt,brt); sendRGB(0,0,0);
}

// send out a byte b in WS2812 protocol
void sendByte (unsigned char b) {

    if (b & 0b10000000) { send(1); } else { send(0); }
    if (b & 0b01000000) { send(1); } else { send(0); }
    if (b & 0b00100000) { send(1); } else { send(0); }
    if (b & 0b00010000) { send(1); } else { send(0); }
    if (b & 0b00001000) { send(1); } else { send(0); }
    if (b & 0b00000100) { send(1); } else { send(0); }
    if (b & 0b00000010) { send(1); } else { send(0); }
    if (b & 0b00000001) { send(1); } else { send(0); }
    
}

// send red, green, and blue values in WS2812 protocol
void sendRGB (unsigned char r, unsigned char g, unsigned char b) {

    sendByte(g);
    sendByte(r);
    sendByte(b);
    
}