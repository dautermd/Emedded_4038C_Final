#include "Temp.h"

unsigned short result = 0;
int temperature_c = 0, temperature_f = 0;
int Temp_Mode = 0; // 0 (F), 1 (C))


void Update_Farenheit()
{
    /* */
    //temp is -55 to 125 -55 = 0 v 125 = 5v;
    //125 = 5 v in Celsius, -67, 257F. 257 = 5v -67 = 0v;
    //every bit is .32640625 degrees F starting from -67;
    //set temp temp = .32640625*result - 67; 
    //Temperature calculation; 
    //used for calculating temp;
    //A temporary variable to help calculate the temperature;
    
    // Start Conversion;
    ADCON0bits.GO = 1;

    while(ADCON0bits.GO == 1); // Wait till conversion is done

    // Read results, stored in two registers (10 bit resolution)
    result = 0x0000;
    result = ADRESH;
    result = result << 8;
    result = result | ADRESL;
    temperature_f = .32640625*result -67; 
    
    PIR1bits.ADIF = 0; // ADC interrupt flag

    temperature_f = .32640625*result - 67;
    
    Temp_Mode = 0;

}

void Update_Celsius()
{
    // Start Conversion
    ADCON0bits.GO = 1;

    while(ADCON0bits.GO == 1); // Wait till conversion is done

    // Read results, stored in two registers (10 bit resolution)
    result = 0x0000;
    result = ADRESH;
    result = result << 8;
    result = result | ADRESL;

    PIR1bits.ADIF = 0; // ADC interrupt flag
    temperature_c = .17578125*result - 55;
    
    Temp_Mode = 1;

    //write code to display temperature 
}