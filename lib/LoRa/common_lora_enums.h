#ifndef COMMON_LORA_H
#define COMMON_LORA_H

#include<Arduino.h>

enum LoRa_Mode {
    NORMAL,
    WAKE_UP,
    POWER_SAVE,
    SLEEP,
    ERROR,
};

/* SPED (1 Byte): PARITY_STATE , BAUD_RATE , AIR_DATA_RATE */

//7-6 BITS
enum PARITY_STATE{
    PARITY_NONE = 0x00, 
    PARITY_ODD  = 0x01,
    PARITY_EVEN = 0x02,
};

//5-4-3 BITS
enum BAUD_RATE{
    Baud_1200   = 0x00,
    Baud_2400   = 0x01,
    Baud_4800   = 0x02,
    Baud_9600   = 0x03,
    Baud_19200  = 0x04,
    Baud_38400  = 0x05,
    Baud_57600  = 0x06,
    Baud_115200 = 0x07
};

//2-1-0 BITS
enum AIR_DATA_RATE{
    AIR_DATA_RATE_RATE_0_3k     = 0x00,
    AIR_DATA_RATE_RATE_1_2k     = 0x01,
    AIR_DATA_RATE_RATE_2_4k     = 0x02,    
    AIR_DATA_RATE_RATE_4_8k     = 0x03,
    AIR_DATA_RATE_RATE_9_6k     = 0x04,
    AIR_DATA_RATE_RATE_19_2k    = 0x05,
};

/* OPTION (1 Byte) : */

//5-4-3 BITS
enum WIRLESS_WAKE_TIME{
    
    FAST_SPEED                  = 0x00,  // 250
    DEFAULT_SPEED               = 0x03,  // 1000
    SLOW_SPEED                  = 0x07,  // 2000

    WIRLESS_WAKE_TIME_250       = 0x00,
    WIRLESS_WAKE_TIME_500       = 0x01,
    WIRLESS_WAKE_TIME_750       = 0x02,
    WIRLESS_WAKE_TIME_1000      = 0x03,
    WIRLESS_WAKE_TIME_1250      = 0x04,
    WIRLESS_WAKE_TIME_1500      = 0x05,
    WIRLESS_WAKE_TIME_1750      = 0x06,
    WIRLESS_WAKE_TIME_2000      = 0x07,

};

enum TRANSMISSION_POWER{
    TRANSMISSION_POWER_30dBm = 0x01,
    TRANSMISSION_POWER_27dBm = 0x02,
    TRANSMISSION_POWER_24dBm = 0x03,
    TRANSMISSION_POWER_21dBm = 0x04,
};

#endif