#include<PS2X_lib.h>

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

int speedo = 0;

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


bool LightF = false;

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

  if (ps2x.ButtonPressed(PSB_CIRCLE)) {
    Serial.println("Circle just pressed");
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
