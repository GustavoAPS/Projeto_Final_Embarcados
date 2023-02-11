#ifndef LM35C_H_
#define LM35C_H_
#include "esp_adc_cal.h"

void check_efuse();
void print_char_val_type(esp_adc_cal_value_t val_type);
void read_lm35dz();

void routine_lm35c();

#endif