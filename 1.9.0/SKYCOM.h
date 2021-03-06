
#ifndef SKYCOM_H
#define SKYCOM_H

#include <stdio.h>

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

//------------*CONFIG*-------------------------------------------------------
//predefined size values
#define BUFF_SIZE 64
#define ADDR_SIZE 8
#define STRUCT_SIZ 8
//---------------------------------------------------------------------------


void COM_Start(uint16_t Device_Address, uint8_t protocol_version);
void COM_Stop();
bool COM_Get_state();

bool COM_Add_receiver(uint16_t receiver);
bool COM_Remove_Receiver(uint16_t receiver);
void COM_Clear_Receivers();

void COM_Set_protocol_version(uint8_t protocol_version);

void COM_Transmit(uint8_t Struct);


void COM_Add_Int(int64_t val);
void COM_Add_Float(double val);


void printb(uint64_t val);

#endif
