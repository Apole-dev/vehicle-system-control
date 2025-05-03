/* #include <Arduino.h>
#include "can.h"

void setup()
{
    Serial.begin(9600); 
    MCP2515::init(10 , BAUD_125kbps);
}

void loop()
{
    
    // Seri port üzerinden veri alındığında
    if (Serial.available() > 0) {
        String message = Serial.readString();  // Kullanıcının yazdığı mesajı al
        MCP2515::messageTransmit(message);  // Mesajı CAN Bus üzerinden gönder
        Serial.print("Gönderilen Mesaj: ");
        Serial.println(message);  // Kullanıcıya gönderilen mesajı ekrana yazdır
    }
    MCP2515::dataReader();
    MCP2515::errorMessages();
} */