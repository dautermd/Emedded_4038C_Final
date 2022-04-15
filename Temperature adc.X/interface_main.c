// PIC16F886 Configuration Bit Settings
// 'C' source line config statements

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = ON       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#define _XTAL_FREQ 20000000 //Crystal Oscillator Frequency


#define MAX_COUNT 3
#define MODE_CLOCK 1
#define MODE_TEMPERATURE 2
unsigned short result; 


void testingPhotoresistor()
{
    // PORTB Tri-State Register
    TRISB = 0b10000000; // Port B is output
    TRISA = 0xff;
    
    // PORTB Register
    PORTA = 0x0;
    PORTB = 0b10000000; // PORT B is < V_IL
    
    // Analog Select Register
    ANSEL = 0x0;
    ANSELbits.ANS0 = 1; // A0 Analog Input
    
    // Analog Select High Register
    ANSELH = 0x0;
        
    // ADC Control Register 0
    ADCON0bits.ADCS = 0b11; // ADC Conversion Clock Select, FRC (internal oscillator)
    ADCON0bits.CHS = 0b0000; // Analog Channel Select, AN0
    
    // ADC Control Register 1
    ADCON1bits.VCFG1 = 0; // Voltage Ref, VSS
    ADCON1bits.VCFG0 = 0; // Voltage Ref, VDD
    ADCON1bits.ADFM = 1; // ADC Result Format, Right Justified
    
    // Peripheral Interrupt request register
    PIR1bits.ADIF = 0; // ADC interrupt flag
    
    // Peripheral Interrupt Enables
    PIE1bits.ADIE = 1; // ADC Interrupt enable bit

    ADCON0bits.ADON = 1; // ADC Enable Bit
    RB0 = 0;
    RB1 = 0; 
    RB3 = 0; 
    RB2 = 1;
        // 4 wait the required acquisition time
    unsigned short photoResult; 
        // Start Conversion
        ADCON0bits.GO = 1;

        while(ADCON0bits.GO == 1); // Wait till conversion is done
        
        // Read results, stored in two registers (10 bit resolution)
        photoResult = 0x0000;
        photoResult = ADRESH;
        photoResult = photoResult << 8;
        photoResult = photoResult | ADRESL;
        
        // Clear all LEDs
        RB0 = 0;
        RB1 = 0;
        RB2 = 0;
        RB3 = 0;
        

        PIR1bits.ADIF = 0; // ADC interrupt flag
       
    

    //test Photoresistor
    
}
void temperature_Farenheit()
{
    // temp is -55 to 125 -55 = 0 v
    //125 = 5 v in Celsius, -67, 257F. 1024 = 5v 0 = 0v 
    //every bit is .32640625 degrees F starting from -67
    //set temp temp = .32640625*result - 67; 
    //Temperature calculation; 
    //used for calculating temp
    unsigned short resultTemp; 
    //A temporary variable to help calculate the temperature
    unsigned short temp;
    //A variable for temperature
    int temperature; 
    
    //Think this is wrong below
    temperature = .32640625*result - 67;
    //write code to display temperature; 
}
void temperature_Celsius()
{
    unsigned short resultTemp; 
    //A temporary variable to help calculate the temperature
    unsigned short temp;
    
    //A variable for temperature
    int temperature; 
   
    temperature = .17578125*result - 55;
    //write code to display temperature 
}
void displayClock() 
{
    RB0 = 0;
    RB1 = 0; 
    RB3 = 0; 
    RB2 = 1;
    //put display code for clock here
}

void displayTemperature()
{
    RB0 = 0;
    RB1 = 0;
    RB2 = 0; 
    RB3 = 1; 
    //put display to lcd code for temperature here
}

void LEDControl()
{
    //put LED control code here
}
 
void interface(){
    
    TRISB = 0b00100000; //Set trise bit otherwise, will not reset after press
    ANSEL = 0;
    ANSELH = 0;
    PORTB = 0b00100000; //init PORTB as outputs except RB5
    TRISE = 0x0;
    
    while(1){
        int displayMode = 0;

        LEDControl(); 
        while(displayMode == 0)
        {
            RB0 = 0;
            RB1 = 0;
            RB2 = 0;
            RB3 = 0; 
            if (RE3 == 0)
            {
                RB0 = 1;
            }
            else if(RE3 == 1)
            {
                RB0 = 0;
            }
            if (RB5 == 0)
            {
                RB1 = 1;
            }
            else if(RB5==1)
            {
                RB1 = 0; 
            }  
            if(RE3 == 0 && RB5 == 0)
            {
                while(RE3== 0 && RB5 == 0);

                displayMode = displayMode+1; 

            }
        }
        //using this mode to test the photo resistor; 
        while(displayMode == MODE_CLOCK)
        {
            

            //displayClock(); 
            if(RE3 == 0 && RB5 == 0)
            {
                while(RE3== 0 && RB5 == 0);
                displayMode = displayMode+1; 

            }
        }
        while(displayMode == MODE_TEMPERATURE)
        {
            testingPhotoresistor(); 
            displayTemperature();
            if(RE3 == 0 && RB5 == 0)
            {
                while(RE3== 0 && RB5 == 0);
                displayMode = displayMode+1; 
            }
        }
        
       
        
    }
    
}

void main(void){ 

    interface();

    return;
}