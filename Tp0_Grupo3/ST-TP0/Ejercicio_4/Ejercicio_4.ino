// Vazquez,Dodino,Schatzyki,Mendelovich
#include "U8g2lib.h"
#include "DHT_U.h"
#include "Adafruit_Sensor.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,  U8X8_PIN_NONE);
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int maquina = 0;
int V_UMBRAL = 12;
#define BTN1 34
#define BTN2 35

#define pantalla1 0
#define pantalla2 1
#define cambio1 2
#define cambio2 3

void setup()
{
  Serial.begin(9600);
  u8g2.begin();
  dht.begin();

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);

}

void loop()
{
  float tem = dht.readTemperature();
  Serial.println(tem);
  char temp[16];
  sprintf(temp, "%2.1f", tem);
  Serial.println(temp);
  char umbral[16];
  sprintf(umbral, "%i", V_UMBRAL);

  switch (maquina) {
    case pantalla1:
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(0, 12, "Temperatura: ");
      u8g2.drawStr(5, 27, temp);
      u8g2.drawStr(0, 47, umbral);
      u8g2.drawStr(90, 27, "C");
       u8g2.drawStr(0, 62, "Valor umbral:");
      u8g2.sendBuffer();

      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        maquina = cambio1;
      }
      break;

    case cambio1:
      u8g2.clearBuffer();
      u8g2.sendBuffer();
      if (digitalRead(BTN1) == HIGH && digitalRead(BTN2) == HIGH) {
        maquina = pantalla2;
      }
      break;

    case pantalla2:
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(0, 30, "Valor umbral: ");
      u8g2.drawStr(0, 50, umbral);
      u8g2.sendBuffer();
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        maquina = cambio2;
      }
      while (digitalRead(BTN1) == LOW) {
        if (digitalRead(BTN1) == HIGH) {
          u8g2.clearBuffer();
          u8g2.sendBuffer();
          V_UMBRAL++;
        }
      }
      while (digitalRead(BTN2) == LOW) {
        if (digitalRead(BTN2) == HIGH) {
          u8g2.clearBuffer();
          u8g2.sendBuffer();
          V_UMBRAL--;
        }
      }
      break;

    case cambio2:
      u8g2.clearBuffer();
      u8g2.sendBuffer();
      if (digitalRead(BTN1) == HIGH && digitalRead(BTN2) == HIGH) {
        maquina = pantalla1;
      }
      break;
  }
}
