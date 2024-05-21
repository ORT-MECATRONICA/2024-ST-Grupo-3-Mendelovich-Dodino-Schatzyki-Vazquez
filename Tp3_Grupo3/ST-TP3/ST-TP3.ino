#include <U8g2lib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
#define DHTPIN 23
#define BOT1 35
#define BOT2 34
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

float tempActual = 0;
char stringTempActual[5];
int valUmbral = 29;
char stringValUmbral[5];

bool aux = false;

unsigned long tiempo;
unsigned long tiempoUltimoCambio;

const char* ssid = "sacho";
const char* password = "123456789";

#define BOTtoken "6773083685:AAFZjtzHz-dkzsIiud_QzTycN7rrS7jnI94"
#define CHAT_ID "6058289546"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Botdelay = 5000;
unsigned long lastTimeBotRan;
int numNewMessages;

#define MODO1 0
#define MODO2 1
#define INTERMEDIO1 2
#define INTERMEDIO2 3
#define INTERMEDIO3 4
#define INTERMEDIO4 5
#define INTERMEDIO5 6
#define INTERMEDIO6 7
int estado = 0;

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  dht.begin();
  pinMode(BOT1, INPUT);
  pinMode(BOT2, INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado.");
  bot.sendMessage(CHAT_ID, "Bot iniciado", "");
}

void loop() {
  if (millis() > lastTimeBotRan + Botdelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("Mensaje");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  tiempo = millis();
  tempActual = dht.readTemperature();
  maquina();
}

void maquina() {
  switch (estado) {
    case MODO1:
      displayTempUmbral();
      if (tempActual > valUmbral && aux == false) {
        bot.sendMessage(CHAT_ID, "ALERTA SUPERO LA TEMPERATURA UMBRAL");
        aux = true;
      }
      if (tempActual < valUmbral) {
        aux = false;
      }
      if (digitalRead(BOT1) == LOW) {
        estado = INTERMEDIO1;
        tiempoUltimoCambio = tiempo;
      }
      break;

    case MODO2:
      displayUmbral();
      if (digitalRead(BOT1) == LOW) {
        estado = INTERMEDIO4;
      }
      if (digitalRead(BOT2) == LOW) {
        estado = INTERMEDIO5;
      }
      break;

    case INTERMEDIO1:
      if (tiempo - tiempoUltimoCambio >= 5000) {
        estado = MODO1;
      }
      if (digitalRead(BOT2) == LOW && digitalRead(BOT1) == HIGH) {
        estado = INTERMEDIO2;
        tiempoUltimoCambio = tiempo;
      }
      break;

    case INTERMEDIO2:
      if (tiempo - tiempoUltimoCambio >= 5000) {
        estado = MODO1;
      }
      if (digitalRead(BOT1) == LOW && digitalRead(BOT2) == HIGH) {
        estado = INTERMEDIO3;
        tiempoUltimoCambio = tiempo;
      }
      break;

    case INTERMEDIO3:
      if (tiempo - tiempoUltimoCambio >= 5000) {
        estado = MODO1;
      }
      if (digitalRead(BOT1) == HIGH) {
        estado = MODO2;
      }
      break;

    case INTERMEDIO4:
      if (digitalRead(BOT1) == HIGH) {
        valUmbral = valUmbral + 1;
        estado = MODO2;
      } else if (digitalRead(BOT2) == LOW) {
        estado = INTERMEDIO6;
      }
      break;

    case INTERMEDIO5:
      if (digitalRead(BOT2) == HIGH) {
        valUmbral = valUmbral - 1;
        estado = MODO2;
      } else if (digitalRead(BOT1) == LOW) {
        estado = INTERMEDIO6;
      }
      break;

    case INTERMEDIO6:
      if (digitalRead(BOT1) == HIGH && digitalRead(BOT2) == HIGH) {
        estado = MODO1;
      }
      break;
  }
}

void displayTempUmbral() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0, 15, "Temp: ");
  sprintf(stringTempActual, "%.2f", tempActual);
  u8g2.drawStr(65, 15, stringTempActual);

  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0, 40, "Umbral: ");
  sprintf(stringValUmbral, "%.2d", valUmbral);
  u8g2.drawStr(85, 40, stringValUmbral);

  u8g2.sendBuffer();
}

void displayUmbral() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0, 30, "Umbral: ");
  sprintf(stringValUmbral, "%.2d", valUmbral);
  u8g2.drawStr(85, 30, stringValUmbral);

  u8g2.sendBuffer();
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
   
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/temp") {
      bot.sendMessage(chat_id, "La temperatura es: ");
      bot.sendMessage(chat_id, stringTempActual);
    }
  }
}