#include <WString.h>
#ifndef Parser_H
#define Parser_H

enum DataType{
    BatteryPercentage,
    BatteryCapacity,
    BatteryCurrent,
    BatteryVoltage        
};

typedef struct 
{
    uint8_t dataChannel;
    uint8_t dataType;
    uint16_t value;
}DataSet;


class Parser
{
    public:
        DataSet GetStatus(uint8_t*);

};



#endif