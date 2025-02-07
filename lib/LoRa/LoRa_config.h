#ifndef LORA_CONFIG_H
#define LORA_CONFIG_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "common_lora_enums.h"


#define READ_OPERATION_PARAM 0xC1 
#define READ_VERSION_NUMBER  0xC3 
#define RESET                0xC4 


struct LoRa_commands {
    uint8_t HEAD;
    uint8_t ADDH;
    uint8_t ADDL;
    uint8_t SPED;
    uint8_t CHAN;
    uint8_t OPTION;
};

struct LoRa_Current_Config {

    SoftwareSerial* loraSerial = nullptr;

    uint8_t M0_PIN ;
    uint8_t M1_PIN ;
    uint8_t RX ;
    uint8_t TX ;
    uint8_t AUX_PIN;


    LoRa_Mode currentMode;  

    //Command Section
    bool saveParameters;
    uint16_t adress; // First High : Second Low (2)
    PARITY_STATE pairty;
    BAUD_RATE baudRate;
    AIR_DATA_RATE airDataRate;
    uint8_t channelFrequency;
    bool fixedTransmissionEnable;
    bool IODriveModeEnable;
    WIRLESS_WAKE_TIME wirlessWakeUpTime;
    bool FECEnable;
    TRANSMISSION_POWER transmisionPower;

};

class LoRaConfig {
    private:
    public:
        LoRaConfig(LoRa_Current_Config*);
        bool waitForAUX(LoRa_Current_Config*);
        void LoRa_SetMode(LoRa_Current_Config*,LoRa_Mode);
        bool LoRa_SendCommand(LoRa_Current_Config*,LoRa_commands*);
        void LoRa_ReciveCommand(LoRa_Current_Config*, uint8_t*);
};

#endif // LORA_CONFIG_H