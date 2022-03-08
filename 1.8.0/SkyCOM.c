#include "SkyCOM.h"

//size of a device ID in bits
#define DEVICE_ID_SIZE 10


//ammount of data in bytes in a single message, tweak to optimize RAM usage
#define DATA_BUFF_SIZE 20


//maximum amounnt of [datatype] values in message
#define BYTE_BUFF_SIZE 16
#define FLOAT_BUFF_SIZE 16
#define INT_BUFF_SIZE 16

//maximum packages per message
#define PKG_BUFF_SIZE 48

//Data Pakcage ID's
#define INT_PKG_ID 1
#define FLOAT_PKG_ID 2
#define BYTE_PKG_ID 3
#define STRING_PKG_ID 4
#define STRUCT_PKG_ID 5

#define CHECKSUM_ID 15

//polarity of value is stored in package ID and is dealt with during message building
//these non-package package ID can be stored anywhere past 15, since the
//package ID is a  4 bit value
#define INTP_PKG_ID 20
#define INTN_PKG_ID 21

#define FLTP_PKG_ID 22
#define FLTN_PKG_ID 23



//Message Type ID's
#define DEVICE_MSG_ID 0
#define DATA_MSG_ID 1
#define REGISTER_MSG_ID 2
#define TIME_MSG_ID 3

uint16_t Data_Buff[DATA_BUFF_SIZE];

//package type and address buffer
uint8_t Package_Type_Buff[PKG_BUFF_SIZE];
//uint8_t Package_Info_Buff[PKG_BUFF_SIZE];
uint8_t Package_Count = 0;
uint8_t Data_Addr = 0;


//float buffer variables
double Float_Buff[FLOAT_BUFF_SIZE];
uint8_t Floating_Res_Buff[FLOAT_BUFF_SIZE];
uint8_t Float_Addr = 0;
//integer buffer variables
uint32_t Int_Buff[INT_BUFF_SIZE];
uint8_t Int_Addr = 0;
//byte buffer variables
uint8_t Byte_Buff[BYTE_BUFF_SIZE];
uint8_t Byte_Addr = 0;


//receiver buffer for transmitting
uint16_t Receiver_Buff[16] = {0};
//device address
uint16_t Transmitter_Addr = 0;



//stores the message in 16 bit values to be send to PIO
uint16_t bitz[64] = { 0 };
//address of the bits in a bitz array entity
uint8_t bitz_addr = 0;
//location in the buffer
uint8_t bitz_buff_addr = 0;

void printb(uint16_t val){

	uint16_t bin = 1;

	bool buff[16];

	for(int i = 16 - 1; i > -1; i--){
		if((val & bin) > 0)
		buff[i] = 1;
		else
		buff[i] = 0;
		bin *= 2;
	}

	for(int i = 0; i < 16; i++){
		printf("%d", buff[i]);
	}
}

//return the size of a value in bytes
uint8_t Get_Val_Size(uint32_t val){

	uint8_t size = 1;	//stores byte count
	uint32_t bits = 8;	//bits in byte, starting w/ one byte

	while(pow(2, bits) < val)
	bits *= 2, size++;

	return size;
}

void Add_bitz(uint16_t val, uint8_t size, uint8_t *ADDR, uint16_t *BUFF){

	//calculate most significant available bit counted from LSB
	int offset = 16 - size - *ADDR;

	if(offset < 0){
		//invert offset since it is negative
		offset *= -1;

		//shift excess bits away and store in val Old
		uint16_t valO = valO | val >> offset;
		//add value Old to bitz array
		BUFF[*ADDR] = BUFF[*ADDR] | valO << 0;

		//calculate value of excess bits by bitwising excess amount of times and storing in val new
		uint16_t valN = 0, bin = 1;
		for(int i = 0; i < offset; i++){
			valN += val & bin;
			bin *= 2;
		}

		//calculate new offset
		size = offset;
		offset = 16 - offset;

		//add excess value to next entity in bitz array
		*ADDR++;
		BUFF[*ADDR] = BUFF[*ADDR] | valN << offset;
		*ADDR = size;       //ADDR = 0; ADDR += size;
	} else{
		//shift value to most significant bit available
		BUFF[*ADDR] = BUFF[*ADDR] | val << offset;
		*ADDR += size;
	}
}

void COM_Add_Byte(uint8_t val){
	/*
	Byte_Buff[Byte_Addr] = val;
	Package_Type_Buff[Package_Count] = BYTE_PKG_ID;
	Byte_Addr++, Package_Count++;
	*/

	Data_Buff[Package_Count];
}

void COM_Add_Int(int32_t val){
  if(val < 0){
    val *= -1;
    Package_Type_Buff[Package_Count] = INTN_PKG_ID;
  } else
  Package_Type_Buff[Package_Count] = INTP_PKG_ID;

	Int_Buff[Int_Addr] = val;

	Int_Addr++, Package_Count++;
}

void COM_Add_Float(double val, uint8_t DecPoints){

  if(val < 0){
    val *= -1;
    Package_Type_Buff[Package_Count] = FLTN_PKG_ID;
  } else
  Package_Type_Buff[Package_Count] = FLTP_PKG_ID;

	Float_Buff[Float_Addr] = val;
	Floating_Res_Buff[Float_Addr] = DecPoints;

	Float_Addr++, Package_Count++;
}

void COM_Add_String(char string[]){

}

//adds receiver to buffer, false if receiver buffer is full
bool COM_Add_Receiver(uint16_t Receiver){
	bool state = false;

	for(int i = 0; i < 16; i++){
		if(Receiver_Buff[i] == 0){
			Receiver_Buff[i] = Receiver;
			state = true;
			break;
		}
	}

	return state;
}

