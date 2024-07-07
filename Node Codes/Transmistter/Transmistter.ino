#include <CAN.h> // Include CAN library
#include <NewPing.h>
#include <string>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define MAX_DISTANCE 200


#define TX_GPIO_NUM   17// Connects to CTX // Not Rx pin directly Tx pin
#define RX_GPIO_NUM  16 // Connects to CRX // Not Tx pin directly Rx pin

#define TRIGGER_PIN 18
#define ECHO_PIN 5
#define TRIGGER_PIN2 26
#define ECHO_PIN2 27

String stringValue;
String stringValue2;
String stringValue_for_acceleration;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);

String readSensors();
void dataSender();
String proximity1_reader();
String proximity2_reader();
String acceleration_reader();

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200); // Initialize serial communication
 
CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3); // Initialize CAN at 500kbps

 if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }

Serial.println("MPU6050 Found!");

mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);


}
}
void loop() {
dataSender();
}

String readSensors(){
return proximity2_reader()+proximity1_reader()+acceleration_reader();
}

void dataSender(){
String stringTotal= readSensors();
char charValue[stringTotal.length() + 1];

stringTotal.toCharArray(charValue, sizeof(charValue));
CAN.beginPacket(0x15);
  for(int i=0; i<stringTotal.length() + 1;i++){ 
    CAN.write(charValue[i]);
  }
CAN.endPacket();
delay(300);
}


String proximity1_reader(){
  delay(150);
int uS = sonar.ping();
int distance;
if (uS==0)
{
Serial.println("MAX: resetting sensor");
pinMode(ECHO_PIN, OUTPUT);
delay(150);
digitalWrite(ECHO_PIN, LOW);
delay(150);
pinMode(ECHO_PIN, INPUT);
delay(150);
}
else
{
Serial.print(" ");
Serial.print("Ping: ");
distance = uS / US_ROUNDTRIP_CM;
Serial.print(uS / US_ROUNDTRIP_CM);
Serial.println("cm");
Serial.print("string:");

 stringValue = String(distance);
Serial.print(stringValue);
}

if(distance>99 || distance<10){
stringValue = "00";
}
return stringValue;
}



String proximity2_reader(){
  delay(150);
int uS = sonar2.ping();
int distance2;
if (uS==0)
{
Serial.println("MAX: resetting sensor");
pinMode(ECHO_PIN2, OUTPUT);
delay(150);
digitalWrite(ECHO_PIN2, LOW);
delay(150);
pinMode(ECHO_PIN2, INPUT);
delay(150);
}
else
{
Serial.print(" ");
Serial.print("Ping: ");
 distance2 = uS / US_ROUNDTRIP_CM;
Serial.print(uS / US_ROUNDTRIP_CM);
Serial.println("cm");
Serial.print("string2:");

stringValue2 = String(distance2);
Serial.print(stringValue2);
}
if(distance2>99 || distance2<10){
 stringValue2 ="00";
}
return stringValue2;
}

String acceleration_reader(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

float acceleration_x = abs(a.acceleration.x);

Serial.print("Acceleration:");
Serial.print(a.acceleration.x);
Serial.println("Y");
 stringValue_for_acceleration = String(acceleration_x);
 if(abs(a.acceleration.x) < 1){
  stringValue_for_acceleration = "0" +  stringValue_for_acceleration;
 }
  Serial.print(stringValue_for_acceleration);
  return stringValue_for_acceleration;
}
