#include "PhotoResistor.h"

unsigned int photo_result;

void PhotoResistor_Init(void) {
    
    ANSELHbits.ANS13 = 1;
    TRISBbits.TRISB5 = 1;
    
//    
    
}

void ReadPhoto(){
    
    // ADC Control Register 0
    ADCON0bits.ADCS = 0b11; // ADC Conversion Clock Select, FRC (internal oscillator)
    ADCON0bits.CHS = 0b1101; // Analog Channel Select, AN13

    // ADC Control Register 1
    ADCON1bits.VCFG1 = 0; // Voltage Ref, VSS
    ADCON1bits.VCFG0 = 0; // Voltage Ref, VDD
    ADCON1bits.ADFM = 1; // ADC Result Format, Right Justified

    // Peripheral Interrupt request register
    PIR1bits.ADIF = 0; // ADC interrupt flag

    // Peripheral Interrupt Enables
    PIE1bits.ADIE = 1; // ADC Interrupt enable bit

    ADCON0bits.ADON = 1; // ADC Enable Bit   
    
    // Start Conversion;
    ADCON0bits.GO = 1;

    while(ADCON0bits.GO == 1); // Wait till conversion is done

    // Read results, stored in two registers (10 bit resolution)
    photo_result = 0x0000;
    photo_result = ADRESH;
    photo_result = photo_result << 8;
    photo_result = photo_result | ADRESL;
    
    PIR1bits.ADIF = 0; // ADC interrupt flag

}

