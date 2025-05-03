#include <Arduino.h>
class MCP2515
{
    public:   
        
        // mcp2515 initial functions

        static void init(uint8_t csPin , uint8_t baudRate);
        static void reset();
        static void bitRate(uint8_t baudRate);
        static void normalMode();
        static uint8_t csPinSlave;
        static uint8_t csPin;
        static String receivedMessage;
        static String tempMessage;

        // message functions

        static void messageTransmit(uint8_t i1 , uint8_t i2 , uint8_t i3 , uint8_t i4,uint8_t i5 , uint8_t i6 , uint8_t i7 , uint8_t i8);
        static void messageTransmit(String);
        static void dataStringReader();
        static int dataIntReader();
        static int* dataArrayReader();

        // cspin functions
        
        static void select(uint8_t);
        static void deSelect(uint8_t);

        // register functions

        static void writeRegister(uint8_t reg , uint8_t val);
        static uint8_t readRegister(uint8_t reg_r);

        // error function

        static void errorMessages();
        

};
enum baudRate{
    BAUD_10kbps,
    BAUD_20kbps,
    BAUD_50kbps,
    BAUD_100kbps,
    BAUD_125kbps,
    BAUD_250kbps,
    BAUD_500kbps,
    BAUD_1000kbps,
};