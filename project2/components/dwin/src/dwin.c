
#include "dwin.h"
#include "ui_messages.h"
#include <stdio.h>
#include <string.h>

const char *TAG= "Dwin Ekran : ";


void save_char_as_hex(const unsigned char *input, size_t length, char *output) {
    size_t index = 0;
    for (size_t i = 0; i < length; i++) {
        // Her karakteri iki basamaklı hex olarak yaz
        index += snprintf(output + index, 3, "%02X", input[i]);
    }
}

void dwin_init(){   
  
SwSerial *soft_serial = sw_new(4,35,false,512);
sw_open(soft_serial,9600);

 
  while (true)
  {     
    
    unsigned char response_value [50];
    int data_len = listenSerial(soft_serial,response_value);
    
    char value_hex_to_string[data_len];
    save_char_as_hex(response_value,data_len,value_hex_to_string);
    ESP_LOGI("String Deger : ","%s",value_hex_to_string);

    if (startsWith5aa5(response_value))    
    {
      if (!memcmp(response_value,CANCEL_CHARGING,ARRAY_LENGHT(CANCEL_CHARGING)))
      {        
        sw_write_array(soft_serial,PAGE_ENDING,sizeof(PAGE_ENDING)); 
      }
        
    } 

    vTaskDelay(pdMS_TO_TICKS(500));   
   
    #if 1
    reset(soft_serial);
    vTaskDelay(pdMS_TO_TICKS(2000));
    setTotalPower(soft_serial,22.23);
    setQRCode(soft_serial,"Yusuf-Aspower23-123*5069&&%&//as3444");   
    vTaskDelay(pdMS_TO_TICKS(1000)); 
    
    //icon check
    setInternetStatus(soft_serial,true);
    vTaskDelay(pdMS_TO_TICKS(500));
    setInternetStatus(soft_serial,false);
    vTaskDelay(pdMS_TO_TICKS(500));
    setConnectionType(soft_serial,ETHERNET);
    vTaskDelay(pdMS_TO_TICKS(500));
    setConnectionType(soft_serial,WIFI);
    vTaskDelay(pdMS_TO_TICKS(500));
    setConnectionType(soft_serial,GSM);
    vTaskDelay(pdMS_TO_TICKS(500));
    
    setOcppStatus(soft_serial,true);
    vTaskDelay(pdMS_TO_TICKS(500));
    setOcppStatus(soft_serial,false);
    vTaskDelay(pdMS_TO_TICKS(500));
    
    setEvseCardStatus(soft_serial,true);
    vTaskDelay(pdMS_TO_TICKS(500));
    setEvseCardStatus(soft_serial,false);
    vTaskDelay(pdMS_TO_TICKS(500));
    
    //================================
    //RTC ZAMAN
    for (size_t i = 3600; i < 4000; i=i+60)
    {
      setCurrentTime(soft_serial,i);
      vTaskDelay(pdMS_TO_TICKS(1000));  
    }   
    
    //PREPARING SAYFASI VE GERİ SAYIM
    changePage(soft_serial,PREPARING);
    setCountDownPreapring(soft_serial,9);
    vTaskDelay(pdMS_TO_TICKS(1000));
    
      //CHARGING SAYFASI
    changePage(soft_serial,CHARGING);
    vTaskDelay(pdMS_TO_TICKS(1000));

    //şarj iptal buton
    setCancelButtonStatus(soft_serial,false);
    vTaskDelay(pdMS_TO_TICKS(1000));
    setCancelButtonStatus(soft_serial,true);
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    int time=40000;
    for (float i = 0; i < 500; i=i+25.23)
    {
      float sayi=20;
      
      //send_value(i,addr_amp);  
      sayi = sayi +i+0.32;
      setCurrent(soft_serial,sayi);
      sayi += 12.23;
      setPower(soft_serial,sayi);
      sayi += 34.34;
      setSessionEnergy(soft_serial,sayi);
      time+=i;
      setSessionElapsed(soft_serial,time);
     
      vTaskDelay(pdMS_TO_TICKS(1000));
    }

    changePage(soft_serial,END);
    vTaskDelay(pdMS_TO_TICKS(1000));

    changePage(soft_serial,HOME);
    vTaskDelay(pdMS_TO_TICKS(1000));


    // data_clear(ADDR_AMP,5);
    #endif
  } 
}

