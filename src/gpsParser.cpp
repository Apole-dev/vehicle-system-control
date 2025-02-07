#include <SoftwareSerial.h>
#include <Arduino.h>
#include "gpsParser.h"

Transform transform;

Transform parse(String data)
{

    if (data.startsWith("$GPGGA"))
    {
        String parts[15];
        int partIndex = 0;

        while (data.length() > 0)
        {
            int index = data.indexOf(',');
            if (index == -1)
            {
                parts[partIndex++] = data;
                break;
            }
            parts[partIndex++] = data.substring(0, index);
            data = data.substring(index + 1);
        }

        String time = parts[1];
        String latitude = parts[2];
        String longitude = parts[4];

        int latitudeDegrees = latitude.substring(0, 2).toInt(); 
        float latitudeMinutes = latitude.substring(2).toFloat();  
        float finalLatitude = latitudeDegrees + (latitudeMinutes / 60.0); 

        transform.latitude = "Latitude: " + String(finalLatitude,6);

        int longitudeDegrees = longitude.substring(0, 3).toInt(); 
        float longitudeMinutes = longitude.substring(3).toFloat();  
        float finalLongitude = longitudeDegrees + (longitudeMinutes / 60.0);  

        transform.longitude = "Longitude: " + String(finalLongitude,6);
        
        String hour = time.substring(0, 2);  
        String minute = time.substring(2, 4);
        String second = time.substring(4, 6);  
      
        
        int h = hour.toInt() + 3;  
        if (h >= 24) h -= 24;  

        transform.time ="Time: " + String(h) + ':' + minute + ':' + second;
    }
    
    if (data.startsWith("$GPVTG"))
    {
        String parts[15];
        int partIndex = 0;

        while (data.length() > 0)
        {
            int index = data.indexOf(',');
            if (index == -1)
            {
                parts[partIndex++] = data;
                break;
            }
            parts[partIndex++] = data.substring(0, index);
            data = data.substring(index + 1);
        }

        transform.velocity = "Velocity: " + parts[7];  
    }
    return transform;   
}