#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "SkyCOM.pio.h"
#include "SkyCOM.h"
#include "stdint.h"

uint8_t buff[20];
uint8_t BuffIndex = 0;
uint8_t BuffValIndex = 0;

void VAL_BUFF(uint64_t val, uint8_t size){
  //size of value in bytes
  //uint8_t size = Get_Val_Size(val);
  //amount of bits to shift, starting with total size minus one byte



  if(size > 8){
    uint8_t part = (size * 8) - 8;

    //loop through the value one byte at a time, starting with most significant BYTE
    for(int i = size - 1; i > -1; i--){
      if(i == 0)
      part = 0;
      else if(part == 0)
      part = 8;

      uint8_t valN = val >> part;

      printb(valN);
      printf("\n");

      buff[index] = buff[index] | valN << 0;

      part /= 2;
      index++;
    }
  }
}

int main() {

  stdio_init_all();

  // We're going to use PIO to print "Hello, world!" on the same GPIO which we
  // normally attach UART0 to.
  const uint PIN_TX = 10;
  // This is the same as the default UART baud rate on Pico
  const uint FREQ = 100000;

  PIO pio = pio0;
  uint sm = 0;
  uint offset = pio_add_program(pio, &SkyCOM_program);
  PIO_SkyCOM_init(pio, sm, offset, PIN_TX, FREQ);

  COM_Start();

  // COM_Add_Integer(1234567);
  //COM_Add_Integer(42);

  uint32_t val = 1234567;

  VAL_BUFF(val);

  printf("---------------------------\n");

  for(int i = 0; i < index; i++){
    printb(buff[i]);
  }


  while (true) {
    //PIO_Send_Byte(pio, sm, bitz);
    //        while(pio_sm_is_tx_fifo_full(pio, sm));
    //        Put_Word(pio, sm, 2684354645);


    sleep_ms(1000);
  }
}
