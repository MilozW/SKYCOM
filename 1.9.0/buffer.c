#include "SKYCOM.h"
#include "buffer.h"

//data buffer variables and arrays
uint16_t  BUFF[2][BUFF_SIZE] = { 0 };
uint8_t   BUFF_ADDR[2] = { 0 };
int8_t    BUFF_INDEX[2] = { 16, 16 };

//addresses
uint16_t DEV_ADDR = 0;
uint16_t RECEIVER_BUFF[16];

//move a value to a data buffer
void Val_to_buff(uint64_t val, uint8_t size, bool buff){
  /*
  uint16_t valN = 0;

  //check if value fits in remaining space at the current value in buffer array
  if((BUFF_INDEX[buff] - size) < 0){

    //shift right away all the bits that don't fit and add this to the buffer, next increment buffer
    BUFF[buff][BUFF_ADDR[buff]] = BUFF[buff][BUFF_ADDR[buff]] | (val >> size - BUFF_INDEX[buff]);
    BUFF_ADDR[buff]++;

    //now shift left away the already added bits and store this value New
    valN = val << 16 - (size - BUFF_INDEX[buff]);
    //calculate the new BUFF_INDEX
    BUFF_INDEX[buff] = 16 - (size - BUFF_INDEX[buff]);

  } else{
    //shift value left until next to used data
    valN = val << BUFF_INDEX[buff] - size;
    BUFF_INDEX[buff] -= size;
  }

  BUFF[buff][BUFF_ADDR[buff]] = BUFF[buff][BUFF_ADDR[buff]] | valN;

  */
 	uint64_t valN;
	while(size > 0){
		valN = val & (uint64_t)pow(2, size - 1);

    int8_t shift = size - BUFF_INDEX[buff];
		if(shift > 0)
		valN = valN >> shift;
		else
		valN = valN << (shift*-1);


		BUFF[buff][BUFF_ADDR[buff]] = BUFF[buff][BUFF_ADDR[buff]] | valN;
		size -= 1;
		if(BUFF_INDEX[buff] == 1){
			BUFF_INDEX[buff] = 8;
			BUFF_ADDR[buff]++;
		} else
		BUFF_INDEX[buff] -= 1;
	}
}

//return the size of a value in bytes
uint8_t Get_Val_Size(uint64_t val){

	uint8_t bits = 8;	//bits in byte, starting w/ one byte
	uint8_t size = 1;	//stores byte count

	while(pow(2, bits) < val)
	bits += 8, size++;

  //printf("val:%lu, siz:%d, bit:%d\n", val, size, bits);
	return size;
}


//merge two buffers
void Merge_Buff(){
  for(int i = 0; i < BUFF_ADDR[DATA_BUFF] + 1; i++)
  Val_to_buff(BUFF[DATA_BUFF][i], 16, SETUP_BUFF);
}

void PrintD(bool buff){
  for(int i = 0; i < BUFF_ADDR[buff] + 1; i++){
    printb(BUFF[buff][i]);
  }
}

bool FoI(double val){
  uint64_t  Ival = val;
  double    Dval = val - Ival;

  if(Dval == 0)
  return true;
  else
  return false;  
}