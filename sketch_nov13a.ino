#include <Adafruit_GFX.h> //библиотеки дисплея
#include <Adafruit_SSD1306.h>

//объявили переменные
float T_0; //батарея
float T_1; //улица
float T_3; //дом
float T_4; //влажнось
float T_5; //давление

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET  -1 // sharing Arduino reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
Adafruit_BME280 bme;

#include <OneWire.h> //библиотеки DS18B20
#include <DallasTemperature.h>
OneWire oneWire(2); //пин подключения датчиков
//OneWire oneWire(SensorDataPin);
DallasTemperature sensors(&oneWire);


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define WIFI_SSID "lol"
 #define WIFI_PASSWORD "123321456"
 #define BOT_TOKEN "6607908525:AAGewcpNQJgwoS7yCE_nibXnIf0ZSBOlo_8"
 const unsigned long BOT_MTBS = 100;
 X509List cert(TELEGRAM_CERTIFICATE_ROOT);
 WiFiClientSecure secured_client;
 UniversalTelegramBot bot(BOT_TOKEN, secured_client);
 int botRequestDelay = 1000;
 unsigned long bot_lasttime;
 //int ledStatus = 0;
 void handleNewMessages(int numNewMessages)
 {
 for (int i = 0; i < numNewMessages; i++)
 {
 String chat_id = bot.messages[i].chat_id;
 String text = bot.messages[i].text;
 String from_name = bot.messages[i].from_name;
 if (from_name == "")
 from_name = "Guest";
 if (text == "/start") {
      String welcome = "Привет, " + from_name + ".\n";
      welcome += "Для управления устройством используйте следующий команды:\n\n";  // Используйте следующие команды
      welcome += "/temp - узнать температуру \n";                                    // Включение светодиода
     
      bot.sendMessage(chat_id, welcome, "");
    }
     if(text == "/temp"){
      bot.sendMessage(chat_id, (String("Температура на улице ") + String(T_1) + String(" °C")), "");
      bot.sendMessage(chat_id, (String("Давление ") + String(T_5) + String(" ммРст")), "");
      bot.sendMessage(chat_id, (String("Температура в комнате ") + String(T_3) + String(" °C")),"");
      bot.sendMessage(chat_id, (String("Влажность в комнате ") + String(T_4) + String(" %")), "");
      bot.sendMessage(chat_id, (String("Температура батареи ") + String(T_0) + String(" °C")), "");
   }
 }
 }


void setup() {
  bme.begin(0x76);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.display();
sensors.requestTemperatures(); 
  sensors.begin();

  Serial.begin(9600);
 Serial.println();
 configTime(0, 0, "pool.ntp.org");
 secured_client.setTrustAnchors(&cert);
 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 while (WiFi.status() != WL_CONNECTED)
 {

 delay(500);
 }
 Serial.print("WiFi connected. IP address: ");
 Serial.println(WiFi.localIP());
 Serial.print("Retrieving time: ");
 time_t now = time(nullptr);
 while (now < 24 * 3600)
 {
  delay(100);
 now = time(nullptr);
 }

  

}

void loop() {
  display.setRotation(0);
  sensors.requestTemperatures();
  T_0 = sensors.getTempCByIndex(0);\
  T_1 = sensors.getTempCByIndex(1);
  T_3 = bme.readTemperature();
  T_4 = bme.readHumidity();
  T_5 = bme.readPressure() / 133.322;

    delay(500);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.clearDisplay();

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,20);
    display.print("D");
    display.print(" ");
    display.print(T_3);
    display.print("");
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);
    display.setTextSize(2);
    display.print("C");
     display.display();

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,40);
    display.print("U");
    display.print(" ");
    display.print(T_1);
    display.print("");
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);
    display.setTextSize(2);
    display.print("C");
     display.display();
     delay(200);
    if (millis() - bot_lasttime > BOT_MTBS)
    {
     int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while (numNewMessages)
     {

     handleNewMessages(numNewMessages);
     numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
     bot_lasttime = millis();
     }

}