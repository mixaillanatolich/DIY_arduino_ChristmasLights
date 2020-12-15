/*
   notamesh - IR controlled 'synchronized' led lighting effects using FastLED across multiple Arduino controllers.

         By: Andrew Tuline
       Date: October, 2018
        URL: www.tuline.com
      Email: atuline@gmail.com
       Gist: https://gist.github.com/atuline
     GitHub: https://github.com/atuline
   Pastebin: http://pastebin.com/u/atuline
    Youtube: https://www.youtube.com/user/atuline/videos
*/

//  Переделка и дополнение
//  Декабрь 2018
//  Бикин Дмитрий
//  d.bikin@bk.ru
//  SysWork.ru
//  Как подключить пульт
//  https://mysku.ru/blog/aliexpress/68990.html

// Добавлено управление одной кнопкой, декабрь 2019
// AlexGyver
// https://AlexGyver.ru/
// https://www.youtube.com/c/alexgyvershow/

/////////////////////////////////////////////////////////////////////////////////
///                               Настройка
/////////////////////////////////////////////////////////////////////////////////

#define ArduinoAVR 0  // AVR (Arduino NANO/MEGA/UNO)
#define ESP8266Board 1     // ESP8266 (NodeMCU, Wemos D1)

#define MCU_TYPE ESP8266Board

#define KOL_LED       20         // Сколько светодиодов в гирлянде при первом включении

//////////////////////////////////////// Параметры пульта
////
//Настройка действия кнопке осуществляется в файле command.h

#define USE_BTN 1   // использовать управление одной кнопкой
// клик: вкл-выкл
// 2х клик: след. режим
// 3х клик: пред. режим
// 4х клик: вкл/выкл блёстки
// удержание - яркость


#define IR_MAX_LEDS   250         // Максимальное количество светодиодов при работе с пультом должно быть больше
// или равно KOL_LED
// От размера этого числа зависит колличество используемой памяти


//////////////////////////////////////// Параметры гирлянды
#define COLOR_ORDER   GRB         //Очередность цвета светодиодов  RGB или GRB
#define CHIPSET       WS2812B     //Тип микросхемы светодиодов в гирлянде

#if (MCU_TYPE == ArduinoAVR)
#define LED_DT 6  
#define BTN_PIN 3
#elif (MCU_TYPE == ESP8266Board)
#define LED_DT 2   //(D4)
#define BTN_PIN 14 //(D5)      
#endif

#define POWER_V       5           // напряжение блока питания в Вольтах
#define POWER_I       100        // Ток блока питания в милиАмперах

//////////////////////////////////////// Параметры программы
////
#define BLACKSTART    1           // Первый запуск делать с черного экрана 0- начинать с эффекта, 1- начинать с черного экрана

#define GLITER_ON     0           // Включить блеск 0 - блеск отключен, 1- блеск включен

#define CANDLE_KOL    5           // если больше 0 то будут включена эмитация свечей
// число задает через какое расстояние друг от друга располагать свечи
#define CANDLE_ON     0           // Если включена эмитация свечей то 1 = при запуске она будет включена
//                                  0 = при запуске она будет выключена

#define CANDLE_40     1           // Управление 40 эффектом (свечи)   1 = при запуске она будет включена по варианту efimova1969
//                                  иначе при запуске она будет включена по моему варианту

#define BACKGR_ON     0           // Включить заполнение черного цвета фоном

#define DEMO_MODE     1           // Режим при включении
// 0 - последняя запущенная программа
// 1 - демо режим с последовательным перебором эффектов
// 2 - демо режим с случайным перебором эффектов
// 3 - демо режим с последовательным перебором выбранных эффектов
// 4 - демо режим с случайным перебором выбранных эффектов

#define MY_MODE     0,1,2,4,5,9,10,11,14,16,17,18,19,21, 22, 28, 30, 32, 34, 36, 37, 38
// Выбранные эффекты пишутся номера через запятую,
// можно писать и одинаковые что актуально в демо 3
// Если закоментировать, то демо 3 и 4 отключаются

#define DEMO_TIME     180           // Время отображение эффекта в демо режиме в секундах

#define CHANGE_ON     1           // Включить плавную смену эффектов
#define CHANGE_SPARK  1           // Эффект смены 0 - без эффекта, 1 - искры (бенгальский огонь) 2 - яркий свет
                                  // 3 - метеорит,    4 - случайный эффект (из 1-3)
#define CHANGE_TIME   5           // Время в секундах, за которое произойдет плавная смена эффектов

#define DIRECT_TIME   30          // Через сколько секунд менять направление если 0 - не меняем
#define PALETTE_TIME  60          // Через сколько секунд менять палитру если 0 - не меняем

#define HARDWARE_BT_SERIAL        0 

#if HARDWARE_BT_SERIAL
#define LOG_ON        0 
#else
#define LOG_ON        1           // Включить лог  1 - включить лог
#endif


#if LOG_ON
# define DBG_PRINT(...)    Serial.print(__VA_ARGS__)
# define DBG_PRINTLN(...)  Serial.println(__VA_ARGS__)
#else
# define DBG_PRINT(...)
# define DBG_PRINTLN(...)
#endif

#if (MCU_TYPE == ESP8266Board)
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0
#include <ESP8266WiFi.h>
#endif
