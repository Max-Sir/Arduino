#include "DHT.h"                  //Подключаем библиотеку для работы с датчиком DHT11
#include "Wire.h"               
#include "LiquidCrystal_I2C.h"    //Подключаем библиотеки для работы с LCD дисплеем

int LD;                           //Объявляем вспомогательную переменную

#define DHTPIN 2                  //Задаем PIN для подключения датчика DHT11
#define LED 9                     //Задаем PIN который выдает ШИМ-сигнал
#define FOTO 0                    //Задаем PIN для подключения фоторезистора
#define MOTOR 6
#define PEZO 13
#define PIN_POT     A0
#define FLAME A5
#define HIGH_HUM_LED 3
#define SEC_LED 5
#define CONDITION(x,part) (x>=1024/part)

DHT dht(DHTPIN, DHT11);           //Инициализируем датчик DHT11
LiquidCrystal_I2C lcd(0x27,16,2); //Инициализируем дисплей


void setup() {
 pinMode(LED, OUTPUT);            //Настраиваем PIN LED как выход
 pinMode(MOTOR,OUTPUT);
 pinMode(FLAME, INPUT);
                                  //Пин с переменным резистором является входом
 pinMode(PIN_POT, INPUT);
 dht.begin();                     //Включаем датчик температуры и влажности
 Serial.begin(9600);
 lcd.init();                      //Включаем LCD дисплей
 lcd.backlight();
}

void secLedOn(){
  analogWrite(SEC_LED, 255);
}

void secLedOff(){
  digitalWrite(SEC_LED, LOW);
}

int light(int svet)               //Функция преобразования 
{                                 //значения с датчика освещенности 
  if (svet<25)                    //в значение ШИМ-сигнала
  {
    return 255;
  }

  if(svet>1020)
  {
    return 2;
  }
  else
  {
    int L=(int)(-0.2*svet+261.262);
    return L;
  }
}

void buzzON(){
   // Whoop up
   secLedOn();
  for(int hz = 440; hz < 1000; hz++){
    if(!CONDITION(analogRead(PIN_POT),4)) {buzzOFF();return;}
    tone(PEZO, hz, 50);
    if(hz==1440/2){
      secLedOff();
      ledOn();
    }
    delay(2);
  }
  noTone(PEZO);
  ledOff();
secLedOn();
  // Whoop down
  for(int hz = 1000; hz > 440; hz--){
    if(!CONDITION(analogRead(PIN_POT),4)) {buzzOFF();return;}
    if(hz==1440/2){
      secLedOff();
      ledOn();
    }
    tone(PEZO, hz, 50);
    delay(2);
  }
  noTone(PEZO);
  ledOff();
}

void buzzOFF(){
  noTone(PEZO);
}

void ledOn(){
  digitalWrite(LED, HIGH);
}

void ledOff(){
    digitalWrite(LED, LOW);
}

void fanON(){
  digitalWrite(MOTOR,HIGH);
}

void fanOFF(){
  digitalWrite(MOTOR,LOW);
}

void loop() {
   //int f = analogRead(FOTO);              //Считываем значение с датчика освещенности
   //LD=light(f);
   int flame = analogRead(FLAME);
   Serial.println(flame);
   if(flame>600){
    /*tone(PEZO, 50, 500);
    delay(4);
    buzzOFF();*/
   }
   delay(100);
   int rotat = analogRead(PIN_POT);
   analogWrite(LED, LD);                  //Устанавливаем значение ШИМ 
   float h=dht.readHumidity();            //Считываем значение влажности
   float t=dht.readTemperature();         //Считываем значение температуры
    if(h>=70.00){
    analogWrite(HIGH_HUM_LED, 255);
   }
   else{
    digitalWrite(HIGH_HUM_LED, LOW);
   }
   
    if(t>=24.0){
      if(CONDITION(rotat,4)){
      fanON();
      buzzON();
      }
      else{
        fanOFF();
        buzzOFF();  
      }
    }
    else{
       buzzOFF();
       fanOFF();
    }
    lcd.setCursor(0,0); //Устанавливаем курсор в нулевую позицию верхней строки
    lcd.print("Temp: ");                   //Отображаем значение температуры
    lcd.print(t);
    lcd.print((char)223);
    lcd.print("C");
    
    lcd.setCursor(0,1);                   //Помещаем курсор в нулевую позицию нижней строки
    lcd.print("Humidity: ");                    //Отображаем значение влажности
    lcd.print(h);
    lcd.print("%");

}
