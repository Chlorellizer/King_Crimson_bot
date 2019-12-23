#include <DallasTemperature.h>// підключаемо бібліотеки 
#include <OneWire.h>
#include "CTBot.h"

#define LIGHT_ON_CALLBACK  "lightON"  //Оголошуємо "константу" яка потім при компіляції
#define LIGHT_OFF_CALLBACK "lightOFF" 
#define LIGHT_check "Chec"
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
String pass = ""; // зміна яка містить WIFI PASSWORD
String token = "900079398:AAEDyh4UHcKIg0-FEFmit2yZeLiY2jEG2F8";   // зміна що містить TELEGRAM BOT TOKEN 
uint8_t led = 0;            //пін реле   
uint8_t DS18B20 =4 ;            // пін тернометру 
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
String PS = "2345";
String ps;
int f =1;
int  password;
float temperature;
int i;
char v=0;
String names[]  = {
  "Vrode_you",   // 0
  "ti_proigral_i_ya_toshe",     // 1
  "nikitosik_tb",
  "ReyForest"
  
};




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
  pinMode(13, INPUT);
  pinMode(12, OUTPUT);
  randomSeed(millis());
  digitalWrite(led, HIGH); //включаемо реле
  password = random(1000,10000);
  
  
 

  //описуемо кнопки клавіатури
  myKbd.addButton("ON світло", LIGHT_ON_CALLBACK, CTBotKeyboardButtonQuery);//кнопки on/off світло
  myKbd.addButton("OFF світло", LIGHT_OFF_CALLBACK, CTBotKeyboardButtonQuery);
  myKbd.addButton("Стан світла", LIGHT_check, CTBotKeyboardButtonQuery);
  myKbd.addRow(); //перенести кнопки на наступний рядок 
  myKbd1.addButton("Температура",Temp, CTBotKeyboardButtonQuery);// кнопка для заміру температури
  myKbd1.addRow();
  myKbd2.addButton("☁",Dark, CTBotKeyboardButtonQuery);//кнопки для ригулюваня яскравості світла (25/50/75/100)%
  myKbd2.addButton("⛅",Light, CTBotKeyboardButtonQuery);
  myKbd2.addButton("🌤",Lighter, CTBotKeyboardButtonQuery);
  myKbd2.addButton("☀",Clear, CTBotKeyboardButtonQuery);
  myKbd2.addRow();
  myKbd3.addButton("Рівень води",Level, CTBotKeyboardButtonQuery);
  myKbd3.addRow();
  myKbd4.addButton("Дізнатися пароль",Access, CTBotKeyboardButtonQuery);
  myKbd4.addRow();
 
}

void loop() {
  String PS= String(password);
  // a variable to store telegram message data
  TBMessage msg;
  
 // якщо є вхідне повідомлення
  if (myBot.getNewMessage(msg)) {
    // перевірити, яке саме повідомлення я отримав
    if (msg.messageType == CTBotMessageText) {
       // отримав текстове повідомлення
      if (msg.text.equalsIgnoreCase(PS)|| msg.text.equalsIgnoreCase("/Keyboar")) {
        if(msg.text.equalsIgnoreCase(PS)){
         v=v+2;
         Serial.print(v);
         }
        for (i = 0; i < sizeof(names)-1; i = i + 1) {
         if( msg.sender.username ==names[i] ){
        // Користувач просить показати вбудовану клавіатуру -> показати її 
          v=v+1;
          Serial.print(v);
       } 
       }if(v==1) {
        myBot.sendMessage(msg.sender.id, "  Стан світла  ", myKbd ); 
        myBot.sendMessage(msg.sender.id, "Яскравість світла", myKbd2 );
         myBot.sendMessage(msg.sender.id, "Перевірити температуру", myKbd1 );
          myBot.sendMessage(msg.sender.id, "Перевірити рівень води", myKbd3 );
           myBot.sendMessage(msg.sender.id, "Надати доступ", myKbd4 );
           v=0;
        }else  if(v>1) {
        myBot.sendMessage(msg.sender.id, "  Стан світла  ", myKbd ); 
        myBot.sendMessage(msg.sender.id, "Яскравість світла", myKbd2 );
         myBot.sendMessage(msg.sender.id, "Перевірити температуру", myKbd1 );
          myBot.sendMessage(msg.sender.id, "Перевірити рівень води", myKbd3 );
           v=0;
        }
        else {myBot.sendMessage(msg.sender.id, "Вам не надано доступу до бота");}}
     
      else {
        
        myBot.sendMessage(msg.sender.id, "Натисни на команду в описі");
      }
    } else if (msg.messageType == CTBotMessageQuery) {
      if (msg.callbackQueryData.equals(LIGHT_ON_CALLBACK)) {
        // натиснута кнопка "СВІТЛИ ВКЛ." ...
        digitalWrite(led, HIGH); 
        f=f+1;
        // ... увімкніть світлодіод
       // припинити зворотний дзвінок за допомогою попереджувального повідомлення
        myBot.endQuery(msg.callbackQueryID, "світло увімкнено", true);
      } else if (msg.callbackQueryData.equals(LIGHT_OFF_CALLBACK)) {
        // натиснута кнопка "LIGHT OFF" ...
        digitalWrite(led, LOW);// ... вимкніть світлодіод
        f=0;
       // припинити зворотний дзвінок за допомогою попереджувального повідомлення
        myBot.endQuery(msg.callbackQueryID, "Світло вимкнено", true);
      }if (msg.callbackQueryData.equals(LIGHT_check)) {
         if(f==1){
        myBot.endQuery(msg.callbackQueryID, "світло увімкнено", true);}
        else {myBot.endQuery(msg.callbackQueryID, "Світло вимкнено", true);}
        }
      if (msg.callbackQueryData.equals(Temp)) {
    sensors.requestTemperatures();
        temperature = sensors.getTempCByIndex(0);
        String reply;
        reply = (String)"Температура C: " + temperature;
          myBot.endQuery(msg.callbackQueryID,reply , true);
       Serial.print("Temp C: ");
  Serial.println(temperature);
  } if (msg.callbackQueryData.equals(Dark)) {
     analogWrite(5, 255);
        myBot.endQuery(msg.callbackQueryID, "Яскравість світла 25%", true);
    }
     if (msg.callbackQueryData.equals(Light)) {
     analogWrite(5, 511);
        myBot.endQuery(msg.callbackQueryID, "Яскравість  світла 50%", true);
    }
     if (msg.callbackQueryData.equals(Lighter)) {
     analogWrite(5, 767);
        myBot.endQuery(msg.callbackQueryID, "Яскравість світла 75%", true);
    }
     if (msg.callbackQueryData.equals(Clear)) {
     analogWrite(5, 1023);
        myBot.endQuery(msg.callbackQueryID, "Яскравість світла 100%", true);
    }
    if (msg.callbackQueryData.equals(Level)) {
      digitalWrite(12, HIGH);
      delay(2000);
      boolean yt =digitalRead(13);
      delay(4000);
      if(yt==0){
     
        myBot.endQuery(msg.callbackQueryID, "Рівень води в нормі✔ ", true);
        digitalWrite(12, LOW);
    }else{myBot.endQuery(msg.callbackQueryID, "Не вистачае води ❗ ", true);
    digitalWrite(12, LOW);}}
    if (msg.callbackQueryData.equals(Access)) {
      ps= ("Пароль для входу: "+PS);
      myBot.endQuery(msg.callbackQueryID, ps, true);
    }
    
     else if (msg.messageType == CTBotMessageQuery) {
      // received a callback query message
  
     
  }
  
  delay(100);
}}}
