/* #include <Arduino.h>
#include "LoRa_config.h"
#include "LoRa.h"
#include "gpsParser.h"
#include "can.h"


#define MY_RX 3
#define MY_TX 2
#define MY_MO 0 
#define MY_M1 1 
#define MY_AUX 4

LoRa_Current_Config* myConfig;
SoftwareSerial* myLoraSerial;
LoRaConfig* loraConfig;
LoRaMain* lora;

SoftwareSerial gpsSerial(8,7);
//SoftwareSerial readDataSerial(A2,A3);

unsigned long previousMillis = 0;
const long interval = 3000;       
bool modeSwitch = false; //gps -can  

Transform gpsData;



void setup() {    
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
    MCP2515::init(6 , BAUD_125kbps);
    gpsSerial.begin(9600);
    //readDataSerial.begin(9600);

    delay(100);

}

void loop() {
    MCP2515::dataReader();
    
    String data;

    unsigned long currentMilis = millis();
    if (currentMilis-previousMillis >= interval)
    {
        previousMillis = currentMilis;
        modeSwitch = !modeSwitch;
    }

    if(!modeSwitch) //gps
    {
        gpsSerial.listen();
        gpsData;
        data = gpsSerial.readStringUntil('\n');
        gpsData = parse(data); 
    }
    if (modeSwitch)
    {
        myLoraSerial->listen();
        if(MCP2515::tempMessage != NULL)
            lora->SendData(myConfig,MCP2515::tempMessage);//mcp data
        
        //gps data
        lora->SendData(myConfig,gpsData.time);
        lora->SendData(myConfig,gpsData.latitude);
        lora->SendData(myConfig,gpsData.longitude);
        lora->SendData(myConfig,gpsData.velocity);
    }

} */







#include <Arduino.h>
#include <SoftwareSerial.h>
#include "LoRa_config.h"
#include "LoRa.h"

#define MY_RX 3
#define MY_TX 2
#define MY_MO 5 
#define MY_M1 6 
#define MY_AUX 4


LoRa_Current_Config* myConfig;
SoftwareSerial* myLoraSerial;
LoRaConfig* loraConfig;
LoRaMain* lora;

String loraMessageBuffer = "";

void NextionInit(){
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
  }

void setup() {

    pinMode(MY_MO, OUTPUT);
    pinMode(MY_M1, OUTPUT);
    pinMode(MY_AUX, INPUT);

    myConfig = new LoRa_Current_Config();
    myLoraSerial = new SoftwareSerial(MY_RX, MY_TX);
    Serial.begin(9600); 

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
    

        lora->ReciveData(myConfig, &loraMessageBuffer);

        Serial.print("n4.val=");
        Serial.print(loraMessageBuffer);
        //Serial.print('\n');
        NextionInit();
        loraMessageBuffer = "";
}






