#include <Arduino.h>
#include "LoRa_config.h"
#include "LoRa.h"



#define MY_RX 2
#define MY_TX 3
#define MY_MO 4
#define MY_M1 5
#define MY_AUX 8

LoRa_Current_Config* myConfig;
SoftwareSerial* myLoraSerial;
LoRaConfig* loraConfig;
LoRaMain* lora;

void setup() {
    Serial.begin(9600);
    
    pinMode(MY_MO,OUTPUT); 
    pinMode(MY_M1,OUTPUT);
    pinMode(MY_AUX,INPUT);

    myConfig = new LoRa_Current_Config();
    myLoraSerial = new SoftwareSerial(MY_RX, MY_TX); 
    
    myConfig->loraSerial  = myLoraSerial;
    myConfig->currentMode = NORMAL; 

    //Pinouts
    myConfig->M0_PIN = MY_MO;
    myConfig->M1_PIN = MY_M1;
    myConfig->RX = MY_RX;       
    myConfig->TX = MY_TX;
    myConfig->AUX_PIN = MY_AUX;

    //Command Section
    myConfig->saveParameters            = false;
    myConfig->adress                    = 0xFFFF;
    myConfig->pairty                    = PARITY_NONE;
    myConfig->airDataRate               = AIR_DATA_RATE_RATE_2_4k;
    myConfig->baudRate                  = Baud_9600;
    myConfig->channelFrequency          = 1;
    myConfig->fixedTransmissionEnable   = false;
    myConfig->IODriveModeEnable         = false;
    myConfig->wirlessWakeUpTime         = WIRLESS_WAKE_TIME_250;
    myConfig->FECEnable                 = true;
    myConfig->transmisionPower          = TRANSMISSION_POWER_30dBm;
    
    lora = new LoRaMain(myConfig);
    loraConfig = new LoRaConfig(myConfig);

    delay(100);

}

void loop() {

/*Send right
  Recive left  */

 //Sending data
    if(Serial.available()>0){
        String data = Serial.readString();
        lora->SendData(myConfig,data);
    } 

   
    
/*  // Receiving data
    String* receivedMessage = new String();
    lora->ReciveData(myConfig, receivedMessage);
    Serial.print("Received: ");
    Serial.println(*receivedMessage);
    MCP2515::messageTransmit(*receivedMessage);
    delete receivedMessage;  */
 
}



