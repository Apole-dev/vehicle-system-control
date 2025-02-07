#include <Arduino.h>
#include "gpsParser.h"
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(6,7);


void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  while (gpsSerial.available()) {
    String data = gpsSerial.readStringUntil('\n');

    Transform gpsData = parse(data); 

    Serial.println(gpsData.time);
    Serial.println(gpsData.latitude);
    Serial.println(gpsData.longitude);
    Serial.println(gpsData.velocity);

  }
}