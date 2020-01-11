
byte tachPIN=2;                               // Объявлем переменную tachPIN - Вывод, используемый для подключения анемометра = 2
byte meterPIN=3;                              // Объявлем переменную meterPIN - Вывод, используемый для подключения стрелочного прибора = 3
byte ledPIN=4;                                // Объявлем переменную ledPIN - Вывод, используемый для подключения светодиода = 4
unsigned long sampleTime=5000;                // Объявлем переменную sampleTime - время, в течении которого происходит замер скорости ветра
unsigned int count;                           // Объявляем переменную count - счетчик количества импульсов анемометра
boolean kflag;                                // Объявляем переменную kflag - флаг импульса
unsigned long currentTime;                    // Объявляем переменную currentTime - текущее время
unsigned long startTime;                      // Объявляем переменную startTime - время начала замера скорости
unsigned int wind;                            // Объявляем переменную wind - значение скорости ветра
unsigned int wspeed;                          // Объявляем переменную wspeed - значение скорости ветра при замере
int newvalue;
int value=0;

#include <LedControl.h>                       // Подключаем библиотеку управления LED индикатором на базе MAX7219

const int DIN_PIN = 11;                       // Конфигурируем выводы контроллера для управления сегментным индикатором
const int CS_PIN = 9;
const int CLK_PIN = 13;
const int SEGMENTS = 1;
                                              // Задаем массив изображений цифр 0 ... 39 для односегментного индикатора
const uint64_t IMAGES[] = {
  0x3844444444444438,
  0x3810101010101810,
  0x7c08102040404438,
  0x3844404030404438,
  0x40407c4448506040,
  0x384440403c04047c,
  0x384444443c044438,
  0x080808102040407c,
  0x3844444438444438,
  0x3844407844444438,
  0x6792929292929362,
  0xe742424242426342,
  0xf712224282829362,
  0x6792828262829362,
  0x878282f292a2c382,
  0x67928282721213f2,
  0x6792929272129362,
  0x27222222428283f2,
  0x6792929262929362,
  0x679282e292929362,
  0x6791919294949567,
  0xe741414244446547,
  0xe72121428484a5e7,
  0x6791818264849567,
  0x878181f294a4c587,
  0x67918182741415f7,
  0x6791919274149567,
  0x27212122448485f7,
  0x6791919264949567,
  0x679181e294949567,
  0x6794949492949467,
  0xe744444442446447,
  0xe72424448284a4e7,
  0x7744444422444477,
  0x878484f492a4c487,
  0x67948484721414f7,
  0x6794949472149467,
  0x27242424428484f7,
  0x6794949462949467,
  0x679484e492949467
};
const int IMAGES_LEN = sizeof(IMAGES)/8;


LedControl display = LedControl(DIN_PIN, CLK_PIN, CS_PIN,SEGMENTS);

void setup()
{
display.clearDisplay(0);
display.shutdown(0, false);
display.setIntensity(0, 5);

// Serial.begin(9600);                               // start serial port                         
pinMode(tachPIN, INPUT);                             // Конфигурируем tachPIN как вход
pinMode(ledPIN, OUTPUT);
}

void loop()
{
displayImage(IMAGES[wind]);                        // Переходим в подпрограмму вывода значения скорости ветра на LED индикатор
out();                                             // Переходим в подпрограмму вывода значения скорости ветра на стрелочный прибор
wind = getWind();                                  // Вызываем функцию getWind - замер скорости ветра   
// Serial.print ("Wind speed: ");
// Serial.println (wind);
}

                                  
int getWind()                                      // Функция getWind - замер скорости ветра
{
count=0;                                          // Обнуляем счетчик количества импульсов
kflag=LOW;                                        // Сбрасываем флаг импульса в 0

if (digitalRead(tachPIN)==HIGH)                   // Считываем значение на входе tachPIN
{
kflag=HIGH;                                       // В случае если на входе tachpin логическая единица, устанавливаем kflag = 1
}

currentTime=0;                                    // Обнуляем текущее время замера
startTime=millis();                               // Записываем в переменную startTime текущее значение времени
while (currentTime<=sampleTime)                   // Цикл будет выполнятся до тех пор пока значение currentTime не превысит sampleTime
{

 
if (digitalRead(tachPIN)==HIGH)                   //  Если на входе "1"
{                                                
  digitalWrite (ledPIN, HIGH);                    // Включаем светодиод pin4 = 1
  if (kflag==LOW)                                 //  Если состояние kflag = '0'
  {
  count++;                                        // Инкрементируем значение счетчика импульсов
  }                                                
  kflag=HIGH;                                     // Устанавливаем kflag = '1'
}                                                 
else                                              // В противном случае (если на входе "0")
{
  digitalWrite (ledPIN, LOW);                     // Выключаем светодиод pin4 = 0
  if (kflag==HIGH)                                // Если состояние kflag = '1'
  {
  count++;                                        // Инкрементируем значение счетчика импульсов
  }
  kflag=LOW;                                      // Устанавливаем kflag = '0'                                        
}


currentTime= millis()-startTime;                  // Определяем - сколько прошло времени от момента времени начала измерения, указанного в startTime
}

wspeed = 0.13*count;                              // Формула расчета скорости ветра в зависимости от количества поступивших импульсов
return wspeed;

}

void out()                   // Подпрограмма вывода скорости ветра на стрелочный прибор
{   
   newvalue = wind*5.1;        // Расчет нового дискретного значения ШИМ 
   while (value != newvalue)
   {
     if ((newvalue) > (value))
     {
       value++;
     }
      else
     { 
      value--;
     }
     delay (20);                       // Задержка времени при перемещении стрелки на одну дискрету ШИМ
     analogWrite (meterPIN,value);     // Вывод показания на стрелочный прибор
   }
}

void displayImage(uint64_t image)      // Подпрограмма вывода значения скорости ветра на LED индикатор
{
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(row, j));
    }
  }
}
