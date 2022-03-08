#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "SkyCOM.pio.h"
#include "SkyCOM.h"
#include "stdint.h"



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


  COM_Setup(12);

  // COM_Add_Byte(42);
  // COM_Add_Byte(69);
  // COM_Add_Int(623);

  COM_Add_Float(5.25, 2);

  COM_Add_Receiver(16);

  COM_Send_Data(6);


  while (true) {
    //PIO_Send_Byte(pio, sm, bitz);
    //        while(pio_sm_is_tx_fifo_full(pio, sm));
    //        Put_Word(pio, sm, 2684354645);


    sleep_ms(1000);
  }
}
