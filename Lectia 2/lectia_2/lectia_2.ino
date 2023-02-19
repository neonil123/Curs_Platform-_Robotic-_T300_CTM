#include <PS2X_lib.h>  //for v1.6// instalati libraria :)

/******************************************************************
   set pins connected to PS2 controller:
     - 1e column: original
     - 2e colmun: Stef?
   replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;


#define MotorB_S  6
#define MotorB_D  7
#define MotorA_S  9
#define MotorA_D  8



void setup() {
  pinMode(MotorA_S, OUTPUT);
  pinMode(MotorA_D, OUTPUT);
  pinMode(MotorB_S, OUTPUT);
  pinMode(MotorB_D, OUTPUT);

  digitalWrite(MotorA_S, LOW);
  digitalWrite(MotorB_S, LOW);





  Serial.begin(115200);

  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
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


/*
     digitalWrite(MotorB_S, HIGH);
  digitalWrite(MotorB_D, HIGH);
  delay(5000);
  digitalWrite(MotorB_D, LOW);
  delay(5000);
  digitalWrite(MotorB_S, LOW);


  digitalWrite(MotorA_S, HIGH);
  digitalWrite(MotorA_D, HIGH);
  delay(5000);
  digitalWrite(MotorA_D, LOW);
  delay(5000);
  digitalWrite(MotorA_S, LOW);
*/

#define speedo 200

void loop() {

  ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

  if (ps2x.Analog(PSS_LY) < 150 && ps2x.Analog(PSS_LY) > 100) {
    digitalWrite(MotorB_S, LOW);
  }
  else if (ps2x.Analog(PSS_LY) < 50) {

    analogWrite(MotorB_S, speedo);
    digitalWrite(MotorB_D, HIGH);
  }
  else if (ps2x.Analog(PSS_LY) > 250) {
    analogWrite(MotorB_S, speedo);
    digitalWrite(MotorB_D, LOW);
  }


  if (ps2x.Analog(PSS_RY) < 150 && ps2x.Analog(PSS_RY) > 100) {
    digitalWrite(MotorA_S, LOW);
  }
  else if (ps2x.Analog(PSS_RY) < 50) {
    analogWrite(MotorA_S, speedo);
    digitalWrite(MotorA_D, HIGH);
  }
  else if (ps2x.Analog(PSS_RY) > 250) {

    analogWrite(MotorA_S, speedo);
    digitalWrite(MotorA_D, LOW);
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
