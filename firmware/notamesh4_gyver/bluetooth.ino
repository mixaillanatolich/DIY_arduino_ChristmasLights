
#define PARSE_AMOUNT 8    // максимальное количество значений в массиве, который хотим получить
#define HEADER_SYMBOL '$'        // стартовый символ

//int intData[PARSE_AMOUNT];     // массив численных значений после парсинга
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

uint8_t byte_convert[255];
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

String successResponse() {
      String response = "";
      response += (char)0x24; //cmd
      response += (char)0x01; //len
      response += (char)0x01; //response
      return response;
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
    switch (byte_convert[0]) {
        case 0:   // ping
            btSerial.print(successResponse());
            break;
        case 2:
//            Command = 0x0;
//            Command |= ((uint32_t)byte_convert[4]) << 24;
//            Command |= ((uint32_t)byte_convert[3]) << 16;
//            Command |= ((uint32_t)byte_convert[2]) << 8;
//            Command |= ((uint32_t)byte_convert[1]);
            Command = (uint32_t)byte_convert[1];
            handleControlCmd();
            btSerial.print(successResponse());
            break;
        case 3:
            int mode = (int)byte_convert[1];
            DBG_PRINT("new mode: ");
            DBG_PRINTLN(mode);
            SetMode(mode);
            btSerial.print(successResponse());
            break;
    }
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
        uint8_t incomingByte = btSerial.read();      // обязательно ЧИТАЕМ входящий символ
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
                getStarted = false;
                recievedFlag = true;
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
