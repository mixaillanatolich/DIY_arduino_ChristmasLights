#ifndef COMMANDS_H
#define COMMANDS_H

//здесь находятся команды обрабатываемые скетчем
//формат такой
// #define IR_Key_XXXX   YYYYY
// где IR_Key_XXXX - код команды, это лучше не менять
//     YYYYY -Код клавиши пульта или аналоговой клавиатуры
//            Если кнопка не используется, то тогда надо либо поставить код 0, либо закоментировать строку
//                Например #define IR_Key_XXXX   0xff4ab5
//                или так  #define IR_Key_XXXX   KEY_0

// кнопка на пульте
#define IR_Key_Brightness_plus              0x01  // N/P                    // Увеличить максимальную яркость 
#define IR_Key_Brightness_minus             0x02  // Repeat                 // Уменьшить максимальную яркость 
#define IR_Key_Reset                        0x03  // Power                  // Сброс всех настроек и перезагрузка 
#define IR_Key_Demo                         0x04  // Open                   // Включит демо режим (перебор) 
#define IR_Key_Demo_Random                  0x05  // Prog                   // Включит демо режим (Случайно) 
#define IR_Key_Demo_MyMode                  0x06  // A-B                   // Включит демо режим (перебор) 
#define IR_Key_Demo_MyMode_Random           0x07  // RAN                   // Включит демо режим (Случайно) 

#define IR_Key_Length_Garland_plus          0x08  // L/R                    // Увеличить количество светодиодов в гирлянде 
#define IR_Key_Length_Garland_minus         0x09  // Slow                   // Уменьшить количество светодиодов в гирлянде
#define IR_Key_Rotate                       0x0a  // Setup                  // Сменить направление движения эффектов
#define IR_Key_Speed_minus                  0x0b  // vol -                  // Замедлить движение
#define IR_Key_Speed_plus                   0x0c  // vol +                  // Ускорить движение

#define IR_Key_Glitter                      0x0d  // Subtitle               // Включить/выключить сверкание
#define IR_Key_BackGround                   0x0e  // Time                   // Включить/выключить заполнение фона
#define IR_Key_Candle                       0x0f  // PDVD                   // Включить/выключить свечки

#define IR_Key_Previous_mode                0x10  // Down                   // Предыдущий эффект. Отключает демонстрационный режим
#define IR_Key_Next_mode                    0x11  // Up                     // Следующий эффект. Отключает демонстрационный режим

#define IR_Key_Save_Mode                    0x12  // Clear                  // Сохранить эффект как запускающийся первым
#define IR_Key_Delay_minus                  0x13  // Resume                 // Уменьшить задержку на 100ms (используя белые светодиоды)
#define IR_Key_Delay_plus                   0x14  // Step                   // Увеличить задержку на 100ms (используя белые светодиоды)

#define IR_Key_Palette_Stop                 0x15  // Menu                   // Остановка вращения палитры        Остановить перебор Палитр
#define IR_Key_Palette_Previous             0x16  // <-                     // Выберите предыдущую палитру       Установить предыдущую палитру
#define IR_Key_Palette_Next                 0x17  // ->                     // Выберите следующую палитру        Установить следующую палитру
#define IR_Key_Palette_Auto                 0x18  // Enter                  // Включить поворот палитры          Продолжить автоматический перебор палитр

#define IR_Key_Solid_Black                  0x20  // <<                     //  Установить цвет Черный  (Выключает гирлянду, Например перед тем как сказать елочка зажгись на детском утреннике)
#define IR_Key_Solid_Red                    0x21  // >>                     //  Установить цвет Красный
#define IR_Key_Solid_Orange                 0x22  // |<<                    //  Установить цвет Оранжевый
#define IR_Key_Solid_Yellow                 0x23  // >>|                    //  Установить цвет Желтый
#define IR_Key_Solid_Green                  0x24  // |>                     //  Установить цвет Зеленый
#define IR_Key_Solid_SkyBlue                0x25  // ||                     //  Установить цвет Голубой
#define IR_Key_Solid_Blue                   0x26  // []                     //  Установить цвет Синий
#define IR_Key_Solid_Violet                 0x27  // ><                     //  Установить цвет Фиолетовый
#define IR_Key_Solid_White                  0x28  // Zoom                   //  Установить цвет Белый

