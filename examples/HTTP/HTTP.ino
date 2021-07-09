/*
    Description: 
    Use CBACK NBIoT Create HTTP request GET / POST
    Please install library before compiling:  
    M5GFX: https://github.com/m5stack/M5GFX
    M5StickC: https://github.com/m5stack/M5StickC
*/

#include "M5StickC.h"
#include "M5GFX.h"
#include "CBACK_NBIoT.h"

M5GFX display;
M5Canvas canvas(&display);

CBACK_NBIoT DTU;

void showLog(String text)
{
  Serial.print(text);
  canvas.println(text);
  canvas.pushSprite(0, 0);
}

void setup()
{
    M5.begin();

    display.begin();
    canvas.setTextColor(TFT_GREEN);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextScroll(true);
    
    showLog("Reset Module...");
    //SIM7020
    DTU.Init(&Serial2, 36, 26);
    // DTU.Init();
    //Reset Module
    DTU.sendMsg("AT+CRESET\r\n");
    delay(5000);
}

String readstr;

void loop()
{

    DTU.sendMsg("AT+CSMINS=?\r\n");
    readstr = DTU.waitMsg(1000);

    showLog(readstr);

    while(1){
        DTU.sendMsg("AT+CSQ\r\n\r\n");
        readstr = DTU.waitMsg(1000);
    
        if(readstr.indexOf("0,0") ==-1 || readstr.indexOf("99") ==-1 ){
            break;
        }
    }

    DTU.sendMsg("AT+CREG?\r\n");
    readstr = DTU.waitMsg(1000);
    showLog(readstr);

    DTU.sendMsg("AT+COPS?\r\n");
    readstr = DTU.waitMsg(1000);

    showLog(readstr);

    //Create HTTP host instance
    DTU.sendMsg("AT+CHTTPCREATE=\"http://api.m5stack.com/\"\r\n");
    readstr = DTU.waitMsg(5000);

    showLog(readstr);

    //Connect server
    DTU.sendMsg("AT+CHTTPCON=0\r\n");
    readstr = DTU.waitMsg(5000);

    showLog(readstr);

    //HTTP GET
    DTU.sendMsg("AT+CHTTPSEND=0,0,\"/v1\"\r\n");
    readstr = DTU.waitMsg(5000);

    showLog(readstr);

    //HTTP POST
    // DTU.sendMsg("AT+CHTTPSEND=0,1,\"/v1\",48656c6c6f204d352055736572\r\n");
    // readstr = DTU.waitMsg(5000);
    // Serial.print(readstr);

    if(readstr.indexOf("OK") !=-1){
      while(Serial2.available()){
          showLog(Serial2.readString());
          (Serial2.readString());
      }
    }

    DTU.sendMsg("AT+CHTTPDISCON=0\r\n");
    DTU.sendMsg("AT+CHTTPDESTROY=0\r\n");
    readstr = DTU.waitMsg(1000);
    showLog(readstr);

}
