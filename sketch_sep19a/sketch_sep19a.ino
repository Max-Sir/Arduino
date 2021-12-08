#include "DHT.h"                  //Подключаем библиотеку для работы с датчиком DHT11
#include "Wire.h"               
#include "LiquidCrystal_I2C.h"    //Подключаем библиотеки для работы с LCD дисплеем

int LD;                           //Объявляем вспомогательную переменную

#define DHTPIN 2                  //Задаем PIN для подключения датчика DHT11
#define LED 9                     //Задаем PIN который выдает ШИМ-сигнал
#define FOTO 0                    //Задаем PIN для подключения фоторезистора
#define MOTOR 6

DHT dht(DHTPIN, DHT11);           //Инициализируем датчик DHT11
LiquidCrystal_I2C lcd(0x27,16,2); //Инициализируем дисплей


void setup() {
 pinMode(LED, OUTPUT);            //Настраиваем PIN LED как выход
 pinMode(MOTOR,OUTPUT);
 dht.begin();                     //Включаем датчик температуры и влажности
 
 lcd.init();                      //Включаем LCD дисплей
 lcd.backlight();
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


void loop() {
   int f = analogRead(FOTO);              //Считываем значение с датчика освещенности
   LD=light(f);
   delay(1);
   analogWrite(LED, LD);                  //Устанавливаем значение ШИМ 
   float h=dht.readHumidity();            //Считываем значение влажности
   float t=dht.readTemperature();         //Считываем значение температуры
    if(t>=24.0){
      digitalWrite(MOTOR,HIGH);
    }
    else{
      digitalWrite(MOTOR,LOW);
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
