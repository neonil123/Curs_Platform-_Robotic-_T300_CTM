#include<PS2X_lib.h>
#include "HCPCA9685.h"
#include <Adafruit_NeoPixel.h>

//pinii pentru Receiver
#define PS2_DAT        13
#define PS2_CMD        11
#define PS2_SEL        10
#define PS2_CLK        12

//pinii pentru driverul de la motoare
#define MotorB_S  6
#define MotorB_D  7
#define MotorA_S  9
#define MotorA_D  8

//pinii pentru ledu-uri
#define RightLight 4
#define LeftLight  5

// creăm un obiect PS2x
PS2X ps2x;
#define pressures   false
#define rumble      false
int error = 0;
byte type = 0;
byte vibrate = 0;

//inițializarea modulului cu 16 canale servo HCPCA9685
#define  I2CAdd 0x40
HCPCA9685 HCPCA9685(I2CAdd);

//pin buzzer
#define buzzPin 3

//pin banda led
#define ledStripPin 2
#define NUMPIXELS   6
Adafruit_NeoPixel pixels(NUMPIXELS, ledStripPin, NEO_GRB + NEO_KHZ800);

//variabile de tip flag
bool LightF = false;
bool StripF = false;

void setup() {
  //setăm pinii de la motoare
  pinMode(MotorA_S, OUTPUT);
  pinMode(MotorA_D, OUTPUT);
  pinMode(MotorB_S, OUTPUT);
  pinMode(MotorB_D, OUTPUT);

  digitalWrite(MotorA_S, LOW);
  digitalWrite(MotorB_S, LOW);

  //inițializăm pinii de la led-uri
  pinMode(RightLight, OUTPUT);
  pinMode(LeftLight, OUTPUT);

  //inițializăm comunicatrea serială
  Serial.begin(115200);
  delay(100);
  //inițializare HCPCA9685
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  HCPCA9685.Servo(3, 210);

  //setam pinul de la buzzer
  pinMode(buzzPin, OUTPUT);

  //initializam banda led
  pixels.begin();
  pixels.clear();

  //inițializăm controllerul PS2
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  if (error == 0) {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
}

void loop() {
  ps2x.read_gamepad(false, vibrate);


  //control motor stânga

  int leftStick = ps2x.Analog(PSS_LY);
  if (leftStick < 100) {
    leftStick = map(leftStick, 100, 0, 0, 255);
    leftStick = constrain(leftStick, 0, 255);
    analogWrite(MotorB_S,  leftStick);
    digitalWrite(MotorB_D, HIGH);

  }
  else if (leftStick > 170) {
    leftStick = map(leftStick, 170, 255, 0, 255);
    leftStick = constrain(leftStick, 0, 255);
    analogWrite(MotorB_S,  leftStick);
    digitalWrite(MotorB_D, LOW);
  }
  else {
    analogWrite(MotorB_S,  0);
  }


  //control motor dreapta
  int rightStick = ps2x.Analog(PSS_RY);
  if (rightStick < 100) {
    rightStick = map(rightStick, 100, 0, 0, 255);
    rightStick = constrain(rightStick, 0, 255);
    analogWrite(MotorA_S, rightStick);
    digitalWrite(MotorA_D, HIGH);
  }
  else if (rightStick > 170) {
    rightStick = map(rightStick, 170, 255, 0, 255);
    rightStick = constrain(rightStick, 0, 255);
    analogWrite(MotorA_S, rightStick);
    digitalWrite(MotorA_D, LOW);
  }
  else {
    analogWrite(MotorA_S, 0);
  }





  //Control lumini flash

  if (ps2x.ButtonPressed(PSB_TRIANGLE)) {
    Serial.println("Triangle just pressed");
    LightF = ! LightF;
  }

  if (LightF) {
    digitalWrite(RightLight, HIGH);
    digitalWrite(LeftLight , HIGH);
  }
  else {
    if (ps2x.Button(PSB_PAD_UP)) {
      Serial.println("Up pressed");
      digitalWrite(RightLight, HIGH);
      digitalWrite(LeftLight , HIGH);
    }
    else {
      digitalWrite(RightLight, LOW);
      digitalWrite(LeftLight, LOW);
    }
  }


  // control Gripper

  if (ps2x.Button(PSB_L1) ) {
    Serial.print("Gripper activated: ");
    int leftStickX = ps2x.Analog(PSS_LX);
    if (leftStickX > 200) {
      Serial.println("Right");
      HCPCA9685.Servo(3, 270);
    }
    else if (leftStickX < 50) {
      Serial.println("Left");
      HCPCA9685.Servo(3, 120);
    }
    else {
      HCPCA9685.Servo(3, 210);
      Serial.println();
    }
  }
  else {
    HCPCA9685.Servo(3, 210);
  }



  //Control pattern intoarcere 180 grade
  if (ps2x.ButtonPressed(PSB_CIRCLE)) {
    Serial.println("180 TURN");
    for (int i = 0; i < 10; i++) {
      ps2x.read_gamepad(false, vibrate);
      analogWrite(MotorB_S,  255);
      analogWrite(MotorA_S,  255);
      digitalWrite(MotorB_D, HIGH);
      digitalWrite(MotorA_D, LOW);
      delay(200);
    }
  }

  //Control pattern patrat
  if (ps2x.ButtonPressed(PSB_SQUARE)) {
    Serial.println("Sqare");

    for (int i = 0; i < 10; i++) {
      ps2x.read_gamepad(false, vibrate);
      analogWrite(MotorB_S,  255);
      analogWrite(MotorA_S,  255);
      digitalWrite(MotorB_D, HIGH);
      digitalWrite(MotorA_D, HIGH);
      delay(200);
    }

    for (int i = 0; i < 10; i++) {
      ps2x.read_gamepad(false, vibrate);
      analogWrite(MotorB_S,  255);
      analogWrite(MotorA_S,  255);
      digitalWrite(MotorB_D, HIGH);
      digitalWrite(MotorA_D, LOW);
      delay(100);
    }

    for (int i = 0; i < 10; i++) {
      ps2x.read_gamepad(false, vibrate);
      analogWrite(MotorB_S,  255);
      analogWrite(MotorA_S,  255);
      digitalWrite(MotorB_D, HIGH);
      digitalWrite(MotorA_D, HIGH);
      delay(200);
    }
    for (int i = 0; i < 10; i++) {
      ps2x.read_gamepad(false, vibrate);
      analogWrite(MotorB_S,  255);
      analogWrite(MotorA_S,  255);
      digitalWrite(MotorB_D, HIGH);
      digitalWrite(MotorA_D, LOW);
      delay(100);
    }
    for (int i = 0; i < 10; i++) {
      ps2x.read_gamepad(false, vibrate);
      analogWrite(MotorB_S,  255);
      analogWrite(MotorA_S,  255);
      digitalWrite(MotorB_D, HIGH);
      digitalWrite(MotorA_D, HIGH);
      delay(200);
    }
    for (int i = 0; i < 10; i++) {
      ps2x.read_gamepad(false, vibrate);
      analogWrite(MotorB_S,  255);
      analogWrite(MotorA_S,  255);
      digitalWrite(MotorB_D, HIGH);
      digitalWrite(MotorA_D, LOW);
      delay(100);
    }
    for (int i = 0; i < 10; i++) {
      ps2x.read_gamepad(false, vibrate);
      analogWrite(MotorB_S,  255);
      analogWrite(MotorA_S,  255);
      digitalWrite(MotorB_D, HIGH);
      digitalWrite(MotorA_D, HIGH);
      delay(200);
    }
  }

  // control buzzer/clacson
  if (ps2x.Button(PSB_CROSS)) {
    Serial.println("Buzzer");
    digitalWrite(buzzPin, HIGH);
  }
  else {
    digitalWrite(buzzPin, LOW);
  }


  //control banda Led emergency lights
  if (ps2x.ButtonPressed(PSB_R2)) {
    Serial.println("R2 just pressed");
    StripF = ! StripF;
  }
  if (StripF) {
    static bool tmp1 = false;
    pixels.clear();
    if (tmp1) {
      pixels.setPixelColor(0, pixels.Color(0, 0, 255));
      pixels.setPixelColor(1, pixels.Color(0, 0, 255));
      pixels.setPixelColor(2, pixels.Color(0, 0, 255));
      digitalWrite(RightLight, HIGH);
      digitalWrite(buzzPin, HIGH);
      pixels.show();
    } else {
      pixels.setPixelColor(3, pixels.Color(255, 0, 0));
      pixels.setPixelColor(4, pixels.Color(255, 0, 0));
      pixels.setPixelColor(5, pixels.Color(255, 0, 0));
      digitalWrite(buzzPin, LOW);
      digitalWrite(LeftLight , HIGH);
      pixels.show();
    }
    tmp1 = ! tmp1;
  }
  else {
    pixels.clear();
    pixels.show();
  }


  // afisare valori stick
  Serial.print("Stick Values:");
  Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
  Serial.print(",");
  Serial.print(ps2x.Analog(PSS_LX), DEC);
  Serial.print(",");
  Serial.print(ps2x.Analog(PSS_RY), DEC);
  Serial.print(",");
  Serial.println(ps2x.Analog(PSS_RX), DEC);

  delay(50);
}
