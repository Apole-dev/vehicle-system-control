#include "LoRa_config.h"
#include "LoRa.h"
#include "common_lora_enums.h"

#pragma region Command Operations

LoRaConfig::LoRaConfig(LoRa_Current_Config* config) {
   //TODO conf veriables ok check
    if(config == nullptr){
        Serial.println("Config not available");
    }

    //todo not 9600
    config->loraSerial->begin(9600);
    LoRa_SetMode(config,config->currentMode);


    Serial.print("LoRa mode set to: ");
    if (config->currentMode == SLEEP) Serial.println("SLEEP");
    else if (config->currentMode == NORMAL) Serial.println("NORMAL");
    else if (config->currentMode == WAKE_UP) Serial.println("WAKE_UP");
    else if (config->currentMode == POWER_SAVE) Serial.println("POWER_SAVE");
    else Serial.println("ERROR");

}

bool LoRaConfig::waitForAUX(LoRa_Current_Config* config){
    unsigned long startTime = millis();
    while(digitalRead(config->AUX_PIN) == LOW) {
        if(millis() - startTime > 1000) { // 1 second timeout
            Serial.println("AUX timeout");
            return false;
        }
    }
    delay(2); // Wait 2ms after AUX goes high
    return true;
}

void LoRaConfig::LoRa_SetMode(LoRa_Current_Config* config,LoRa_Mode mode) {
    if(!waitForAUX(config)) {
        Serial.println("return flase");
        return;
    }
    switch (mode) {
        case NORMAL:
            digitalWrite(config->M0_PIN, LOW);
            digitalWrite(config->M1_PIN, LOW);
            break;
        case WAKE_UP:
            digitalWrite(config->M0_PIN, HIGH);
            digitalWrite(config->M1_PIN, LOW);
            break;
        case POWER_SAVE:
            digitalWrite(config->M0_PIN, LOW);
            digitalWrite(config->M1_PIN, HIGH);
            break;
        case SLEEP:
            digitalWrite(config->M0_PIN, HIGH);
            digitalWrite(config->M1_PIN, HIGH);
            break;
        default:
            config->currentMode = ERROR;
    }
}

bool LoRaConfig::LoRa_SendCommand(LoRa_Current_Config* config,LoRa_commands* myCommand) {
    if (myCommand == nullptr) {
        Serial.println("Command structure is null");
        return false;
    } 
    if (config->currentMode != SLEEP) {
        Serial.println("Command can only be sent in sleep mode");
        return false;
    }

    uint8_t command[6] = {
        myCommand->HEAD,
        myCommand->ADDH,
        myCommand->ADDL,
        myCommand->SPED,
        myCommand->CHAN,
        myCommand->OPTION
    };

    config->loraSerial->write(command, 6);
    return true;
}

void LoRaConfig::LoRa_ReciveCommand(LoRa_Current_Config* config, uint8_t* outputCommand) {
    if (config->currentMode != SLEEP) return; //work on sleep mode  

    unsigned long startTime = millis();
    while (config->loraSerial->available() < 6) {
        if (millis() - startTime > 2000) return; 
        Serial.println("No response from lora module.");
    }

    config->loraSerial->readBytes(outputCommand, 6);
}

#pragma endregion

#pragma region Additional Functions

void LoRaMain::IntToString(String* responseString){
    *responseString = "";  
    for (uint8_t i = 0; i < recivedBufferLength; i++) {
        responseString->concat((char)recivedBuffer[i]);  // Convert ASCII back to characters
    }
}

void LoRaMain::StringToInt(String contex,uint16_t* outputArray,int length){
    for (int i = 0; i < length; i++) {
        outputArray[i] = (uint16_t) contex[i];  // ASCII değerini dönüştür
    }
}

void LoRaMain::SeparateAdress(uint16_t adress,uint8_t* result){
    result[0] = (adress >> 8) & 0xFF;  // High byte
    result[1] = (adress >> 0) & 0xFF;  // Low byte

}

#pragma endregion

#pragma region Send Data 

void LoRaMain::SendData(LoRa_Current_Config* config, const String& contex) {
    int length = contex.length();

    if (length > 0) {
        if (length > 57) { 
            Serial.println("Warning: String too long, truncating.");
            length = 57; 
        }

        uint8_t buffer[58]; 

        buffer[0] = (uint8_t)length; 
        for (int i = 0; i < length; i++) {
            buffer[i + 1] = (uint8_t)contex[i]; 
        }

        SendData(config, buffer, length + 1); 
    } else {
        Serial.println("Warning: Sending empty string.");
    }
}


