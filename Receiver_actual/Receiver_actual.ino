/* Padraig Lysandrou 2019 recever for Ebrd
 *
*/

#include <SoftwareSerial.h>
#include <Servo.h>


Servo left_motor;
Servo right_motor;
SoftwareSerial BLE(3, 2); // RX, TX
const int status_led = 8;
int throttle_value = 0;
const int min_PWM = 1000;
const int max_PWM = 1300;
const int left_motor_offset = 10;
const int right_motor_offset = 0;
String packet_RX = "";
unsigned int data;
unsigned long time_var = 0;

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
  BLE.setTimeout(30);

}

void loop() {
  while(true){
    if(BLE.available()>0) {
      time_var = micros();
      packet_RX = BLE.readString();
      data = packet_RX.toInt();
      Serial.println(data);
      digitalWrite(status_led, HIGH);
      if(data == 0 || data < 1000 || data > max_PWM){
        right_motor.writeMicroseconds(min_PWM);
        left_motor.writeMicroseconds(min_PWM);
        BLE.write(10);
      }
      right_motor.writeMicroseconds(data + right_motor_offset);
      left_motor.writeMicroseconds(data + left_motor_offset);
    }
    else if((micros() - time_var) >= 700000){
      Serial.print("BLE timeout:  ");
      Serial.println(micros() - time_var);
      right_motor.writeMicroseconds(min_PWM);
      left_motor.writeMicroseconds(min_PWM);
      digitalWrite(status_led, LOW);
      BLE.write(10);
    }
    else {}
    delay(10);
  }
}
