#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ESP32Servo.h>


enum ControlMode{
    BLUETOOTH,
    RADIO
};

void readBluetooth();
void readRadio();

ControlMode currentMode = BLUETOOTH;
BluetoothSerial SerialBT;
String receivedData = "";
int leftState = 0;
int rightState = 0;
int flipperState = 0;
bool connected = false;


//pins
#define IN1 0
#define IN2 0
#define IN3 0
#define IN4 0
#define SERVO_PIN 0

//SERVO PARAMETERS
ESP32Servo servo;
#define SERVO_MAX 180
#define SERVO_MIN 0
#define SERVO_STEP 4

//setting up
void setup_servo();
void setup_communication();

//control
void handleDrive();
void handleFlipper();
void handleSaw();
void hardBreak();



void setup() {
  Serial.begin(115200);

  pinMode(SERVO_PIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  setup_communication();
  setup_servo();
  hardBreak();
}

void loop() {
  if (currentMode == BLUETOOTH) {
        readBluetooth();
    } else {
        readRadio();
    }

  if (connected) {
        handleDrive();
        handleFlipper();
        handleSaw();
    } else {
        hardBreak();
        idleServo();
    }
}



//SERVO SETTINGS
void setup_servo(){
  servo.attach(SERVO_PIN, SERVO_MIN, SERVO_MAX);
}
void handleServo(){
}
void idleServo(){
  servo.write(SERVO_MIN);
}

//MOTOR SETTINGS
void handleDrive(){
  digitalWrite();
  digitalWrite();
}

void hardBreak(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//COMMUNICATION SETTINGS
void setup_communication(){

}
void readBluetooth() {
  while (SerialBT.available()) {
    char c = SerialBT.read();
    
    //left, right, servo
    if (c == '\n') {
      int firstComma = receivedData.indexOf(',');
      int secondComma = receivedData.indexOf(',', firstComma + 1);

      if (firstComma != -1 && secondComma != -1) {
        leftState = receivedData.substring(0, firstComma).toInt();

        rightState = receivedData.substring(
          firstComma + 1, secondComma
        ).toInt();

        flipperState = receivedData.substring(
          secondComma + 1
        ).toInt();
      }

      receivedData = "";
    }
    else if (receivedData.length() < 40) {
      receivedData += c;
    }
  }
}

void readRadio(){

}