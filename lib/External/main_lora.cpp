/* #include <Arduino.h>
#include <SoftwareSerial.h>
#include "LoRa_config.h"
#include "LoRa.h"

#define MY_RX 2
#define MY_TX 3
#define MY_MO 6 
#define MY_M1 5
#define MY_AUX 4

LoRa_Current_Config* myConfig;
SoftwareSerial* myLoraSerial;
LoRaConfig* loraConfig;
LoRaMain* lora;

void setup() {

    pinMode(MY_MO, OUTPUT);
    pinMode(MY_M1, OUTPUT);
    pinMode(MY_AUX, INPUT);

    myConfig = new LoRa_Current_Config();
    myLoraSerial = new SoftwareSerial(MY_RX, MY_TX);

    myConfig->loraSerial = myLoraSerial;
    myConfig->currentMode = NORMAL;
    myConfig->M0_PIN = MY_MO;
    myConfig->M1_PIN = MY_M1;
    myConfig->RX = MY_RX;
    myConfig->TX = MY_TX;
    myConfig->AUX_PIN = MY_AUX;
    myConfig->saveParameters = false;
    myConfig->adress = 0xFFFF;
    myConfig->pairty = PARITY_NONE;
    myConfig->airDataRate = AIR_DATA_RATE_RATE_2_4k;
    myConfig->baudRate = Baud_9600;
    myConfig->channelFrequency = 1;
    myConfig->fixedTransmissionEnable = false;
    myConfig->IODriveModeEnable = false;
    myConfig->wirlessWakeUpTime = WIRLESS_WAKE_TIME_250;
    myConfig->FECEnable = true;
    myConfig->transmisionPower = TRANSMISSION_POWER_30dBm;

    loraConfig = new LoRaConfig(myConfig); 
    lora = new LoRaMain(myConfig);


    delay(500);
}

void loop() {
    String messageToSend = "TEST";
    lora->SendData(myConfig, messageToSend);

} Sender */


#include <Arduino.h>
#include <SoftwareSerial.h>
#include "LoRa_config.h"
#include "LoRa.h"

#define MY_RX 3
#define MY_TX 2
#define MY_MO 0 
#define MY_M1 1 
#define MY_AUX 4


LoRa_Current_Config* myConfig;
SoftwareSerial* myLoraSerial;
SoftwareSerial* dataWriteSerial; 
LoRaConfig* loraConfig;
LoRaMain* lora;

//time
unsigned long previousMillis = 0;
const long interval = 3000;       
bool isLoraReceivingMode = true;  
String loraMessageBuffer = ""; 

void setup() {

    pinMode(MY_MO, OUTPUT);
    pinMode(MY_M1, OUTPUT);
    pinMode(MY_AUX, INPUT);

    myConfig = new LoRa_Current_Config();
    myLoraSerial = new SoftwareSerial(MY_RX, MY_TX);
    dataWriteSerial = new SoftwareSerial(A2, A3); 
    dataWriteSerial->begin(9600); 

    myConfig->loraSerial = myLoraSerial;
    myConfig->currentMode = NORMAL;
    myConfig->M0_PIN = MY_MO;
    myConfig->M1_PIN = MY_M1;
    myConfig->RX = MY_RX;
    myConfig->TX = MY_TX;
    myConfig->AUX_PIN = MY_AUX;
    myConfig->saveParameters = false;
    myConfig->adress = 0xFFFF;
    myConfig->pairty = PARITY_NONE;
    myConfig->airDataRate = AIR_DATA_RATE_RATE_2_4k;
    myConfig->baudRate = Baud_9600;
    myConfig->channelFrequency = 1;
    myConfig->fixedTransmissionEnable = false;
    myConfig->IODriveModeEnable = false;
    myConfig->wirlessWakeUpTime = WIRLESS_WAKE_TIME_250;
    myConfig->FECEnable = true;
    myConfig->transmisionPower = TRANSMISSION_POWER_30dBm;

    loraConfig = new LoRaConfig(myConfig); 
    lora = new LoRaMain(myConfig);

    myLoraSerial->listen(); 
}

void loop() {
    unsigned long currentTime = millis();
    if (currentTime-previousMillis >= interval)
    {
        previousMillis = currentTime;
        isLoraReceivingMode = !isLoraReceivingMode;
    }
    
    if(isLoraReceivingMode){
        myLoraSerial->listen();
        lora->ReciveData(myConfig, &loraMessageBuffer);
    }
    else if (!isLoraReceivingMode)
    {
        dataWriteSerial->listen();
        dataWriteSerial->print("recived:");
        dataWriteSerial->println(loraMessageBuffer);
        loraMessageBuffer = "";
    }

    delay(50);
}

