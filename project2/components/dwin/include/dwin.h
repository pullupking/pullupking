#ifndef _DWIN_H
#define _DWIN_H

#include <stddef.h>
#include <string.h>
#include <esp_log.h>
#include <stdbool.h>
#include "../../components/soft_serial/include/sw_serial.h"

#define ARRAY_LENGHT(array) (sizeof(array) / sizeof(array[0]))

typedef unsigned char uchar;

typedef enum  {
    HOME,       // 0
    PREPARING,   // 1
    CHARGING,    // 2
    END          // 3
} State_t;


typedef enum {
    ETHERNET,   // 0
    WIFI,       // 1
    GSM,        // 2
    NONE        // 3
} ConnectionType;



  // static void (*storedCallback)(void *);
  // static void* storedContext;

void dwin_init();
void dwin_listen();


int listenSerial(SwSerial *soft_serial, unsigned char *buffer);
int startsWith5aa5(unsigned char *dizi);

void sw_write_array(SwSerial *soft_serial,unsigned char const *array, size_t array_lenght );

void send_value_int(SwSerial *soft_serial,int value, unsigned char *addr);
void send_value_string(SwSerial *soft_serial,const char *data,const unsigned char *addr, int data_lenght);

void send_value_float(SwSerial *soft_serial,float value,const unsigned char *addr);
void reverse_memcpy(void* destination, const void* source, size_t size); //float sayısı içi tersinine çevirlmesi gerekiyor.

void changePage(SwSerial *soft_serial,State_t current_page);
void setQRCode(SwSerial *soft_serial,const char *qr_code);
void setCurrentTime(SwSerial *soft_serial,uint32_t currentTimeSeconds); 
void setCountDownPreapring(SwSerial *soft_serial,short second); //max 2 basamak

void setCancelButtonStatus(SwSerial *soft_serial,bool isActive);
void setCancelChargingCallback(SwSerial *soft_serial,void* context, void (*callback)(void *));

void setSessionElapsed(SwSerial *soft_serial,uint32_t sessionElapsed);
void setSessionEnergy(SwSerial *soft_serial,float energy); 
void setCurrent(SwSerial *soft_serial,float currentAmper);
void setPower(SwSerial *soft_serial,float power);
void setTotalPower(SwSerial *soft_serial,float totalPower);

void setInternetStatus(SwSerial *soft_serial,bool isInternetAvailable);
void setConnectionType(SwSerial *soft_serial,ConnectionType currentConnType);
void setOcppStatus(SwSerial *soft_serial,bool isOcppAvailable);
void setEvseCardStatus(SwSerial *soft_serial,bool isEvseAvailable);

void data_clear(SwSerial *soft_serial,unsigned char *addr,int numberOfDigits);

void reset(SwSerial *soft_serial);


/*
void get_scaled_number_value(double value, int precision, const char *unit, char *buffer, uint8_t size);
*/

#endif