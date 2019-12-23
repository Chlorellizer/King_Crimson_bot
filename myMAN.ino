#include <DallasTemperature.h>// підключаемо бібліотеки 
#include <OneWire.h>
#include "CTBot.h"

#define LIGHT_ON_CALLBACK  "lightON"  //Оголошуємо "константу" яка потім при компіляції
#define LIGHT_OFF_CALLBACK "lightOFF" 
#define Temp "g"
#define Dark "25"
#define Light "50"
#define Lighter "75"
#define Clear "100"
#define Level "lev"
#define Access "acc"

CTBot myBot;// робимо копію класа 
CTBotInlineKeyboard myKbd;
CTBotInlineKeyboard myKbd1;
CTBotInlineKeyboard myKbd2; 
CTBotInlineKeyboard myKbd3; 
CTBotInlineKeyboard myKbd4; 

String ssid = "ASUS";     //  зміна яка містить WIFI SSID
String pass = "1234567890"; // зміна яка містить WIFI PASSWORD
String token = "900079398:AAEDyh4UHcKIg0-FEFmit2yZeLiY2jEG2F8";   // зміна що містить TELEGRAM BOT TOKEN 
uint8_t led = 0;            //пін реле   
uint8_t DS18B20 =4 ;            // пін тернометру 
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);


void setup() {
  Serial.begin(115200); //шидкість спілкуваня з компьютером
  Serial.println("Starting TelegramBot...");
  sensors.setResolution(12); // Встановлюємо разрядність терномитру біт(9-12)
  myBot.wifiConnect(ssid, pass); //підключамося до WIFI
  myBot.setTelegramToken(token); // підключаемося до телеграм бота
  if (myBot.testConnection()) // провіряємо підключеня до телеграму
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH); //включаемо реле

  //описуемо кнопки клавіатури
  myKbd.addButton("ON світло", LIGHT_ON_CALLBACK, CTBotKeyboardButtonQuery);//кнопки on/off світло
  myKbd.addButton("OFF світло", LIGHT_OFF_CALLBACK, CTBotKeyboardButtonQuery);
  myKbd.addRow(); //перенести кнопки на наступний рядок 
  myKbd1.addButton("Температура",Temp, CTBotKeyboardButtonQuery);// кнопка для заміру температури
  myKbd.addRow();
  myKbd2.addButton("☁",Dark, CTBotKeyboardButtonQuery);//кнопки для ригулюваня яскравості світла (25/50/75/100)%
  myKbd2.addButton("⛅",Light, CTBotKeyboardButtonQuery);
  myKbd2.addButton("🌤",Lighter, CTBotKeyboardButtonQuery);
  myKbd2.addButton("☀",Clear, CTBotKeyboardButtonQuery);
  myKbd2.addRow();
  myKbd3.addButton("Рівень води",Level, CTBotKeyboardButtonQuery);
  myKbd3.addRow();
  myKbd4.addButton("Доступ боту",Access, CTBotKeyboardButtonQuery);
  myKbd4.addRow();
 
}

void loop() {
  
  

  // a variable to store telegram message data
  TBMessage msg;
  float temperature;
 

  // if there is an incoming message...
  if (myBot.getNewMessage(msg)) {
    // check what kind of message I received
    if (msg.messageType == CTBotMessageText) {
      // received a text message
      if (msg.text.equalsIgnoreCase("/Keyboar")) {
        // the user is asking to show the inline keyboard --> show it
        myBot.sendMessage(msg.sender.id, "  Стан світла  ", myKbd ); 
        myBot.sendMessage(msg.sender.id, "Яскравість світла", myKbd2 );
         myBot.sendMessage(msg.sender.id, "Перевірити температуру", myKbd1 );
          myBot.sendMessage(msg.sender.id, "Перевірити рівень води", myKbd3 );
           myBot.sendMessage(msg.sender.id, "Надати доступ", myKbd4 );
        
      }
      else {
        
        myBot.sendMessage(msg.sender.id, "Натисни на команду в описі");
      }
    } else if (msg.messageType == CTBotMessageQuery) {
      if (msg.callbackQueryData.equals(LIGHT_ON_CALLBACK)) {
        // pushed "LIGHT ON" button...
        digitalWrite(led, HIGH); // ...turn on the LED (inverted logic!)
        // terminate the callback with an alert message
        myBot.endQuery(msg.callbackQueryID, "світло увімкнино", true);
      } else if (msg.callbackQueryData.equals(LIGHT_OFF_CALLBACK)) {
        // pushed "LIGHT OFF" button...
        digitalWrite(led, LOW); // ...turn off the LED (inverted logic!)
        // terminate the callback with a popup message
        myBot.endQuery(msg.callbackQueryID, "Світло вимкнено", true);
      } if (msg.callbackQueryData.equals(Temp)) {
    sensors.requestTemperatures();
        temperature = sensors.getTempCByIndex(0);
         myBot.sendMessage(msg.sender.id, String("Температура C:" ));
          myBot.sendMessage(msg.sender.id,String( temperature));
       Serial.print("Temp C: ");
  Serial.println(temperature);
  } if (msg.callbackQueryData.equals(Dark)) {
     analogWrite(5, 64);
        myBot.endQuery(msg.callbackQueryID, "Яскравість світла 25%", true);
    }
     if (msg.callbackQueryData.equals(Light)) {
     analogWrite(5, 127);
        myBot.endQuery(msg.callbackQueryID, "Яскравість  світла 50%", true);
    }
     if (msg.callbackQueryData.equals(Lighter)) {
     analogWrite(5, 191);
        myBot.endQuery(msg.callbackQueryID, "Яскравість світла 75%", true);
    }
     if (msg.callbackQueryData.equals(Clear)) {
     analogWrite(5, 255);
        myBot.endQuery(msg.callbackQueryID, "Яскравість світла 100%", true);
    }
    if (msg.callbackQueryData.equals(Level)) {
     
        myBot.endQuery(msg.callbackQueryID, "Рівень води в нормі ", true);
    }
     else if (msg.messageType == CTBotMessageQuery) {
      // received a callback query message
  
     
  }
  
  delay(100);
}}}
