
#define HEADER_SYMBOL '$'        // стартовый символ

boolean recievedFlag;
boolean getStarted;

uint32_t parseTimer;
uint16_t parseWaitPeriod = 2*1000; // wait 2 sec

uint8_t request[255];
byte byte_index = 0;
byte expected_bytes = 0;


void sendCommand(uint8_t cmd[], uint8_t len) {
#if HARDWARE_BT_SERIAL
    Serial.write(cmd,len);
#else
    btSerial.write(cmd,len);
#endif
}

void sendSettingsUpdate() {
    sendSettings(true);
}

void sendSettings() {
    sendSettings(false);
}

void sendSettings(bool isNotification) {
    uint8_t cmd[7];
    cmd[0] = isNotification ? 0xA4 : 0xA5;
    cmd[1] = 0x05;
    cmd[2] = StepMode == 1 ? newMode : ledMode;
    cmd[3] = demorun;
    cmd[4] = glitter;
    cmd[5] = background;
    cmd[6] = candle;
    sendCommand(cmd,cmd[1]+2);
}

void successResponse() {
    uint8_t cmd[3]; 
    cmd[0] = 0xA0;
    cmd[1] = 0x01;
    cmd[2] = 0x01;
    sendCommand(cmd,cmd[1]+2);
}

void bluetoothTick() {
  parsing();               // функция парсинга
  if (recievedFlag) {     // если получены данные
    recievedFlag = false;

    /*
    for (byte i = 0; i < index; i++) {
      DBG_PRINT(request[i]);
      DBG_PRINT(" ");
    } 
    DBG_PRINTLN(" ");
    for (byte i = 0; i < index; i++) {
      DBG_PRINT(request[i],HEX);
      DBG_PRINT(" ");
    } 
    DBG_PRINTLN("===========");
    */

    bool needSendSuccessResponse = false;
    bool needSendSettingsResponse = false;
    switch (request[0]) {
        case 0:   // ping
            needSendSuccessResponse = true;
            break;
        case 1:   // ping
            needSendSettingsResponse = true;
            break;
        case 2:
            Command = (uint32_t)request[1];
            handleControlCmd();
            needSendSettingsResponse = true;
            break;
        case 3:
            int modeId = (int)request[1];
            DBG_PRINT("new mode: ");
            DBG_PRINTLN(modeId);
            SetMode(modeId);
            needSendSuccessResponse = true;
            break;
    }

    if (needSendSuccessResponse) {
        successResponse();
    }

    if (needSendSettingsResponse) {
        sendSettings();
    }
    
  }
}

void parsing() {

    // the check for make sure we are not freazing in the case wrong message
    if (getStarted) {
        if ((millis() - parseTimer >= parseWaitPeriod)) {
            parseTimer = millis();
            getStarted = false;
            expected_bytes = 0;
            byte_index = 0;
            DBG_PRINTLN("Reset parse msg by timeout");
        }
    }

#if HARDWARE_BT_SERIAL
    if (Serial.available() > 0) {
        uint8_t incomingByte = Serial.read();      // обязательно ЧИТАЕМ входящий символ
#else
    if (btSerial.available() > 0) {
        uint8_t incomingByte = btSerial.read();      // обязательно ЧИТАЕМ входящий символ
#endif
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
                request[byte_index] = incomingByte;
                byte_index++;
            }

//            DBG_PRINT("byte_index: ");
//            DBG_PRINTLN(byte_index);

            if (expected_bytes == byte_index) {
                getStarted = false;
                recievedFlag = true;
            }
        } else {
            if (incomingByte == HEADER_SYMBOL) {
                getStarted = true;
                expected_bytes = 0;
                byte_index = 0;
            }
        }
    }
}
