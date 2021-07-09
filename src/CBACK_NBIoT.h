
#include <Arduino.h>

class CBACK_NBIoT
{
    private:
        HardwareSerial *_serial;
    public:
        void Init(HardwareSerial *serial = &Serial2, uint8_t RX = 19, uint8_t TX = 22);
        String waitMsg(unsigned long time);
        void sendMsg(String command);
};