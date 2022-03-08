#ifndef SKYCOM_H
#define SKYCOM_H

#include <stdio.h>
#include "stdint.h"
#include "math.h"
#include "pico/stdlib.h"

void COM_Add_Byte(uint8_t val);
void COM_Add_Int(int32_t val);
void COM_Add_Float(double val, uint8_t DecPoints);

bool COM_Add_Receiver(uint16_t Receiver);
bool COM_Remove_Receiver(uint16_t Receiver);
void COM_Clear_Receivers();


bool COM_Setup(uint16_t Device_Addr);
bool COM_Send_Data(uint8_t Struct);

#endif
