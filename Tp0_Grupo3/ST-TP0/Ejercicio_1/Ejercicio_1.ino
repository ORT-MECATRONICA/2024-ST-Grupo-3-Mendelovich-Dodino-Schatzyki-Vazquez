// Vazquez,Dodino,Schatzyki,Mendelovich
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() 
{
  Serial.begin(9600);
  u8g2.begin();
}
  
void loop() 
{
    printBMP_OLED();
}

void printBMP_OLED(void)
{
    u8g2.clearBuffer();        
    u8g2.setFont(u8g2_font_ncenB14_tr); 
    u8g2.drawStr(10,30,"Hola mundo");  
    u8g2.sendBuffer();          
}
