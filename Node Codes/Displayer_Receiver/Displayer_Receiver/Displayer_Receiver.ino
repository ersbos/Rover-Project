#include <CAN.h> // Include CAN library
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <string>
LiquidCrystal_I2C lcdekranim(0x27,16,2);

#define TX_GPIO_NUM   17// Connects to CTX // Not Rx pin directly Tx pin
#define RX_GPIO_NUM  16 // Connects to CRX // Not Tx pin directly Rx pin

void lcd_printer_for_acceleration(int id,int pos_x,int pos_y);
void dataLCDprinter_for_prox(int id,int def,int pos_x,int pos_y);


void setup() {
  Serial.begin(115200); // Initialize serial communication
  
CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3); // Initialize CAN at 500kbps
 lcdekranim.init();
  lcdekranim.backlight();
  lcdekranim.setCursor(0,0);

}

void loop() {
 dataLCDprinter_for_prox(21,0,1,0);
 dataLCDprinter_for_prox(21,2,0,1);
 lcd_printer_for_acceleration(21,8,0);
}

void dataLCDprinter_for_prox(int id,int def,int pos_x,int pos_y){
if (CAN.parsePacket()){
  // Read the received packet
    uint32_t canId = CAN.packetId();
    uint8_t len = CAN.packetDlc();
  uint8_t buf[8];
  if(canId=id){
    // Copy the data into buf
    CAN.readBytes(buf, len);
    
    }
  lcdekranim.setCursor(pos_x,pos_y);
 lcdekranim.print("di2:");
    for(int i=def;i<def+2;i++){
 lcdekranim.print((char)buf[i]);
 } 
}
}


void lcd_printer_for_acceleration(int id,int pos_x,int pos_y){
   if (CAN.parsePacket()) {
    // Read the received packet
    uint32_t canId = CAN.packetId();
    uint8_t len = CAN.packetDlc();
  uint8_t buf[8];
  String data;
    if(canId=id){
    // Copy the data into buf
    CAN.readBytes(buf, len);
    }
    // Print the received data
    Serial.print("Received packet from ID 0x");
    Serial.print(canId, HEX);
    Serial.print(", Data: ");
    for(int i = 0; i < len-1; i++) {
      Serial.print((char)buf[i]);
      
    }
    Serial.println();
 lcdekranim.setCursor(pos_x,pos_y);
 lcdekranim.print("XAc:");
 for(int i=4;i<8;i++){
 lcdekranim.print((char)buf[i]);
 }
 lcdekranim.print("");
}}