void LoRaMain::SendData(LoRa_Current_Config* config,uint8_t* tData,uint8_t length){
    Serial.println("-----Data sending------");

    if (!config->loraSerial) {
        Serial.println("Error: LoRa serial is not initialized!");
        return;
    }
    if(length >58){
        Serial.println("length can not be bigger than 58 byte at once");
        length = 58;
        return;
    }
    
    for (size_t i = 0; i < length; i++)
    {
        config->loraSerial->write(tData[i]);
    }
    
}

#pragma endregion

#pragma region Recive Data

void LoRaMain::ReciveData(LoRa_Current_Config* config, String* contex) {
    uint8_t lengthBuffer[1]; 
    int bytesRead = ReciveData(config, lengthBuffer, 1); 

    if (bytesRead == 1) {
        int messageLength = lengthBuffer[0]; 

        if (messageLength > 0 && messageLength <= 57) { 
            uint8_t messageBuffer[57]; 
            bytesRead = ReciveData(config, messageBuffer, messageLength); 

            if (bytesRead == messageLength) {
                recivedBufferLength = messageLength; 
                memcpy(recivedBuffer, messageBuffer, messageLength); 
                IntToString(contex); 
                return; 
            } else {
                //Serial.println("Error: Incomplete message data received.");
            }
        } else {
            //Serial.println("Error: Invalid message length received.");
        }
    } else {
        //Serial.println("Error: Could not receive message length.");
    }
    *contex = "";
}

int LoRaMain::ReciveData(LoRa_Current_Config* config, uint8_t* rData, uint8_t length) {
    unsigned long startTime = millis();
    int bytesRead = 0;

    while (config->loraSerial->available() < length) {
        if (millis() - startTime > 1000) {
            //Serial.println("Receive Timeout: Not enough data available for now");
            return 0;
        }
    }

    bytesRead = config->loraSerial->readBytes(rData, length);
    if (bytesRead < length) {
        Serial.print("Warning: Received fewer bytes than requested: ");
        Serial.println(bytesRead);
    }
    return bytesRead; // Gerçekte okunan byte sayısını döndür
}

#pragma endregion

#pragma region LoRa Command Update

LoRaMain::LoRaMain(LoRa_Current_Config* config){
    
    uint8_t command[6] = {0,0,0,0,0,0};
    uint8_t* result = new uint8_t[2]; 
    SeparateAdress(config->adress,result); // array[2]

    command[0] = 0xC0; //Save param when power down.
    if(config->saveParameters == false)
        command[0] = 0xC2;
    
    command[1] = result[0]; // high adress
    command[2] = result[1]; // low adress
    
    //Sped section:
    uint8_t sped = 0;
    sped = sped | (config->pairty   << 6 );
    sped = sped | (config->baudRate << 3 );
    sped = sped | (config->airDataRate << 0);
    command[3] = sped;

    //Channel Section:
    uint8_t channel = 0;
    channel = channel | (config->channelFrequency << 0);
    command[4] = channel;

    //Option SEction:
    uint8_t option = 0;
    option = config->fixedTransmissionEnable ? (option | (1 << 7)) : (option & ~(1 << 7)); 
    option = config->IODriveModeEnable       ? (option | (1 << 6)) : (option & ~(1 << 6));
    option = option | ( 1 << config->wirlessWakeUpTime);
    option = config->FECEnable ? (option | (1<<2)) : (option & ~(1 << 2));
    option = option | (config->transmisionPower << 0);

    config->loraSerial->write(command,6);

}
/**
 * @brief Set the transmission power for the LoRa module.
 *
 * This function sets the transmission power setting for the LoRa module.
 * The LoRa module must be in sleep mode for this function to succeed.
 *
 * @param config The configuration structure for the LoRa module.
 * @param transmisionPower The transmission power to set for the LoRa module.
 */
void LoRaMain::SetTransmissionPower(LoRa_Current_Config* config,TRANSMISSION_POWER transmisionPower){
    if (config->currentMode != SLEEP)
        return;

    config->transmisionPower = transmisionPower;  
}
/**
 * @brief Set the forward error correction switch on the LoRa module.
 * If FECEnable is true, FEC is enabled. If false, it is disabled.
 * The LoRa module must be in sleep mode for this function to succeed.
 */
