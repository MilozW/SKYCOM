#ifndef BUFFER_H
#define BUFFER_H

//predefined variable values
#define DATA_BUFF 0
#define SETUP_BUFF 1

void Val_to_buff(uint32_t val, uint8_t size, bool buff);
uint8_t Get_Val_Size(uint32_t val);

void Merge_Buff();

void PrintD(bool buff);


#endif
