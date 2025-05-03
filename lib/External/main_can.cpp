#include <Arduino.h>
#include "can.h"

void setup()
{
    Serial.begin(9600); 
    MCP2515::init(6 , BAUD_125kbps);

}

void loop()
{ 
    MCP2515::messageTransmit(1,2,3,4,5,6,7,8);
    MCP2515::messageTransmit(9,10,11,12,13,14,15,16);
    MCP2515::messageTransmit(17,18,19,20,21,22,23,24);
    MCP2515::messageTransmit(25,26,27,28,29,30,31,32);
    MCP2515::messageTransmit(33,34,35,36,37,38,39,40);
    MCP2515::errorMessages(); 
    /* int* array = MCP2515::dataArrayReader();
    
     for(int i = 0; i < 8; i++)
     {
         Serial.println(*(array+i));
     }
     delete[] array; 
     delay(50); */

}