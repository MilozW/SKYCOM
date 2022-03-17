#include "SKYCOM.h"
#include "buffer.h"

//Protocol ID's
#define ID_STRUCT              0
#define ID_VERSION             1
#define ID_TRANSMITTER         2
#define ID_RECEIVER_SPECIFIC   3
#define ID_RECEIVER_ALL        4
#define ID_SYSTEM_MSG          5
#define ID_DATA_MSG            6


uint16_t DEV_ADDR;
uint8_t  DEV_PROTV = 0;
uint16_t RECEIVER_BUFF[16];

//states
bool COM = false;

//initializes communication
void COM_Start(uint16_t Device_Address, uint8_t protocol_version){
  COM = true;
  DEV_ADDR = Device_Address;
  DEV_PROTV = protocol_version;
}

//stops communication
void COM_Stop(){
  COM = false;
}

//
bool COM_Get_state(){
  return COM;
}


void Setup_Message(uint8_t Struct, uint8_t Msg_type){
  // ---setup instruction set---
  // -0   Struct
  // -1
  // -2   Transmitter
  // -3   Specific receiver
  // -4   All receivers
  // -5   System message
  // -6   Data message
  // -7

  if(COM == false)
    return;

  Val_to_buff(DEV_PROTV, 5, SETUP_BUFF);

  if(Struct != 0){
    Val_to_buff(ID_STRUCT, 3, SETUP_BUFF);
    Val_to_buff(Struct, 8, SETUP_BUFF);
  }

  //add transmitter instruction
  Val_to_buff(ID_TRANSMITTER, 3, SETUP_BUFF);
  //add transmitter address
  Val_to_buff(DEV_ADDR, ADDR_SIZE, SETUP_BUFF);

  uint8_t receiver_count = 0;

  for(int i = 0; i < 16; i++){
    if(RECEIVER_BUFF[i] != 0){
      Val_to_buff(ID_RECEIVER_SPECIFIC, 3, SETUP_BUFF);
      Val_to_buff(RECEIVER_BUFF[i], ADDR_SIZE, SETUP_BUFF);
      receiver_count++;
    }
  }

  printf("receivercount %d\n", receiver_count);

  if(receiver_count == 0)
      Val_to_buff(ID_RECEIVER_ALL, 3, SETUP_BUFF);

  Val_to_buff(Msg_type, 3, SETUP_BUFF);



  PrintD(SETUP_BUFF);
  printf("\n");
}

void COM_Transmit(uint8_t Struct){
  Setup_Message(Struct, ID_DATA_MSG);
}

void COM_Set_protocol_version(uint8_t protocol_version){
  DEV_PROTV = protocol_version;
}

//adds receiver to receiver array, returns true if succesfully putin buffer or already present
bool COM_Add_receiver(uint16_t receiver){
	bool state = false;

  //check if receiver is already in buffer
	for(int i = 0; i < 16; i++){
		if(RECEIVER_BUFF[i] == receiver)
      state = true; break;
	}

  //if not, check for empty values and if found set the value
  if(state == false){
	   for(int i = 0; i < 16; i++){
       if(RECEIVER_BUFF[i] == 0){
         RECEIVER_BUFF[i] = receiver;
         state = true; break;
       }
     }
  }
	return state;
}

//search for the receiver to be removed and set this spot to zero
bool COM_Remove_Receiver(uint16_t receiver){
	bool state = false;

	for(int i = 0; i < 16; i++){
		if(RECEIVER_BUFF[i] == receiver){
			RECEIVER_BUFF[i] = 0;
			state = true; break;
		}
	}
	return state;
}

//clear receiver buffer
void COM_Clear_Receivers(){
  for(int i = 0; i < 16; i++)
    RECEIVER_BUFF[i] = 0;
}
