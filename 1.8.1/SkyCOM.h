#ifndef SKYCOM_H
#define SKYCOM_H

#include <stdio.h>
#include "stdint.h"
#include "math.h"
#include "pico/stdlib.h"

void COM_Add_Integer(int32_t val);

bool COM_Add_Receiver(uint16_t Receiver);
bool COM_Remove_Receiver(uint16_t Receiver);
void COM_Clear_Receivers();

void COM_Start();

bool COM_Setup(uint16_t Device_Addr);
bool COM_Send_Data(uint8_t Struct);


void printb(uint32_t val);
uint8_t Get_Val_Size(uint32_t val);

#endif
