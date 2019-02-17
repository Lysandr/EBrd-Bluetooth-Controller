/* Padraig Lysandrou 2019 recever for Ebrd
 *
*/

#include <SoftwareSerial.h>
#include <Servo.h>


Servo left_motor;
Servo right_motor;
SoftwareSerial BLE(3, 2); // RX, TX
int status_led = 8;
int throttle_value = 0;
const int min_PWM = 1000;
const int max_PWM = 1500;
const int left_motor_offset = 10;
const int right_motor_offset = 0;
String packet_RX = "";
int data;
unsigned long time_var;

void setup() {
  // perform motor setup
  pinMode(status_led, OUTPUT);
  left_motor.attach(6);
  right_motor.attach(5);
  right_motor.writeMicroseconds(min_PWM);
  left_motor.writeMicroseconds(min_PWM);
  delay(1000);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {;}
  Serial.println("Initializing Debug Serial Port");

  // setup the bluetooth device
  BLE.begin(9600);
}

void loop() {
  while(true){
    if(BLE.available()) {
      time_var = micros();
      packet_RX = BLE.readString();
      Serial.println(packet_RX);
      data = packet_RX.toInt();
      digitalWrite(status_led, HIGH);
      right_motor.writeMicroseconds(data + right_motor_offset);
      left_motor.writeMicroseconds(data + left_motor_offset);
    }
    else if(time_var - micros() > 500000){
      right_motor.writeMicroseconds(min_PWM);
      left_motor.writeMicroseconds(min_PWM);
      digitalWrite(status_led, LOW);
      BLE.write(10);
    }
    else{
      right_motor.writeMicroseconds(min_PWM);
      left_motor.writeMicroseconds(min_PWM);
      digitalWrite(status_led, LOW);
      BLE.write(10);
    }
    delay(10);
  }
}
