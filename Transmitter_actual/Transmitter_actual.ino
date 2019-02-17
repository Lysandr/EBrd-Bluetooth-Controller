/* Padraig Lysandrou 2019
 * BLE Controller for my electric longboard
 * 
 */
#include <SoftwareSerial.h>
#include <Servo.h>
SoftwareSerial BLE(3, 2); // RX, TX
int status_led = 8;
const int throttle_pin = A0;
int throttle_value = 0;
const float min_PWM = 1000;
const float max_PWM = 1300;
const float min_ADC = 370;
const float max_ADC = 630;
float throttle_calc = 0.0;
int last_throttle_val = 0;


void setup() {
  // configure LED as output
  pinMode(status_led, OUTPUT);
  pinMode(throttle_pin, INPUT);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {;}
  Serial.println("Initializing Debug Serial Port");

  // set the data rate for the SoftwareSerial port
  BLE.begin(9600);
}


void loop() {
  // check if slave is sending things...
  if (BLE.available()) {
    if(BLE.read() == 10){
      Serial.println("Received Error");
      BLE.print(min_PWM);
      BLE.println("");
    }
    digitalWrite(status_led, HIGH);
  }
  else{digitalWrite(status_led,LOW);}

  // send what is sent over computer serial port.
  if (Serial.available()) {
    BLE.print(Serial.read());
    BLE.println("");
    digitalWrite(status_led, HIGH);
  }
  else{digitalWrite(status_led,LOW);}


  // read the throttle finger, remap, and transmit
  throttle_value = analogRead(throttle_pin);
  Serial.print(throttle_value);
  Serial.print("   ");
  if(throttle_value < min_ADC){
    throttle_calc = min_PWM;
  }
  else if(throttle_value > max_ADC && throttle_value <= 700){
    throttle_calc = max_PWM;
  }
  else if(throttle_value > 700){
    throttle_calc = min_PWM;
  }
  else{ // span what we want
    throttle_calc = min_PWM + ((throttle_value - min_ADC)*((max_PWM - min_PWM)/(max_ADC - min_ADC)));
  }
  throttle_value = (int) ((throttle_calc + (3*last_throttle_val))/4);
  if(throttle_value < min_PWM){throttle_value = min_PWM;}
  Serial.print(throttle_value); Serial.print("   ");
  Serial.println(throttle_value);
  BLE.print(throttle_value);
  BLE.println("");
  delay(100);
  last_throttle_val = throttle_value;
}











