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

#define ID_INT    0
#define ID_CHARS  1
#define ID_FLOAT  2
#define ID_CUSTOM 3

uint16_t DEV_ADDR;
uint8_t  DEV_PROTV = 0;
uint16_t RECEIVER_BUFF[16];

//states
bool COM = false;
/**
 * @brief start device and set basic parameters
 * 
 * @param Device_Address address of this device on the network
 * @param protocol_version version of the protocol implementation on the users network
 */
void COM_Start(uint16_t Device_Address, uint8_t protocol_version){
  COM = true;
  DEV_ADDR = Device_Address;
  DEV_PROTV = protocol_version;
}

/**
 * @brief change protocol version from current
 * 
 * @param protocol_version protocol version
 */
void COM_Set_protocol_version(uint8_t protocol_version){
  DEV_PROTV = protocol_version;
}

/**
 * @brief disables communication interface
 * 
 */
void COM_Stop(){
  COM = false;
}

/**
 * @brief get current communication interface state
 * 
 * @return true 
 * @return false 
 */
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
    Val_to_buff(Struct, STRUCT_SIZ, SETUP_BUFF);
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

  if(receiver_count == 0)
  Val_to_buff(ID_RECEIVER_ALL, 3, SETUP_BUFF);

  Val_to_buff(Msg_type, 3, SETUP_BUFF);

  PrintD(SETUP_BUFF);
  printf("\n");
  PrintD(DATA_BUFF);
  printf("\n");

  Merge_Buff();

  PrintD(SETUP_BUFF);
  printf("\n");
}

/**
 * @brief transmit the transmit buffer
 * 
 * @param Struct predefined message value structure to be send along with message
 */
void COM_Transmit(uint8_t Struct){
  Setup_Message(Struct, ID_DATA_MSG);
}

/**
 * @brief add receiver to receiver buffer
 * 
 * @param receiver receiver ID
 * @return true receiver added
 * @return false buffer full
 */
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
/**
 * @brief remove a receiver from the to be send message
 * 
 * @param receiver receiver ID
 * @return true receiver ID found and removed
 * @return false receiver ID does not exist
 */
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

/**
 * @brief remove all receivers from receiver buffer
 * 
 */
void COM_Clear_Receivers(){
  for(int i = 0; i < 16; i++)
    RECEIVER_BUFF[i] = 0;
}

/**
 * @brief add an integer value to the transmit buffer
 * 
 * @param val integer to be added to transmit buffer
 */
void COM_Add_Int(int64_t val){
  bool pol = true;

  if(val < 0){
    pol = false;
    val *= -1;
  }
  
  uint8_t siz = Get_Val_Size(val);

  Val_to_buff(ID_INT, 2, DATA_BUFF);
  Val_to_buff(pol, 1, DATA_BUFF);
  Val_to_buff(siz, 4, DATA_BUFF);
  Val_to_buff(val, 16, DATA_BUFF);

}


/**
 * @brief add floating point value to transmit buffer
 * 
 * @param val double/ float to be added to transmit buffer
 */
void COM_Add_Float(double val){
  bool pol = true;

  if(val < 0){
    pol = false;
    val *= -1;
  }

  uint64_t  Ival;

  memcpy(&Ival, &val, 8);


  Val_to_buff(ID_FLOAT, 2, DATA_BUFF);
  Val_to_buff(Ival, 64, DATA_BUFF);
}
