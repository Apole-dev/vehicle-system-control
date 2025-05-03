#include "Parser.h"


DataSet Parser::GetStatus(uint8_t* canData){
    DataSet dataSet;
    dataSet.dataChannel = canData[0];
    dataSet.dataType    = canData[1];
    dataSet.value       = (canData[2] << 8) | (canData[3]);
    return dataSet;
}
