#include <stdio.h>
#include "stdint.h"
#include "stdbool.h"

#include "SKYCOM.h"
#include "buffer.h"

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


int main(){

  COM_Start(320, 9);
  COM_Add_receiver(500);
  COM_Add_receiver(865);
  COM_Add_receiver(365);
  COM_Remove_Receiver(865);
  COM_Transmit(8);

}
