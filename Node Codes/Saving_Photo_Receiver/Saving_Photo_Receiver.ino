#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <EEPROM.h>            // read and write from flash memory
#include <string.h>
#include<CAN.h>

// Define the number of bytes you want to access
#define EEPROM_SIZE 1

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

int pictureNumber = 0;

#define TX_GPIO_NUM  3 // normally // Connects to CTX // Not Rx pin directly Tx pin
#define RX_GPIO_NUM 1 //normally // Connects to CRX// Not Tx pin directly Rx pin

int can_receiver_for_cam(int id);
void takePicture();

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector
 
  Serial.begin(115200);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
    return;
  }
  
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }

  // Initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  pictureNumber = EEPROM.read(0) + 1;

CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
     // start the CAN bus at 500 kbps
  if (!CAN.begin (500E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }

}


void loop() {
  
int received_data = can_receiver_for_cam(21);

  if (received_data >=15 && received_data <= 22 ) {
   takePicture();
   delay(1000);
  }


}

int can_receiver_for_cam(int id){
  String data;
  int send_data=0;
if (CAN.parsePacket()){
  // Read the received packet
    uint32_t canId = CAN.packetId();
    uint8_t len = CAN.packetDlc();
  uint8_t buf[8];
  if(canId=id){
    // Copy the data into buf
    CAN.readBytes(buf, len);
    }
    char array[8];
    for(int i=0;i<2;i++){
      array[i] = (char)buf[i];
      Serial.print(array[i]);
    }
    Serial.print(data);
     data = String(array).substring(0,2);
     send_data = data.toInt();
  }
 return send_data;
}

void takePicture(){
  
  //take picture
  camera_fb_t *pic = esp_camera_fb_get();
  if (!pic){
    Serial.println("Error when taking picture");
    return;
  }
  Serial.println("Picture taken!");
  //save Pic on SD Card
  EEPROM.begin(EEPROM_SIZE);
  pictureNumber=EEPROM.read(0) + 1;
  String pic_name = "/picture_"+String(pictureNumber)+".jpg";
  fs::FS &fs = SD_MMC;
  Serial.printf("%s: %d bytes\n", pic_name.c_str(), pic->len);

  File pic_file = fs.open(pic_name.c_str(), FILE_WRITE);
  if (!pic_file){
    Serial.println("Error when opening the file");
    return;
  }
  pic_file.write(pic->buf, pic->len);
  Serial.println("Saved file");
  EEPROM.write(0, pictureNumber);
  EEPROM.commit();
  pic_file.close();
  esp_camera_fb_return(pic);
}


