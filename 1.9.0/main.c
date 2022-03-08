#include <stdio.h>
#include "stdint.h"
#include "stdbool.h"

vvsd
uint16_t buff[20] = { 0 };
uint8_t BuffIndex = 0;
int8_t BuffValIndex = 16;

void VAL_BUFF(uint32_t val, uint8_t size){
  uint16_t valN = 0;

  if((BuffValIndex - size) < 0){

    buff[BuffIndex] = buff[BuffIndex] | (val >> size - BuffValIndex);

    valN = val << 16 - (size - BuffValIndex);
    BuffValIndex = 16 - (size - BuffValIndex);

    BuffIndex++;
  } else{
    valN = val << BuffValIndex - size;
    BuffValIndex -= size;
  }

  buff[BuffIndex] = buff[BuffIndex] | valN;

}
cccxxx
s