//removes receiver from buffer, returns false if receiver not present
bool COM_Remove_Receiver(uint16_t Receiver){
	bool state = false;

	for(int i = 0; i < 16; i++){
		if(Receiver_Buff[i] == Receiver){
			Receiver_Buff[i] = 0;
			state = true;
			break;
		}
	}

	return state;
}

//clear receiver buffer
void COM_Clear_Receivers(){
	for(int i = 0; i < 16; i++)
	Receiver_Buff[i] = 0;
}

bool COM_Setup(uint16_t Device_Addr){

	bool state = false;

	if(Device_Addr != 0){
		Transmitter_Addr = Device_Addr;
		state = true;
	}

	return state;
}

bool COM_Send_Data(uint8_t Struct){

	bool state = false;

	//start bit
	Add_bitz(1,1, &bitz_addr, bitz);
	//data message
	Add_bitz(DATA_MSG_ID,2, &bitz_addr, bitz);

	uint16_t Receiver_Addrs[16] = {0};
	uint8_t Receiver_Count = 0;

	for(int i = 0; i < 16; i++){
		if(Receiver_Buff[i] != 0){
			Receiver_Addrs[Receiver_Count] = Receiver_Buff[i];
			Receiver_Count++;
		}
	}

	//add receiver count
	Add_bitz(Receiver_Count, 4, &bitz_addr, bitz);

	//add transmitter address
	Add_bitz(Transmitter_Addr, DEVICE_ID_SIZE, &bitz_addr, bitz);

	//add receiver addresses
	for(int i = 0; i < Receiver_Count; i++)
	Add_bitz(Receiver_Addrs[i], DEVICE_ID_SIZE, &bitz_addr, bitz);

	if(Struct > 0){
		Add_bitz(STRUCT_PKG_ID, 4, &bitz_addr, bitz);

		Add_bitz(Struct, 8, &bitz_addr, bitz);
	}

	//counter values for all datatypes
	uint8_t Byte_Cnt = 0, Int_Cnt = 0, Float_Cnt = 0;

	//build all packages and add them to message
	for(int i = 0; i < Package_Count; i++){
		//build BYTE
		if(Package_Type_Buff[i] == BYTE_PKG_ID){
			Add_bitz(BYTE_PKG_ID, 4, &bitz_addr, bitz);

			Add_bitz(Byte_Buff[Byte_Cnt], 8, &bitz_addr, bitz);
			Byte_Cnt++;
		}

		//build INTEGER
		else if(Package_Type_Buff[i] == INTP_PKG_ID || Package_Type_Buff[i] == INTN_PKG_ID){
			Add_bitz(INT_PKG_ID, 4, &bitz_addr, bitz);

			//add polarity bit based on package flag
			if(Package_Type_Buff[i] == INTP_PKG_ID)
			Add_bitz(1,1, &bitz_addr, bitz);
			else
			Add_bitz(0,1, &bitz_addr, bitz);

			//add size and reduce by one to make full use of 2 size bits
			uint8_t size = Get_Val_Size(Int_Buff[Int_Cnt]);
			Add_bitz(size-1, 2, &bitz_addr, bitz);

			Add_bitz(Int_Buff[Int_Cnt], size*8, &bitz_addr, bitz);
			Int_Cnt++;
		}

		//build FLOAT
		else if(Package_Type_Buff[i] == FLTP_PKG_ID || Package_Type_Buff[i] == FLTN_PKG_ID){
			Add_bitz(FLOAT_PKG_ID, 4, &bitz_addr, bitz);

			//add polarity bit based on package flag
			if(Package_Type_Buff[i] == FLTP_PKG_ID)
			Add_bitz(1,1, &bitz_addr, bitz);
			else
			Add_bitz(0,1, &bitz_addr, bitz);

			//store integral part of float
			uint32_t Int = (uint32_t)Float_Buff[Float_Cnt];
			//remove integral from float
			Float_Buff[Float_Cnt] -= Int;
			//make integral from fraction
			Float_Buff[Float_Cnt] *= pow(10, Floating_Res_Buff[Float_Cnt]);
			uint32_t Frac = (uint32_t)Float_Buff[Float_Cnt];

			printf("Integral: %d\n", Int);
			printf("Fraction: %d\n", Frac);

			//add size and reduce by one to make full use of 2 size bits
			uint8_t size = Get_Val_Size(Int);
			Add_bitz(size-1, 2, &bitz_addr, bitz);
			//add integral
			Add_bitz(Int, size*8, &bitz_addr, bitz);

			size = Get_Val_Size(Frac);
			Add_bitz(size-1, 2, &bitz_addr, bitz);
			//add flaction
			Add_bitz(Frac, size*8, &bitz_addr, bitz);

			Float_Cnt++;
		}

		//build STRING
		else if(Package_Type_Buff[i] == STRING_PKG_ID){

		}

	}

	Add_bitz(CHECKSUM_ID, 4, &bitz_addr, bitz);

	uint16_t checksum = 0;

	for(int i = 0; i < bitz_buff_addr + 1; i++){
		uint8_t val1 = bitz[i] >> 8;
		uint8_t val2 = bitz[i] & 0x00FF;
		checksum += val1 + val2;
	}

	Add_bitz(checksum, 16, &bitz_addr, bitz);

	printf("\n");

	for(int i = 0; i < bitz_buff_addr + 1; i++){
		//printf("bitz: %d\n", bitz[i]);
		printb(bitz[i]);
	}

	return state;
}
