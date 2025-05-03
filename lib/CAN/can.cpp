#include "can.h"
#include <SPI.h>
#include <Arduino.h>
uint8_t csPin_g;
uint8_t temperrorid = 13;
String MCP2515::receivedMessage;
String MCP2515::tempMessage;
// mcp2515 initial functions

void MCP2515::init(uint8_t csPin, uint8_t baudRate)
{
    pinMode(10,OUTPUT);
    pinMode(csPin,OUTPUT);
    digitalWrite(csPin,1);
    csPin_g = csPin;
    //Serial.begin(9600);
    SPI.begin();
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    MCP2515::select(csPin);
    MCP2515::reset();
    MCP2515::bitRate(baudRate);
    MCP2515::normalMode();
    MCP2515::deSelect(csPin);
    delay(100);
    MCP2515::errorMessages();
    Serial.println("MCP2515 has been initialized successfully.");
}
void MCP2515::reset()
{
    SPI.transfer(0xC0);
    delay(50);
}
void MCP2515::bitRate(uint8_t baudRate)
{
    switch (baudRate)
    {
    case BAUD_10kbps:
    writeRegister(0x2A,0x63);
        break;
    case BAUD_20kbps:
    writeRegister(0x2A,0x31);
        break;
    case BAUD_50kbps:
    writeRegister(0x2A,0x13);
        break;
    case BAUD_100kbps:
    writeRegister(0x2A,0x09);
        break;
    case BAUD_125kbps:
    writeRegister(0x2A,0x07);
        break;
    case BAUD_250kbps:
    writeRegister(0x2A,0x03);
        break;
    case BAUD_500kbps:
    writeRegister(0x2A,0x01);
        break;
    case BAUD_1000kbps:
    writeRegister(0x2A,0x00);
        break;
    }
    writeRegister(0x29, 0xD0); 
    writeRegister(0x28, 0x82);
}
void MCP2515::normalMode()
{
    writeRegister(0x0F, 0x00);
}

//messagetransmit , messagereceive functions

void MCP2515::messageTransmit(String input)
{
    int j = 0;
    MCP2515::writeRegister(0x31, 0x7D);  
    MCP2515::writeRegister(0x32, 0xF0);  
    MCP2515::writeRegister(0x35, 0x08);
    String modifiedInput = "";
    modifiedInput = input + '|';
    int lenght = modifiedInput.length();
    while(lenght > j*8)
    {
        for (size_t i = 0; i < 8; i++)
        {
            MCP2515::writeRegister(54+i , modifiedInput[i+8*j]);
        }
        MCP2515::writeRegister(0x30, 0x08);
        delay(5);
        j += 1;
    }
}

String received8byte;
void MCP2515::dataReader() {
    uint8_t intFlags = MCP2515::readRegister(0x2C); // Kesme bayraklarını oku
    int test = 0;
    if (intFlags & 0x01)  // RX0IF bayrağı ayarlandıysa
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            char character = (char)MCP2515::readRegister(0x66 + i);
            if(character == '|')
            {
                test = 1;
                break;
            }
            else
            {
                received8byte += character;
            }
        }
        receivedMessage += received8byte;
        if(test == 1)
        {
            Serial.println(receivedMessage);
            tempMessage = receivedMessage;
            receivedMessage = "";
            test = 0;
        }
        received8byte = "";
        MCP2515::writeRegister(0x2C, intFlags & ~0x01);
    }
    if (intFlags & 0x02)  // RX1IF bayrağı ayarlandıysa
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            char character = (char)MCP2515::readRegister(0x66 + i);
            if(character == '|')
            {
                test = 1;
                break;
            }
            else
            {
                received8byte += character;
            }        
        }
        receivedMessage += received8byte;
        if(test == 1)
        {
            Serial.println(receivedMessage);
            tempMessage = receivedMessage;
            receivedMessage = "";
            test = 0;
        }
        received8byte = "";
        MCP2515::writeRegister(0x2C, intFlags & ~0x02);
    }    
}



//csPin function
 
void MCP2515::select(uint8_t csPin)
{
    digitalWrite(csPin , LOW);
}
void MCP2515::deSelect(uint8_t csPin)
{
    digitalWrite(csPin , HIGH);
}

//register functions
void MCP2515::writeRegister(uint8_t reg, uint8_t val) 
{
    MCP2515::select(csPin_g);    
    SPI.transfer(0x02);       
    SPI.transfer(reg);          
    SPI.transfer(val);          
    MCP2515::deSelect(csPin_g); 
}
uint8_t MCP2515::readRegister(uint8_t reg_r) 
{
    MCP2515::select(csPin_g);   
    SPI.transfer(0x03);         
    SPI.transfer(reg_r);        
    uint8_t val = SPI.transfer(0x00); 
    MCP2515::deSelect(csPin_g); 
    return val;                 
}

//error function
void MCP2515::errorMessages()
{
    uint8_t errorid = MCP2515::readRegister(0x2D);
    if(temperrorid != errorid)
    {
    if(errorid == 0)
    {
        Serial.println("No Error Found.");
    }
    else if(errorid == 1)
    {
        Serial.println("General Error.");
    }
    else if(errorid == 2)
    {
        Serial.println("Receive Error.");
    }
    else if(errorid == 4)
    {
        Serial.println("Transmit Error");
    }
    else if(errorid == 8)
    {
        Serial.println("Receive Error-Passive");
    }
    else if(errorid == 16)
    {
        Serial.println("Transmit Error-Passive");
    }
    else if(errorid == 32)
    {
        Serial.println("Bus Off - Restarting");
    }
    else if(errorid == 64)
    {
        Serial.println("Receive Buffer 0 Overflow");
    }
    else if(errorid == 128)
    {
        Serial.println("Receive Buffer 1 Overflow");
    }
    }
        temperrorid = errorid;
}