void changePage(SwSerial *soft_serial,State_t current_page){
  switch (current_page)
  {
  case HOME:
    sw_write_array(soft_serial,PAGE_HOME,ARRAY_LENGHT(PAGE_HOME));
    break;
  case PREPARING:
  sw_write_array(soft_serial,PAGE_PREPARING,ARRAY_LENGHT(PAGE_PREPARING));
  break;
  case CHARGING:
  sw_write_array(soft_serial,PAGE_CHARGING,ARRAY_LENGHT(PAGE_CHARGING));
  break;
  case END:
  sw_write_array(soft_serial,PAGE_ENDING,ARRAY_LENGHT(PAGE_ENDING));
  break;  
  default:
    break;
  }
}

void setCurrent(SwSerial *soft_serial,float currentValue){
    send_value_float(soft_serial,currentValue,ADDR_AMP);
  }

void setPower(SwSerial *soft_serial,float powerValue){
  send_value_float(soft_serial,powerValue,ADDR_POWER);
}

void setSessionEnergy(SwSerial *soft_serial,float energyValue){
  send_value_float(soft_serial,energyValue,ADDR_SESSION_ENERGY);
}

void setTotalPower(SwSerial *soft_serial,float totalPowerValue){
  char valueString[10];
  int value_lenght;
  value_lenght= sprintf(valueString,"%.2f",totalPowerValue);
  memcpy(&valueString[value_lenght]," KW",3);
  
  send_value_string(soft_serial,valueString,ADDR_TOTAL_ENERGY,(value_lenght+3));
}

void setSessionElapsed(SwSerial *soft_serial,uint32_t sessionElapsedSecond){
    int hours = sessionElapsedSecond / 3600;
    int minutes = (sessionElapsedSecond % 3600) / 60;
    int seconds = sessionElapsedSecond % 60;
    char elapsed[20];
    int elapsed_lenght;
    elapsed_lenght = snprintf(elapsed,sizeof(elapsed),"%02d:%02d:%02d",hours,minutes,seconds);
    send_value_string(soft_serial,elapsed,ADDR_SESSION_ELAPSED,elapsed_lenght);
   }

void setQRCode(SwSerial *soft_serial,const char *qrCodeValue){  
  send_value_string(soft_serial,qrCodeValue,ADDR_QR,strlen(qrCodeValue));
}

void setInternetStatus(SwSerial *soft_serial,bool isInternetAvailable){
  if (isInternetAvailable == true)
  {
    sw_write_array(soft_serial,INTERNET_AVAILABLE,ARRAY_LENGHT(INTERNET_AVAILABLE));
  }else
  {
    sw_write_array(soft_serial,INTERNET_UNAVAILABLE,ARRAY_LENGHT(INTERNET_UNAVAILABLE));
  }
}

void setConnectionType(SwSerial *soft_serial,ConnectionType currentConnType){
  if (currentConnType == ETHERNET)
  {
    sw_write_array(soft_serial,CONNECTION_TYPE_ETHERNET,ARRAY_LENGHT(CONNECTION_TYPE_ETHERNET));
  }
  else if (currentConnType == WIFI)
  {
    sw_write_array(soft_serial,CONNECTION_TYPE_WIFI,ARRAY_LENGHT(CONNECTION_TYPE_WIFI));
  }else if (currentConnType == GSM)
  {
    sw_write_array(soft_serial,CONNECTION_TYPE_GSM,ARRAY_LENGHT(CONNECTION_TYPE_GSM));
  }else
  {
    ESP_LOGI("BAĞLANTI TYPE ERROR : ","HİÇ BİR BAĞLANTI TÜRÜ GÖNDERİLMEDİ.");
  }   
}

void setOcppStatus(SwSerial *soft_serial,bool isOcppAvailable){
  if (isOcppAvailable == true)
  {
    sw_write_array(soft_serial,OCPP_AVAILABLE,ARRAY_LENGHT(OCPP_AVAILABLE));
  }
  else
  {
    sw_write_array(soft_serial,OCPP_UNAVAILABLE,ARRAY_LENGHT(OCPP_UNAVAILABLE));
  }
  
}

void setEvseCardStatus(SwSerial *soft_serial,bool isEvseAvailable){
  if (isEvseAvailable == true)
  {
    sw_write_array(soft_serial,EVSE_CARD_AVAILABLE,ARRAY_LENGHT(EVSE_CARD_AVAILABLE));
  }
  else
  {
    sw_write_array(soft_serial,EVSE_CARD_UNAVAILABLE,ARRAY_LENGHT(EVSE_CARD_UNAVAILABLE));
  }  
}

void setCurrentTime(SwSerial *soft_serial,uint32_t currentTimeSeconds){
    int hours = currentTimeSeconds / 3600;
    int minutes = (currentTimeSeconds % 3600) / 60;

    RTC[11] = (unsigned char) hours;
    RTC[12] = (unsigned char) minutes;

    sw_write_array(soft_serial,RTC,ARRAY_LENGHT(RTC));
} 

