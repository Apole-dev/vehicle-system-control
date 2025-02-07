#ifndef LoRa_H
#define LoRa_H

#include<Arduino.h>

class LoRaMain
{
    private:
        uint8_t recivedBuffer[58] = {0};
        int recivedBufferLength = 58;
        void SeparateAdress(uint16_t,uint8_t*);
        void StringToInt(String,uint16_t*,int);
        void IntToString(String*);
    public:
        LoRaMain(LoRa_Current_Config*);
        void SetTransmissionPower(LoRa_Current_Config*,TRANSMISSION_POWER);
        void SetFecSwitch(LoRa_Current_Config*,bool);
        void SetWirlessWakeUpTime(LoRa_Current_Config*,WIRLESS_WAKE_TIME);
        void SetIODriveMode(LoRa_Current_Config*,bool);
        void SetFixedTransmission(LoRa_Current_Config*,bool);
        void SetChannelFrequency(LoRa_Current_Config*,uint8_t);
        void SetAirDataRate(LoRa_Current_Config*,AIR_DATA_RATE);
        void SetBaudRate(LoRa_Current_Config*,BAUD_RATE);
        void SetParity(LoRa_Current_Config*,PARITY_STATE);
        void SetBrodcastTransmission(LoRa_Current_Config*,bool);
        void SendData(LoRa_Current_Config*,uint8_t*,uint8_t);
        void SendData(LoRa_Current_Config*,const String&);
        int ReciveData(LoRa_Current_Config*,uint8_t*,uint8_t);
        void ReciveData(LoRa_Current_Config*,String*);

        bool isReading;
        bool isWriting;

};

#endif