#endif

void SetMode(uint8_t modeId) { 
    demorun = 0;
#if CHANGE_ON == 1
    newMode = modeId;
    StepMode = 1;
#if CHANGE_SPARK == 4
    rand_spark = random8(3) + 1;
#endif //CHANGE_SPARK
#else //CHANGE_ON
    ledMode = modeId;
    meshwait();
    strobe_mode(modeId, 1);                                // Does NOT reset to 0.
#endif // else CHANGE_ON

#if CANDLE_KOL > 0
    PolCandle = random8(CANDLE_KOL);
#endif
}

void bootme() {                                                 // This is used to reset all the Arduinos so that their millis() counters are all in sync.
#if (MCU_TYPE == ArduinoAVR)
    asm volatile("jmp 0");
#elif (MCU_TYPE == ESP8266)

#endif
}

void meshwait() {                                                   // After we press a mode key, we need to wait a bit for the sequence to start.
    DBG_PRINT(F("Mesh delay: ")); DBG_PRINT(meshdelay * 100); DBG_PRINTLN(F("ms delay."));
    FastLED.delay(meshdelay * 100);                                   // Here's our notamesh wait upon keypress. Oh god I'm so sorry there's a delay statement here. At least it's only used upon mode change keypress.
}

void handleControlCmd() {
    DBG_PRINT(F("Command: 0x")); DBG_PRINTLN(Command, HEX);

     switch (Command) {

      case IR_Key_Brightness_plus :   //////////////////////////////////////////////////////////////////////////  Увеличить максимальную яркость
        max_bright = min(max_bright * 2, 255);
        LEDS.setBrightness(max_bright);
        DBG_PRINT(F("Brightness+ ")); DBG_PRINTLN(max_bright);
        break;

      case IR_Key_Brightness_minus :    /////////////////////////////////////////////////////////////////////////  Уменьшить максимальную яркость
        max_bright = max(max_bright / 2, 1);
        LEDS.setBrightness(max_bright);
        DBG_PRINT(F("Brightness- ")); DBG_PRINTLN(max_bright);
        break;

      case IR_Key_Reset :              ///////////////////////////////////////////////////////////////////////////  Сброс всех настроек
        ledMode = 0; strobe_mode(ledMode, 1); FastLED.show(); bootme();
        DBG_PRINTLN(F("Reset"));
        break;

      case IR_Key_Demo :            /////////////////////////////////////////////////////////////////////////////  Включит демо режим (перебор)
        if (ledMode > maxMode) {
            SetMode(newMode);
        }
        demorun = 1; 
        meshwait();
        DBG_PRINTLN(F("Demo On"));
        break;

      case IR_Key_Demo_Random :     /////////////////////////////////////////////////////////////////////////////  Включит демо режим (Случайно)
        if (ledMode > maxMode) {
            SetMode(newMode);
        }
        demorun = 2; 
        meshwait();
        DBG_PRINTLN(F("Demo Random On"));
        break;

      case IR_Key_Demo_MyMode :         /////////////////////////////////////////////////////////////////////////////  Включит демо режим из выбранных режимов (перебор)
        if (ledMode > maxMode) {
            SetMode(newMode);
        }
        demorun = 3; 
        meshwait();
        DBG_PRINTLN(F("Demo On"));
        break;

      case IR_Key_Demo_MyMode_Random :     /////////////////////////////////////////////////////////////////////////////  Включит демо режим из выбранных режимов (Случайно)
        if (ledMode > maxMode) {
            SetMode(newMode);
        }
        demorun = 4; 
        meshwait();
        DBG_PRINTLN(F("Demo Random On"));
        break;

      case IR_Key_Length_Garland_plus : ///////////////////////////////////////////////////////////////////////////  Увеличить количество светодиодов в гирлянде
        demorun = 0; ledMode = 200;
        if (NUM_LEDS < (MAX_LEDS - 1)) NUM_LEDS++;                        //Новое значение

#if MAX_LEDS < 255
        EEPROM.write(STRANDLEN, NUM_LEDS);                              //Сохранить в память
#else
        EEPROM.write(STRANDLEN,   (uint16_t)(NUM_LEDS) & 0x00ff);       //Сохранить в память
        EEPROM.write(STRANDLEN + 1, (uint16_t)(NUM_LEDS) >> 8);         //Сохранить в память
#endif
        EEPROM.commit();
        DBG_PRINT(F("Length Garland ")); DBG_PRINTLN(NUM_LEDS);

        break;

      case IR_Key_Length_Garland_minus : ///////////////////////////////////////////////////////////////////////////  Уменьшить количество светодиодов в гирлянде
        demorun = 0; ledMode = 200;
        if (NUM_LEDS > 0) NUM_LEDS--;                                     //Новое значение

#if MAX_LEDS < 255
        EEPROM.write(STRANDLEN, NUM_LEDS);                              //Сохранить в память
#else
        EEPROM.write(STRANDLEN,   (uint16_t)(NUM_LEDS) & 0x00ff);       //Сохранить в память
        EEPROM.write(STRANDLEN + 1, (uint16_t)(NUM_LEDS) >> 8);         //Сохранить в память
#endif
        EEPROM.commit();
        DBG_PRINT(F("Length Garland ")); DBG_PRINTLN(NUM_LEDS);

        break;

      case IR_Key_Rotate :             ///////////////////////////////////////////////////////////////////////////  Сменить направление движения эффектов
          thisdir = thisdir * -1;
          DBG_PRINTLN(F("Rotate"));
        break;

      case IR_Key_Speed_minus :                ///////////////////////////////////////////////////////////////////////////  Замедлить движение
        if (thisdelay < 1000) thisdelay++;
        DBG_PRINT(F("Speed- ")); DBG_PRINTLN(thisdelay);
        break;

      case IR_Key_Speed_plus :                ///////////////////////////////////////////////////////////////////////////  Ускорить движение
        if (thisdelay > 0) thisdelay--;
        DBG_PRINT(F("Speed+ ")); DBG_PRINTLN(thisdelay);
        break;

      case IR_Key_Glitter :                ///////////////////////////////////////////////////////////////////////////  Включить/выключить сверкание
          glitter = !glitter;
          DBG_PRINT(F("Glitter ")); DBG_PRINTLN(glitter);
        break;

      case IR_Key_BackGround :                ///////////////////////////////////////////////////////////////////////////  Включить/выключить заполнение фона
          background = !background;
          DBG_PRINT(F("BackGround ")); DBG_PRINTLN(background);
        break;

      case IR_Key_Candle :                ///////////////////////////////////////////////////////////////////////////  Включить/выключить Свечки
#if CANDLE_KOL > 0
          candle = !candle;
          PolCandle = random8(10);
          DBG_PRINT(F("Candle ")); DBG_PRINTLN(candle);
#endif
        break;

      case IR_Key_Previous_mode :          ///////////////////////////////////////////////////////////////////////////  Предыдущий эффект
          if (ledMode > 0) {
              SetMode(ledMode - 1);
          } else {
              SetMode(maxMode);
          }
          DBG_PRINTLN(F("Previous mode"));
        break;

      case IR_Key_Next_mode :              ///////////////////////////////////////////////////////////////////////////  Следующий эффект
          if (ledMode >= (maxMode)) {
              SetMode(0);
          } else {
              SetMode(ledMode + 1);
          }
          DBG_PRINTLN(F("Next mode"));
        break;

      case IR_Key_Save_Mode :              ///////////////////////////////////////////////////////////////////////////  Сохранить эффект как запускающийся первым
          EEPROM.write(STARTMODE, ledMode);
          EEPROM.commit();
          DBG_PRINTLN(F("Save Mode"));
        break;

      case IR_Key_Delay_minus :            ///////////////////////////////////////////////////////////////////////////  Уменьшить задержку на 100ms
        demorun = 0; ledMode = 201;
        if (meshdelay > 0) meshdelay--;                                   //Новое значение
        EEPROM.write(STRANDEL, meshdelay);
        EEPROM.commit();
        DBG_PRINT(F("Delay ")); DBG_PRINT(meshdelay * 100); DBG_PRINTLN(F(" ms"));
        break;

      case IR_Key_Delay_plus :            ///////////////////////////////////////////////////////////////////////////  Увеличить задержку на 100ms
        demorun = 0; ledMode = 201;
        if (meshdelay < 100) meshdelay++;                                 //Новое значение
        EEPROM.write(STRANDEL, meshdelay);
        EEPROM.commit();
        DBG_PRINT(F("Delay ")); DBG_PRINT(meshdelay * 100); DBG_PRINTLN(F(" ms"));
        break;

      case IR_Key_Palette_Stop :            ///////////////////////////////////////////////////////////////////////////  Остановить перебор Палитр
        palchg = 0;
        DBG_PRINT(F("Palette_Stop ")); DBG_PRINTLN(gCurrentPaletteNumber);
        break;

      case IR_Key_Palette_Previous :        ///////////////////////////////////////////////////////////////////////////  Установить предыдущую палитру
          palchg = 1;
          if (gCurrentPaletteNumber > 0)  gCurrentPaletteNumber--;
          else                            gCurrentPaletteNumber = gGradientPaletteCount - 1;
          gCurrentPalette = (gGradientPalettes[gCurrentPaletteNumber]);
          DBG_PRINT(F("Palette- ")); DBG_PRINTLN(gCurrentPaletteNumber);
        break;

      case IR_Key_Palette_Next :            ///////////////////////////////////////////////////////////////////////////  Установить следующую палитру
          palchg = 2;
          if (gCurrentPaletteNumber < (gGradientPaletteCount - 1))  gCurrentPaletteNumber++;
          else                                                gCurrentPaletteNumber = 0;
          gCurrentPalette = (gGradientPalettes[gCurrentPaletteNumber]);
          DBG_PRINT(F("Palette+ ")); DBG_PRINTLN(gCurrentPaletteNumber);
        break;

      case IR_Key_Palette_Auto :            ///////////////////////////////////////////////////////////////////////////  Включить перебор палитр
        palchg = 3;
        DBG_PRINTLN(F("Continuous palette change"));
        break;

      case IR_Key_Solid_Black :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Черный
        demorun = 0; ledMode = 100; palchg = 0;
        solid = CRGB::Black;
        DBG_PRINTLN(F("Solid_Black"));
        break;

      case IR_Key_Solid_Red :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Красный
        demorun = 0; ledMode = 100; palchg = 0;
        solid = CRGB::Red;
        DBG_PRINTLN(F("Solid_Red"));

        break;

      case IR_Key_Solid_Orange :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Оранжевый
        demorun = 0; ledMode = 100; palchg = 0;
        solid = CRGB::Orange;
        DBG_PRINTLN(F("Solid_Orange"));
        break;

      case IR_Key_Solid_Yellow :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Желтый
        demorun = 0; ledMode = 100; palchg = 0;
        solid = CRGB::Yellow;
        DBG_PRINTLN(F("Solid_Yellow"));
        break;

      case IR_Key_Solid_Green :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Зеленый
        demorun = 0; ledMode = 100; palchg = 0;
        solid = CRGB::Green;
        DBG_PRINTLN(F("Solid_Green"));
        break;

      case IR_Key_Solid_SkyBlue :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Голубой
        demorun = 0; ledMode = 100; palchg = 0;
        solid = CRGB::SkyBlue;
        DBG_PRINTLN(F("Solid_SkyBlue"));
        break;

      case IR_Key_Solid_Blue :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Синий
        demorun = 0; ledMode = 100; palchg = 0;
        solid = CRGB::Blue;
        DBG_PRINTLN(F("Solid_Blue"));
        break;

      case IR_Key_Solid_Violet :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Фиолетовый
        demorun = 0; ledMode = 100; palchg = 0;
        solid = CRGB::Violet;
        DBG_PRINTLN(F("Solid_Violet"));
        break;

      case IR_Key_Solid_White :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Белый
        demorun = 0; ledMode = 100; palchg = 0;
        solid = CRGB::White;
        DBG_PRINTLN(F("Solid_White"));
        break;

      default:     break;                // We could do something by default
    } 
}