void setCountDownPreapring(SwSerial *soft_serial,short countDownSeconds){
  for (int i = countDownSeconds;  0<= i ; i--)
  {
    send_value_int(soft_serial,i,ADDR_COUNTDOWN);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  
}

void setCancelButtonStatus(SwSerial *soft_serial,bool isActive){
  if (isActive)
  {
    sw_write_array(soft_serial,CANCEL_BUTTON_AVAILABLE,ARRAY_LENGHT(CANCEL_BUTTON_AVAILABLE));
  }
  else
  
  {
    sw_write_array(soft_serial,CANCEL_BUTTON_UNAVAILABLE,ARRAY_LENGHT(CANCEL_BUTTON_UNAVAILABLE));
    
  }
  
}

void setCancelChargingCallback(SwSerial *soft_serial,void* context, void (*callback)(void *)){
  
  // storedContext = context;
  //storedCallback = callback;

}



int listenSerial(SwSerial *soft_serial,unsigned char *buffer){
  
 /*  
 const char * TAG = "SERIAL GELEN VERİ";
  const short ARRAY_LENGHT = 30;
  unsigned char cmd_data[ARRAY_LENGHT];
  memset(cmd_data,0,ARRAY_LENGHT);
 */

  int data_len = sw_any(soft_serial);
    // ESP_LOGI(TAG,"%d",data_len);
        
  for (size_t i = 0; i < data_len; i++)
  {
      unsigned char data = sw_read(soft_serial);
      buffer[i] = data;
    // ESP_LOGI(TAG,"%02X",data);
  }

  return data_len;
}

int startsWith5aa5(unsigned char *dizi)
{
    // The string to compare
    
      const unsigned char prefix[] = {0x5a,0xa5};
    // Length of the prefix
    

    // Use strncmp to compare the first prefix_len characters
    if (!memcmp(dizi, prefix, 2))
    {
     
        return 1; // The string starts with "5aa5"
    }
    else
    {
        return 0; // The string does not start with "5aa5"
    }
}

void send_value_int(SwSerial *soft_serial,int value,unsigned char *addr){
 // ESP_LOGI(TAG,"***********************\n");
  unsigned char ARRAY_LENGHT = 10;
  unsigned char cmd_header[] = {0x5a,0xa5,0x00,0x82};
  unsigned char * cmd_send = (unsigned char *) malloc(ARRAY_LENGHT * sizeof(unsigned char));
  //add header to CMD
  memcpy(cmd_send,cmd_header,sizeof(cmd_header));
  //add component address to CMD
  memcpy(&cmd_send[4],addr,2);
  
  //gelen int değer string dönüştürülüyor
  char value_to_char[4];
  reverse_memcpy(value_to_char,&value,4);
    
  unsigned char value_to_char_size = 4;
   // DBUGF("value : %s , uzunluk : %d",value_to_char,value_to_char_size);
    cmd_send[2] = (unsigned char)((int)value_to_char_size + 3);  
    
   // memcpy(&cmd_send[3],write_addr,sizeof(3));

    for (size_t i = 0; i < value_to_char_size; i++)
    {
      cmd_send[6+i]=(unsigned char) value_to_char[i];
    }

    sw_write_array(soft_serial,cmd_send,ARRAY_LENGHT);
  
    /*  ESP_LOGI(TAG,"VALUE BYTE: ");
   
   for (int i = 0; i < ARRAY_LENGHT; i++) {
        ESP_LOGI(TAG, "array_Send[%d] = 0x%02X", i, cmd_send[i]);
        
    } */
   
   free(cmd_send);
 }
 
 void send_value_string(SwSerial *soft_serial,const char *data,const unsigned char *addr, int data_lenght){

  // ESP_LOGI(TAG,"***********************");
  // ESP_LOGI("gelen data uzuznluğu","%d",data_lenght);
  size_t ARRAY_LENGHT = 8+ data_lenght;
  unsigned char cmd_header[] = {0x5a,0xa5,0x00,0x82};
  unsigned char * cmd_send = (unsigned char *) malloc(ARRAY_LENGHT * sizeof(unsigned char));

  //add header to CMD
  memcpy(cmd_send,cmd_header,sizeof(cmd_header));
  //add component address to CMD
  memcpy(&cmd_send[4],addr,2);

  //ESP_LOGI(TAG,"data : %s",data);
  //DBUGF("value : %s , uzunluk : %d",value,value_to_char_size);
  // "+5" sebebi dizinin 3. elemanından sonra 3 eleman(type+addr) ve sonuna 2 eleman 0XFF ekleniyor.
  cmd_send[2] = (unsigned char)(data_lenght + 5); 
  
  for (size_t i = 0; i < (size_t)cmd_send[2]-3; i++)
  {
    if (i >= (size_t)cmd_send[2] - 5) {
        // Son iki değere 0xFF ekle
        cmd_send[6 + i] = 0xFF;
    } else {
        // Diğer değerlere value dizisini ekle
        cmd_send[6 + i] = (unsigned char)data[i];
    }      
  }

 /*  size_t array_lenght = (size_t)array_send[2]+3; //ilk 3 elemanı unutma.
  memcpy(&array_send[array_lenght],end_value,2); */
   sw_write_array(soft_serial,cmd_send,ARRAY_LENGHT);
  
   /*   ESP_LOGI(TAG,"VALUE BYTE: ");
   
   for (int i = 0; i < ARRAY_LENGHT; i++) {
        ESP_LOGI(TAG, "array_Send[%d] = 0x%02X", i, cmd_send[i]);
        
    } */
   
   free(cmd_send);
 }

 void send_value_float(SwSerial *soft_serial,float value,const unsigned char *addr){

   // ESP_LOGI(TAG,"***********************\n");
    unsigned char ARRAY_LENGHT = 10;
    unsigned char cmd_header[] = {0x5a,0xa5,0x00,0x82};
    unsigned char * cmd_send = (unsigned char *) malloc(ARRAY_LENGHT * sizeof(unsigned char));
   
    
    //add header to CMD
    memcpy(cmd_send,cmd_header,sizeof(cmd_header));
    //add component address to CMD
    memcpy(&cmd_send[4],addr,2);
    
    
    unsigned char float_to_byte_array[4];
    //sprintf(value_to_char,"%.2f",value);
   
    reverse_memcpy(float_to_byte_array,&value,4);
    //memcpy(float_to_byte_array,&value,4);
    //memcpy(value_to_char, &value, 8); 

    unsigned char value_to_float_size = 4;
   // ESP_LOGI(TAG,"value : %f : ",value);
    
    //gidecek verinin uzunluğu ekleniyor
    cmd_send[2] = (unsigned char)((int)value_to_float_size + 3);  
    
    for (size_t i = 0; i < value_to_float_size; i++)
    {
      cmd_send[6+i]=(unsigned char) float_to_byte_array[i];
    }

    sw_write_array(soft_serial,cmd_send,ARRAY_LENGHT);
    
    
    /*  ESP_LOGI(TAG,"VALUE BYTE FLOAT: ");
    
    for (int i = 0; i < ARRAY_LENGHT; i++) {
          ESP_LOGI(TAG, "array_Send[%d] = 0x%02X", i, cmd_send[i]);
      }
     */
    free(cmd_send);
 }


void data_clear(SwSerial *soft_serial,unsigned char *addr,int numberOfDigits){

  memcpy(&RESET_VALUE[4],addr,2);

  RESET_VALUE[2] = (unsigned char) (3 + numberOfDigits);
  for (size_t i = 0; i < numberOfDigits; i++)
  {
    RESET_VALUE[6+i] = "0x20";
  }
  
  for (size_t i = 0; i < ARRAY_LENGHT(RESET_VALUE); i++)
  {
    ESP_LOGI("Reset: ","deger : 0x%02X",RESET_VALUE[i]);
  }

 }

void sw_write_array(SwSerial *soft_serial,unsigned char const *array, size_t array_lenght){

       for (size_t i = 0; i < array_lenght; i++)
        {
            sw_write(soft_serial, array[i]);
        }
}
 
void reverse_memcpy(void* destination, const void* source, size_t size) {
    const unsigned char* src = (const unsigned char*)source;  // Kaynağı byte düzeyinde erişim için dönüştür
    unsigned char* dest = (unsigned char*)destination;        // Hedefi byte düzeyinde erişim için dönüştür

    for (size_t i = 0; i < size; i++) {
        dest[i] = src[size - 1 - i];  // Veriyi ters sırada kopyala
    }
}

void reset(SwSerial *soft_serial){
  sw_write_array(soft_serial,RESET,ARRAY_LENGHT(RESET));
}

#if 0

void get_scaled_number_value(double value, int precision, const char *unit, char *buffer, size_t size)
{
  static const char *mod[] = {
    "",
    "k",
    "M",
    "G",
    "T",
    "P"
  };

  int index = 0;
  while (value > 1000 && index < ARRAY_ITEMS(mod))
  {
    value /= 1000;
    index++;
  }

  snprintf(buffer, size, "%.*f %s%s", precision, value, mod[index], unit);
}


#endif