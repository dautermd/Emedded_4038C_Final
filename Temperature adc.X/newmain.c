// PIC16F886 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "LCD.h"
#include "I2C_Master.h"
#include "Configs.h"
#include "Temp.h"



void Display()
{
    char buffer1[16], buffer2[16];

    
    if (Temp_Mode) { // C
        sprintf(buffer1, "Temp:%3d*C", temperature_c);
        LCD_Clear();
        __delay_ms(10);
        LCD_Write_String(buffer1);
        __delay_ms(100);        
        
    }
    else { // F
        sprintf(buffer1, "Temp:%3d*F", temperature_f);
        LCD_Clear();
        __delay_ms(10);
        LCD_Write_String(buffer1);
        __delay_ms(100);        
        
    }
    
    

}


void main(void) {
    
    // PORTB Tri-State Register
    TRISB = 0x0; // Port B is output
    TRISA = 0xff;
    
    // PORTB Register
    PORTA = 0x0;
    PORTB = 0x0; // PORT B is < V_IL
    
    // Analog Select Register
    ANSEL = 0x0;
    // Analog Select High Register
    ANSELH = 0x0;
    ANSELbits.ANS3 = 1; // A0 Analog Input
        
    // ADC Control Register 0
    ADCON0bits.ADCS = 0b11; // ADC Conversion Clock Select, FRC (internal oscillator)
    ADCON0bits.CHS = 0b0011; // Analog Channel Select, AN3
    
    // ADC Control Register 1
    ADCON1bits.VCFG1 = 0; // Voltage Ref, VSS
    ADCON1bits.VCFG0 = 0; // Voltage Ref, VDD
    ADCON1bits.ADFM = 1; // ADC Result Format, Right Justified
    
    // Peripheral Interrupt request register
    PIR1bits.ADIF = 0; // ADC interrupt flag
    
    // Peripheral Interrupt Enables
    PIE1bits.ADIE = 1; // ADC Interrupt enable bit

    ADCON0bits.ADON = 1; // ADC Enable Bit
    
    I2C_Init();
    LCD_Init();
    LCD_Clear();
    __delay_ms(10);
    

    // 4 wait the required acquisition time
    while (1){
        
        Update_Celsius();
          
        Display();  
        
    }
    
    return;
}