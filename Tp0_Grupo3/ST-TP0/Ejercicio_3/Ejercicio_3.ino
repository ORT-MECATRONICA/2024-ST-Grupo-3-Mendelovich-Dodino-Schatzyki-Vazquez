// Vazquez,Dodino,Schatzyki,Mendelovich
#include "U8g2lib.h"
#include "string.h"
#include "DHT_U.h"
#include "Adafruit_Sensor.h"




U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,  U8X8_PIN_NONE);
#define LED 25
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  u8g2.begin();
  dht.begin();

  pinMode(PIN_LED, OUTPUT);
  
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
  char temp[4];
 
  sprintf(temp, "%f", tem);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0, 12, "Temperatura: ");
  u8g2.drawStr(0, 47, temp);
  u8g2.drawStr(90, 27, "C");
  u8g2.sendBuffer();

  if(tem >= 28)
  {
    digitalWrite(PIN_LED, HIGH);
  }
  else
  {
    digitalWrite(PIN_LED, LOW);
  }
}
