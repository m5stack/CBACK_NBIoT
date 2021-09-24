/*
    Description: 
    Use CBACK NBIoT to connect to the MQTT server, and implement subscription and publishing messages.
    Check the status through Serial. When the MQTT connection is successful, Click BtnA Public Topic
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
    
        if(readstr.indexOf("0,0") ==-1 && readstr.indexOf("99") ==-1 ){
            break;
        }
    }

    DTU.sendMsg("AT+CREG?\r\n");
    readstr = DTU.waitMsg(1000);

    showLog(readstr);

    DTU.sendMsg("AT+COPS?\r\n");
    readstr = DTU.waitMsg(1000);

    showLog(readstr);

    //Create MQTT connection
    //If succeed, MQTT id will return.
    DTU.sendMsg("AT+CMQNEW=\"broker.emqx.io\",\"1883\",12000,1024\r\n");
    readstr = DTU.waitMsg(5000);

    showLog(readstr);

    DTU.sendMsg("AT+CMQCON=0,3,\"myclient\",600,1,0\r\n");
    readstr = DTU.waitMsg(5000);

    showLog(readstr);

    if(readstr.indexOf("OK") !=-1){
        DTU.sendMsg("AT+CMQSUB=0,\"mytopic\",1\r\n");
        readstr = DTU.waitMsg(5000);
    
        showLog(readstr);
        
        while(1){
            M5.update();
            readstr = DTU.waitMsg(0);
        
            showLog(readstr);
            //Click BtnA Public Topic
            if(M5.BtnA.wasPressed()){
                DTU.sendMsg("AT+CMQPUB=0,\"mytopic\",1,0,0,8,\"31323334\"\r\n");
                readstr = DTU.waitMsg(5000);
            
                showLog(readstr);
                if(readstr.indexOf("ERR") !=-1) {
                    ESP.restart();
                }
            }
        }
    }
}
