#ifndef DATA_H
#define DATA_H

typedef struct
{
    String latitude;
    String longitude;
    String time;
    String velocity;
}Transform;

Transform parse(String data);

#endif