void LoRaMain::SetFecSwitch(LoRa_Current_Config* config,bool FECEnable){
    if (config->currentMode != SLEEP)
        return;
        
    config->FECEnable = FECEnable; 
}
/**
 * @brief Set the wireless wake-up time for the LoRa module.
 *
 * This function sets the wireless wake-up time setting for the LoRa module.
 * The LoRa module must be in sleep mode for this function to succeed.
 *
 * @param config The configuration structure for the LoRa module.
 * @param wakeUpTime The wireless wake-up time to set for the LoRa module.
 */
void LoRaMain::SetWirlessWakeUpTime(LoRa_Current_Config* config, WIRLESS_WAKE_TIME wakeUpTime) {
    if (config->currentMode != SLEEP)
        return;
        
    config->wirlessWakeUpTime = wakeUpTime;
}
/**
 * @brief Set the I/O Drive Mode switch on the LoRa module.
 * If enable is true, the I/O drive mode is enabled. If false, it is disabled.
 * The LoRa module must be in sleep mode for this function to succeed.
 */
void LoRaMain::SetIODriveMode(LoRa_Current_Config* config, bool enable) {
    if (config->currentMode != SLEEP)
        return;
        
    config->IODriveModeEnable = enable;
}
/**
 * @brief Set the fixed transmission switch on the LoRa module.
 * If enable is true, fixed transmission is enabled. If false, it is disabled.
 * The LoRa module must be in sleep mode for this function to succeed.
 */
void LoRaMain::SetFixedTransmission(LoRa_Current_Config* config, bool enable) {
    if (config->currentMode != SLEEP)
        return;
        
    config->fixedTransmissionEnable = enable;
}
/**
 * @brief Set the channel frequency for the LoRa module.
 *
 * This function sets the channel frequency for the LoRa module.
 * The LoRa module must be in sleep mode for this function to succeed.
 * The valid channel range is typically 1-83.
 *
 * @param config The configuration structure for the LoRa module.
 * @param frequency The channel frequency to set for the LoRa module.
 */
void LoRaMain::SetChannelFrequency(LoRa_Current_Config* config, uint8_t frequency) {
    if (config->currentMode != SLEEP)
        return;
        
    if (frequency > 0 && frequency <= 83) { // Valid channel range is typically 1-83
        config->channelFrequency = frequency;
    }
}
/**
 * @brief Set the air data rate for the LoRa module.
 *
 * This function sets the air data rate for the LoRa module.
 * The LoRa module must be in sleep mode for this function to succeed.
 *
 * @param config The configuration structure for the LoRa module.
 * @param rate The air data rate to set for the LoRa module.
 */
void LoRaMain::SetAirDataRate(LoRa_Current_Config* config, AIR_DATA_RATE rate) {
    if (config->currentMode != SLEEP)
        return;
        
    config->airDataRate = rate;
}
/**
 * @brief Set the baud rate for the LoRa module.
 *
 * This function sets the baud rate for the LoRa module.
 * The LoRa module must be in sleep mode for this function to succeed.
 */
void LoRaMain::SetBaudRate(LoRa_Current_Config* config, BAUD_RATE rate) {
    if (config->currentMode != SLEEP)
        return;
        
    config->baudRate = rate;
}
/**
 * @brief Set the parity for the LoRa module.
 *
 * This function sets the parity for the LoRa module.
 * The LoRa module must be in sleep mode for this function to succeed.
 */
void LoRaMain::SetParity(LoRa_Current_Config* config, PARITY_STATE parity) {
    if (config->currentMode != SLEEP)
        return;
        
    config->pairty = parity;
}
/**
 * @brief Set the broadcast transmission switch on the LoRa module.
 * If enable is true, broadcast transmission is enabled. If false, it is disabled.
 * The LoRa module must be in sleep mode for this function to succeed.
 */
void LoRaMain::SetBrodcastTransmission(LoRa_Current_Config* config, bool enable) {
    if (config->currentMode != SLEEP)
        return;
        
    if (enable) {
        config->adress = 0xFFFF; // Broadcast address
    }
}
#pragma endregion