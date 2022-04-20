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
    temperature_f = 1.265625*result -67; 
    
    PIR1bits.ADIF = 0; // ADC interrupt flag
    


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
    temperature_c = .7*result - 55;
    
    Temp_Mode = 1;

    //write code to display temperature 
}
void DisplayTemp()
{
    char buffer1[16], buffer2[16];
    sprintf(buffer1, "Temp:%3d*C", temperature_c);
    __delay_ms(1); //necessary otherwise LCD doesn't display at all
    LCD_Write_String(buffer1);       
    LCD_Set_Cursor('2','0'); 
    sprintf(buffer2, "Temp:%3d*F", temperature_f);
    LCD_Write_String(buffer2);
    __delay_ms(200);        
        
}