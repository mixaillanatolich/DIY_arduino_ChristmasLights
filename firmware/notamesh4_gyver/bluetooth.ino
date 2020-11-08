
#define PARSE_AMOUNT 8    // максимальное количество значений в массиве, который хотим получить
#define HEADER_SYMBOL '$'        // стартовый символ

int intData[PARSE_AMOUNT];     // массив численных значений после парсинга
boolean recievedFlag;
String request = "1234567891011121314151617181920";
boolean getStarted;
byte index;
String string_convert = "";
int16_t presetNum;   // номер пресета
int16_t modeNum = 0;     // номер режима
//int presetSettings[6];    // текущие настройки пресета
//boolean ONflag = true;

uint32_t parseTimer;
uint16_t parseWaitPeriod = 2*1000; // wait 2 sec

byte byte_convert[255];
byte byte_index = 0;
byte expected_bytes = 0;

void sendSettings() {
  request = "";
  
  request += (char)0x55;
  request += (char)presetNum;
  request += (char)modeNum;

  for (byte i = 0; i < 5; i++) {
//    byte b1 = (presetSettings[i]>>8);
//    byte b2 = (presetSettings[i]);
//    request += (char) b2;
//    request += (char) b1;

    /*
    int len = request.length();
    for (int i = 0; i < len; i++) {
      DBG_PRINT(request.charAt(i),DEC);
      DBG_PRINT(" ");
    }
    DBG_PRINTLN(" ");
    */
  }

 // request += (char)ONflag;
  btSerial.print(request);

  /*
  DBG_PRINTLN("");
  DBG_PRINTLN(request);
  int len2 = request.length();
  for (int i = 0; i < len2; i++) {
    DBG_PRINT(request.charAt(i),DEC);
    DBG_PRINT(" ");
  }
  */
}

void bluetoothTick() {
  parsing();               // функция парсинга
  if (recievedFlag) {     // если получены данные
    recievedFlag = false;

    /*
    for (byte i = 0; i < index; i++) {
      DBG_PRINT(intData[i]);
      DBG_PRINT(" ");
    } 
    DBG_PRINTLN(" ");
    for (byte i = 0; i < index; i++) {
      DBG_PRINT(intData[i],HEX);
      DBG_PRINT(" ");
    } 
    DBG_PRINTLN("===========");
    */

    switch (intData[0]) {
        case 0:
            Command = 0x0;
            Command |= ((int)byte_convert[2]) << 24;
            Command |= ((int)byte_convert[3]) << 16;
            Command |= ((int)byte_convert[4]) << 8;
            Command |= ((int)byte_convert[5]);
            handleControlCmd();
            break;
        case 1:
            int mode = (int)byte_convert[2];
            SetMode(mode);
            break;
    }
    
    /*
    switch (intData[0]) {
      case 0:   // запрос онлайна
        request = "OK ";
        request += String(batPerc);
        btDBG_PRINT(request);
        break;
      case 1:   // запрос состояния (настройки, пресет)
        sendSettings();
        btnControl = false;
        break;
      case 2:   // применить настройки
        for (byte i = 0; i < 6; i++) {
          presetSettings[i] = intData[i + 1];
        }
        presetSettings[setAmount[modeNum] - 1] = intData[6]; // белый
        settingsChanged = true;

        if (intData[7] != 10) invSet = intData[7];
        else invSet = setAmount[modeNum] - 1; // ой костыли бл*ть
        navPos = 2;
        invFlag = true;
        drawSettings();
        changeFlag = true;
        btnControl = false;
        eeprFlag = true;
        eeprTimer = millis();
        break;
      case 3:   // смена пресета
        changePresetTo(intData[1]);
        sendSettings();
        btnControl = false;
        eeprFlag = true;
        eeprTimer = millis();
        break;
      case 4:   // смена режима
        modeNum = intData[1];
        changeMode();
        sendSettings();
        btnControl = false;
        eeprFlag = true;
        eeprTimer = millis();
        break;
      case 5:   // вкл/выкл
        if (intData[1]) LEDon();
        else LEDoff();
        btnControl = false;
        break;
    }
    */
  }
}

void parsing() {

    // the check for make sure we are not freazing in the case wrong message
    if (getStarted) {
        if ((millis() - parseTimer >= parseWaitPeriod)) {
            parseTimer = millis();
            getStarted = false;
            index = 0;
            expected_bytes = 0;
            byte_index = 0;
            DBG_PRINTLN("Reset parse msg by timeout");
        }
    }
  
    if (btSerial.available() > 0) {
        char incomingByte = btSerial.read();      // обязательно ЧИТАЕМ входящий символ
        parseTimer = millis();

        DBG_PRINT("b: ");
        DBG_PRINT(incomingByte,HEX);
        DBG_PRINT(" - ");
        DBG_PRINTLN(incomingByte);
    
        if (getStarted) {
            if (expected_bytes == 0) {
                expected_bytes = incomingByte;
                DBG_PRINT("expected_bytes: ");
                DBG_PRINTLN(expected_bytes);
            } else {
                byte_convert[byte_index] = incomingByte;
                byte_index++;
            }

            DBG_PRINT("byte_index: ");
            DBG_PRINTLN(byte_index);

            if (expected_bytes == byte_index) {
                for (byte i = 0; i < expected_bytes; i++) {
                    intData[i] = byte_convert[i];
                    index++;
                }
                getStarted = false;
                recievedFlag = true; 
        /*
        if (byte_index == 1) {
          intData[0] = byte_convert[0];
          index = 1;
        } else if (byte_index == 2) {
          intData[0] = byte_convert[0];
          intData[1] = byte_convert[1];
          index =2;
        }

        if (byte_index == 12) {
          intData[0] = byte_convert[0];
          intData[1] = byte_convert[1];
          intData[2] = 0;
          intData[2] |= ((int)byte_convert[2]) << 8;
          intData[2] |= ((int)byte_convert[3]);
          intData[3] = 0;
          intData[3] |= ((int)byte_convert[4]) << 8;
          intData[3] |= ((int)byte_convert[5]);
          intData[4] = 0;
          intData[4] |= ((int)byte_convert[6]) << 8;
          intData[4] |= ((int)byte_convert[7]);
          intData[5] = 0;
          intData[5] |= ((int)byte_convert[8]) << 8;
          intData[5] |= ((int)byte_convert[9]);
          
          intData[6] = byte_convert[10];
          intData[7] = byte_convert[11];
          index = 7;
        }
        */
                   
            }
        } else {
            if (incomingByte == HEADER_SYMBOL) {
                getStarted = true;
                index = 0;
                expected_bytes = 0;
                byte_index = 0;
            }
        }
    }
}
