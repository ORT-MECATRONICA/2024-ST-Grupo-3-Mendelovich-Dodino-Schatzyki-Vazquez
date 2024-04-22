//Schatzyki,Mendelovich,Vazquez y Dodino

#include <ESP32Time.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#define TEMPERATURA 23

#define BOT1 34
#define BOT2 35
#define MODOINICIAL 0
#define PONERHORA 1
#define PASAJESET 2
#define CAMBIO 3
#define ESPERA 4

ESP32Time rtc;

int hora = 0;
int minuto = 0;
int estado = MODOINICIAL;
int tiempo;
int temp;
int x;

DHT dht(TEMPERATURA, DHT11);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
char stringtemp[8];
char stringhora[4];
char stringminuto[4];


void setup() {
  Serial.begin(9600);
  u8g2.begin();
  dht.begin();
  pinMode(TEMPERATURA, INPUT);
  pinMode(BOT1, INPUT_PULLUP);
  pinMode(BOT2, INPUT_PULLUP);

  rtc.setTime(30, 60, 15, 18, 4, 2024);
}

void loop() {
  maquinaEstado();
}

void maquinaEstado() {

  Serial.println(digitalRead(BOT1));
  Serial.println(digitalRead(BOT2));


  switch (estado) {
    case MODOINICIAL:

      if (digitalRead(BOT1) == 0 && digitalRead(BOT2) == 0) {
        estadoActivo = PASAJESET;
        tiempo = millis();
      }

      hora = rtc.getHour(true);
      minuto = rtc.getMinute();

      temp = dht.readTemperature();
      u8g2.clearBuffer();

      u8g2.setFont(u8g2_font_ncenB08_tr);
      sprintf(stringtemp, "%d", temp);
      u8g2.drawStr(15, 60, "Temp:");
      u8g2.drawStr(70, 60, stringtemp);
      u8g2.drawStr(90, 60, "C");
      u8g2.drawStr(15, 20, "Hora:");
      sprintf(stringhora, "%d", hora);
      u8g2.drawStr(70, 20, stringhora);
      u8g2.drawStr(90, 20, stringminuto);
      u8g2.sendBuffer();
      break;

    case PONERHORA:
      if (digitalRead(BOT1) == 0) {
        x = 0;
        estadoActivo = ESPERA;
        tiempo = millis();
      }
      if (digitalRead(BOT2) == 0) {
        x = 1;
        estadoActivo = ESPERA;
        tiempo = millis();
      }
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr);
      sprintf(stringminuto, "%02d", minuto);
      u8g2.drawStr(15, 50, "Minuto:");
      u8g2.drawStr(70, 50, stringminuto);
      u8g2.drawStr(15, 20, "Hora:");
      sprintf(stringhora, "%02d", hora);
      u8g2.drawStr(70, 20, stringhora);

      u8g2.sendBuffer();  // transfer internal memory to the display

      if (digitalRead(BOT1) == 0 && digitalRead(BOT2) == 0) {
        estadoActivo = CAMBIO;
        rtc.setTime(30, minuto, hora, 18, 4, 2024);
        tiempo = millis();
      }
      break;
    case PASAJESET:
      if (digitalRead(BOT1) == 0 && digitalRead(BOT2) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        estadoActivo = PONERHORA;
      }

      break;
    case CAMBIO:
      if (digitalRead(BOT1) == 0 && digitalRead(BOT2) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        estadoActivo = MODOINICIAL;
      }
      break;
    case ESPERA:
      if (digitalRead(BOT1) == 0 && digitalRead(BOT2) == 0) {
        estadoActivo = CAMBIO;
        tiempo = millis();
      }
      if (digitalRead(BOT2) == 0) {
        tiempo = millis();
      }
      if (digitalRead(BOT1) == 0) {
        tiempo = millis();
      }
      if (millis() - tiempo > 15) {
        if (x == 0) {
          hora++;
        }
        if (hora >= 24) {
          hora = hora - 24;
        }
        if (x == 1) {
          minuto++;
        }
        if (minuto >= 60) {
          minuto = minuto - 60;
        }

        estadoActivo = PONERHORA;
      }
      break;
  }
}