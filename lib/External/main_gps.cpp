#include <Arduino.h>
#include "gpsParser.h"
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(8,7);
SoftwareSerial dataReadSerial(A2,A3);


Transform gpsData;


//time
unsigned long previousMillis = 0;
const long interval = 3000;       
bool isGPSReceivingMode = true;  
String gpsMessageBuffer = ""; 


void NextionInit(){
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void setup() {
  //Serial.begin(9600);
  dataReadSerial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
    
    String data;

    unsigned long currentTime = millis();
    if (currentTime-previousMillis >= interval)
    {
        previousMillis = currentTime;
        isGPSReceivingMode = !isGPSReceivingMode;
    }

    
    
    if(isGPSReceivingMode){
      gpsSerial.listen();      
      data = gpsSerial.readStringUntil('\n');
      gpsData = parse(data); 
    }
    if(!isGPSReceivingMode){
      dataReadSerial.listen();

      dataReadSerial.print("\"t0.text=\"");
      dataReadSerial.print(gpsData.time);
      dataReadSerial.print('\n');
      NextionInit();

      dataReadSerial.print("\"t1.text=\"");
      dataReadSerial.println(gpsData.latitude);
      dataReadSerial.print('\n');
      NextionInit();


      dataReadSerial.print("\"t2.text=\"");
      dataReadSerial.println(gpsData.longitude);
      dataReadSerial.print('\n');
      NextionInit();


      dataReadSerial.print("\"t3.text=\"");
      dataReadSerial.println(gpsData.velocity);
      dataReadSerial.print('\n');
      NextionInit();

    }    
}

