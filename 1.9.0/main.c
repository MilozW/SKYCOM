#include <stdio.h>
#include "stdint.h"
#include "stdbool.h"

#include <math.h>

#include "SKYCOM.h"
#include "buffer.h"

void printb(uint64_t val){

	uint64_t bin = 1;

	uint8_t size = Get_Val_Size(val) * 8;

	bool buff[size * 8];

	for(int i = size - 1; i > -1; i--){
		if((val & bin) > 0)
		buff[i] = 1;
		else
		buff[i] = 0;
		bin *= 2;
	}

	for(int i = 0; i < size; i++){
		printf("%d", buff[i]);
	}
}

int8_t buffIndex = 8;
uint8_t buffEnt = 0;
uint8_t buffer[20] = { 0 };
uint64_t value = 0xAE470A525614B44A;


void putval(uint64_t val, int8_t size){
	uint64_t valN;
	while(size > 0){
		valN = val & (uint64_t)pow(2, size - 1);
		int8_t shift = size - buffIndex;
		if(shift > 0)
		valN = valN >> shift;
		else
		valN = valN << (shift*-1);
		buffer[buffEnt] = buffer[buffEnt] | valN;
		size -= 1;
		if(buffIndex == 1){
			buffIndex = 8;
			buffEnt++;
		} else
		buffIndex -= 1;
	}
}


int main(){

  COM_Start(35, 9);
  COM_Add_receiver(112);
  COM_Add_receiver(234);
  COM_Add_receiver(165);
  COM_Remove_Receiver(112);

	COM_Add_Float(53.234);

  COM_Transmit(8);


/*
	putval(3, 4);

	printb(3);
	printf("\n");
	for(int i = 0; i < 20; i++)
	printb(buffer[i]);
*/

	//printb(value, 64);
}
