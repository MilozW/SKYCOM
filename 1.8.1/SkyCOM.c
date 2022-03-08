#include "SkyCOM.h"



#define DATA_BUFF_SIZE 64

//stores the values to be send
uint16_t 	DATA_BUFF[DATA_BUFF_SIZE] = { 0 };
//address of the current bit in the value
uint8_t 	DATA_BIT_ADDR = 0;

//stores the datatypes
uint16_t 	DATA_TYPE[DATA_BUFF_SIZE] = { 0 };
//address of the current bit in the value
uint8_t 	DATA_TYPE_ADDR = 0;

//stores the array location
uint8_t 	DATA_BUFF_ADDR = 0;

void Add_bitz(uint32_t val, uint8_t size, uint8_t bffr);
void printb(uint32_t val);
uint8_t Get_Val_Size(uint32_t val);

struct BUFFERS{
	uint8_t *VAL_ADDR;
	uint8_t *BUFF_ADDR;
	uint16_t *BUFF;
};

struct BUFFERS bit_buffer[2];

void COM_Add_Integer(int32_t val){

	printf("val: %lu", val);
	printf("\n");



	bool pol = true;

	if(val < 0)
	pol = false;


	uint8_t size = Get_Val_Size(val);

	Add_bitz(pol, 1, 1);
	Add_bitz(size, 3, 1);

	Add_bitz(val, size * 8, 0);

	printf("data buff address: %d\n", *bit_buffer[0].BUFF_ADDR);

	 printb(bit_buffer[0].BUFF[*bit_buffer[0].BUFF_ADDR-1]);
	printf("\n");
	 printb(bit_buffer[0].BUFF[*bit_buffer[0].BUFF_ADDR]);
	printf("\n");
	 printb(bit_buffer[0].BUFF[*bit_buffer[0].BUFF_ADDR+1]);
	printf("\n");
	// printf("\n");
	// printb(DATA_TYPE[DATA_BUFF_ADDR-1]);
	// printb(DATA_TYPE[DATA_BUFF_ADDR]);
	// printb(DATA_TYPE[DATA_BUFF_ADDR-1]);
	// printf("\n");
	// printf("\n");
}


void COM_Start(){
	bit_buffer[0].VAL_ADDR = &DATA_BIT_ADDR, bit_buffer[0].BUFF_ADDR = &DATA_BUFF_ADDR, bit_buffer[0].BUFF = DATA_BUFF;
	bit_buffer[1].VAL_ADDR = &DATA_TYPE_ADDR, bit_buffer[1].BUFF_ADDR = &DATA_BUFF_ADDR, bit_buffer[1].BUFF = DATA_TYPE;
}


uint32_t Get_Bitz(){

}

void Add_bitz(uint32_t val, uint8_t size, uint8_t bffr){

	// printf("size:%d\n", size);
	 printf("-----------------");
	//
	 printb(val);
	printf("\n");


	while(size > 16){
		uint16_t valN = val >> 16;

		 printb(valN);
		 printf("\n");

		Add_bitz(valN, 16, bffr);
		size -= 16;
	}

	 printb(val);
	 printf("\n");


	//calculate most significant available bit counted from LSB
	int8_t offset = 16 - size - *bit_buffer[bffr].VAL_ADDR;
/*
	printf("offset %d - ", offset);
	printf("address %d - ", *bit_buffer[bffr].VAL_ADDR);
	printf("buffer %d - ", *bit_buffer[bffr].BUFF);
	printf("\n");
*/

	if(offset < 0){
		//invert offset since it is negative
		offset *= -1;

		//shift excess bits away and store in val Old
		uint16_t valO = valO | val >> offset;
		//add value Old to bitz array
		bit_buffer[bffr].BUFF[*bit_buffer[bffr].BUFF_ADDR] = bit_buffer[bffr].BUFF[*bit_buffer[bffr].BUFF_ADDR] | valO << 0;

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
		*bit_buffer[bffr].BUFF_ADDR++;
		bit_buffer[bffr].BUFF[*bit_buffer[bffr].BUFF_ADDR] = bit_buffer[bffr].BUFF[*bit_buffer[bffr].BUFF_ADDR] | valN << offset;
		*bit_buffer[bffr].VAL_ADDR = size;       //ADDR = 0; ADDR += size;
	} else{
		//shift value to most significant bit available
		bit_buffer[bffr].BUFF[*bit_buffer[bffr].BUFF_ADDR] = bit_buffer[bffr].BUFF[*bit_buffer[bffr].BUFF_ADDR] | val << offset;
		*bit_buffer[bffr].VAL_ADDR += size;
	}
}

//return the size of a value in bytes
uint8_t Get_Val_Size(uint32_t val){

	uint64_t bits = 8;	//bits in byte, starting w/ one byte
	uint8_t size = 1;	//stores byte count

	while(pow(2, bits) < val)
	bits *= 2, size++;

	return size;
}

void printb(uint32_t val){

	uint32_t bin = 1;

	 uint8_t size = Get_Val_Size(val);
	//uint8_t size = 2;

	bool buff[size * 8];

	for(int i = (size * 8) - 1; i > -1; i--){
		if((val & bin) > 0)
		buff[i] = 1;
		else
		buff[i] = 0;
		bin *= 2;
	}

	for(int i = 0; i < size * 8; i++){
		printf("%d", buff[i]);
	}
}
