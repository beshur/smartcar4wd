#include <Servo.h>
#include <Wire.h>                //including the libraries of I2C

/*
    motor
*/
// right
int pinRF = 2;
int pinRB = 4;
// left
int pinLF = 7;
int pinLB = 8;

int powerR = 5;
int powerL = 6;

int outputR = 160;
int outputL = 248;

int outputRLess = 100;
int outputLLess = 80;

/*
   Ultrasonic
*/
int usEcho = 11;
int usTrig = 12;

int Fspeedd = 0;      // forward speed
int Rspeedd = 0;      // right speed
int Lspeedd = 0;      // left speed

char directionn = 0;   // front=8 back=2 left=4 right=6
Servo myservo;        // setting myservo
int delay_time = 250; // time for servo motor turning backward

char Fgo = 8;         // going forward
char Rgo = 6;         // turning right
char Lgo = 4;         // turning left
char Bgo = 2;         // turning backward

/*
   Bluetooth
*/
unsigned char Bluetooth_val
;       //defining variable val


void setup()
{
  Serial.begin(9600);
  pinMode(pinRF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(powerR, OUTPUT);
  pinMode(powerL, OUTPUT);

  analogWrite(powerR, outputR);
  analogWrite(powerL, outputL);

  pinMode(usTrig, OUTPUT);
  pinMode(usEcho, INPUT);

  myservo.attach(9);    // defining output pin9 of motor
  Serial.println("Car4WD init");
}

void setSpeed(int rightSpeed, int leftSpeed) {
  analogWrite(powerR, rightSpeed);
  analogWrite(powerL, leftSpeed);
}

void stop() {
//  setSpeed(100, 75);
//  delay(200);
//  setSpeed(50, 25);
//  delay(100);

  setSpeed(0, 0);
}

void fwd() {
  setSpeed(outputR, outputL);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinRB, LOW);
  digitalWrite(pinLF, HIGH);
  digitalWrite(pinLB, LOW);
}

void bcwd() {
  setSpeed(outputR, outputL);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinLF, LOW);
  digitalWrite(pinLB, HIGH);
}


void left() {
  setSpeed(outputR/2, outputL);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinRB, LOW);
  digitalWrite(pinLF, LOW);
  digitalWrite(pinLB, HIGH);
}


void right() {
  setSpeed(outputR, outputL/4);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinLF, HIGH);
  digitalWrite(pinLB, LOW);
}

void move(char nextDirection) {
  if (directionn != nextDirection) {
    directionn = nextDirection;
    stop();
    
    if (nextDirection == Fgo) {
      fwd();
    }
    if (nextDirection == Bgo) {
      bcwd();
    }
    if (nextDirection == Lgo) {
      left();
    }
    if (nextDirection == Rgo) {
      right();
    }
  }
}

void btLoop() {
  if (Serial.available()) //to judge whether the serial port receives the data.
  {
    Bluetooth_val = Serial.read(); //reading (Bluetooth) data of serial port,giving the value of val;
    Serial.println(Bluetooth_val);
    char nextDirection = 0;
    switch (Bluetooth_val)
    {
      case 'U': 
        nextDirection = Fgo;
        break;
      case 'D': 
        nextDirection = Bgo;
        break;
      case 'L': 
        nextDirection = Lgo;
        break;
      case 'R':
        nextDirection = Rgo;
        break;
      case 'S': stop();
        break;
    }

    move(nextDirection);
  }
}

void loop()
{
  btLoop();
}
