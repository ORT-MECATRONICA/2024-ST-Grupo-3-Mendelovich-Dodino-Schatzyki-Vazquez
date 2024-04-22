//Schatzyki,Mendelovich,Vazquez y Dodino


#include <Arduino.h>
#include <ESP32Time.h>
#include "time.h"
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define DHTPIN 23
#define DHTTYPE DHT11
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define BOTON_1 34
#define BOTON_2 35
#define RELE 26

int modosPantalla;
int lecturaBot1;
int lecturaBot2;
int tprevio;
float tem = 0;
int GMT = -3;

#define MODO_INIT 0
#define MODO_1 1
#define MODO_2 2
#define MODO_INTERMEDIO1 3
#define MODO_INTERMEDIO2 4
#define S_GMT 5
#define B_GMT 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

void hora(void);
void setup_rtc_ntp(void);

struct tm timeinfo;
ESP32Time rtc;


long unsigned int HORA; 
const char *ntpServer = "south-america.pool.ntp.org";
long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

const char* ssid = "ORT-IoT";
const char* password = "OrtIOTnew22$2";

void setup() {
  Serial.begin(9600);
  pinMode(BOT_1, INPUT_PULLUP);
  pinMode(BOT_2, INPUT_PULLUP);
  dht.begin();
  modosPantalla = MODO_1;
  Serial.println(F("Programa Iniciado"));

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();
  Serial.println("Connecting to Wi-Fi...");
  initWiFi();
  setup_rtc_ntp();
}

void loop() {
  hora();

  tprevio = tem;
  tem = dht.readTemperature();

  Serial.print(F("% Temperatura: "));
  Serial.print(tem);

  if (isnan(tem)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  lecturaBot1 = digitalRead(BOTON_1);
  lecturaBot2 = digitalRead(BOTON_2);

  switch (modosPantalla) {

    case MODO_INIT:

      modosPantalla = MODO_1;

      break;

    case MODO_1:

     
      if (tem != tprevio) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        
        display.setCursor(0, 10);
        display.println("tem:");
        display.setCursor(40,10);
        display.println(t);
        
        display.setCursor(0, 40);
        display.println(&timeinfo, "%H:%M");
        display.display();
      }

      if (lecturaBot1 == LOW && lecturaBot2 == LOW) {
        modosPantalla = MODO_INTERMEDIO1;
      }

      break;

    case MODO_2:
      Serial.println("modo 2");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 40);
      display.println("GMT:");
      display.setCursor(25, 40);
      display.println(GMT);
      display.display();



      if (lecturaBot1 == LOW) {
        display.clearDisplay();
        modosPantalla = S_GMT;
      }

      if (lecturaBot2 == LOW) {
        display.clearDisplay();
        modosPantalla = B_GMT;
      }

      if (lecturaBot1 == LOW && lecturaBot2 == LOW) {
        modosPantalla = MODO_INTERMEDIO2;
      }

      break;

    case MODO_INTERMEDIO1:
      if (lecturaBot1 == HIGH && lecturaBot2 == HIGH) {
        display.clearDisplay();
        modosPantalla = MODO_2;
      }

      break;

    case MODO_INTERMEDIO2:
      if (lecturaBot1 == HIGH && lecturaBot2 == HIGH) {
        display.clearDisplay();
        modosPantalla = MODO_1;
      }

      break;

    case S_GMT:
      
      if (lecturaBot1 == HIGH) {
        if (gmtOffset_sec < 43200) {
          gmtOffset_sec += 3600;
          GMT += 1;
        }
        display.clearDisplay();
        setup_rtc_ntp();
        modosPantalla = MODO_2;
      }


      if (lecturaBot2 == LOW) {
        modosPantalla = MODO_INTERMEDIO2;
      }

      break;

    case B_GMT:
      if (lecturaBot2 == HIGH) {
        if (gmtOffset_sec > -43200) {
          gmtOffset_sec -= 3600;
          GMT -= 1;
        }<
        display.clearDisplay();
        setup_rtc_ntp();
        modosPantalla = MODO_2;
      }

      if (lecturaBot1 == LOW) {
        modosPantalla = MODO_INTERMEDIO2;
      }

      break;

  }
}


void initWiFi() {
  WiFi.begin(ssid , password );
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
  }
  Serial.println(WiFi.localIP());
  
}

void setup_rtc_ntp(void)
{
 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  timestamp = time(NULL);
  rtc.setTime(timestamp + gmtOffset_sec);
}


void pedir_lahora(void)
{
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("veo la hora del rtc interno ");
    timeinfo = rtc.getTimeStruct(); 
  }
  else
  {
    Serial.print("NTP Time:");
    timestamp = time(NULL);
  }

  return;
}