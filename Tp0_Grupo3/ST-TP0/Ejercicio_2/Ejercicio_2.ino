// Vazquez,Dodino,Schatzyki,Mendelovich
#include "U8g2lib.h"
#include "string.h"
#include "DHT_U.h"
#include "Adafruit_Sensor.h"




U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
 
  u8g2.begin();
  dht.begin();
}

void loop()
{
  printOLED();
  delay(2000);
}

void printOLED(void)
{
  float tem = dht.readTemperature();
  Serial.println(tem);
  char temp[16];
 
  sprintf(temp, "%f", tem);


  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0, 30, "Temperatura: ");
  u8g2.drawStr(0, 60, temp);
  u8g2.drawStr(90, 60, "C");
  u8g2.sendBuffer();
}
