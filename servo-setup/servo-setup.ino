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

int outputR = 120;
int outputL = 160;

int outputRLess = 75;
int outputLLess = 100;

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
unsigned char Bluetooth_val;       //defining variable val


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
  setSpeed(100, 75);
  delay(200);
  setSpeed(50, 25);
  delay(100);

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
  setSpeed(outputR, outputL/2);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinLF, HIGH);
  digitalWrite(pinLB, LOW);
}

void detection()        //measuring 3 angles(0.90.179)
{
  ask_pin_F();            // reading out the front distance
  //  Serial.print("pinF Fspeedd ");
  //  Serial.println(Fspeedd);
  if (Fspeedd < 20)        // assuming the front distance less than 10cm
  {
    stop();               // clear output material
    delay(100);
    bcwd();                // going backward for 0.2 second
    delay(200);
  }

  if (Fspeedd < 40)        // assuming the front distance less than 25cm
  {
    stop();
    delay(100);             // clear output material
    ask_pin_L();            // reading out the left distance
    delay(delay_time);      // waiting servo motor to be stable
    ask_pin_R();            // reading out the right distance
    delay(delay_time);      // waiting servo motor to be stable

    if (Lspeedd > Rspeedd)  //assuming left distance more than  right distance
    {
      directionn = Lgo;      //turning left
    }

    if (Lspeedd <= Rspeedd)  //assuming left distance less than or equal to right distance
    {
      directionn = Rgo;      //turning right
    }

    if (Lspeedd < 15 && Rspeedd < 15)   //assuming both left distance and right distance less than  10cm
    {
      directionn = Bgo;      //going backward
    }
  }
  else                      //assuming the front distance  more than 25 cm
  {
    directionn = Fgo;        //going forward
  }

  Serial.print("Detection:");
  Serial.println(directionn);
}
void ask_pin_F()   // measuring the front distance
{
  myservo.write(110);
  digitalWrite(usTrig, LOW);   // ultrasonic launching low voltage at 2μs
  delayMicroseconds(2);
  digitalWrite(usTrig, HIGH);  // ultrasonic launching high voltage at 10μs，at least at10μs
  delayMicroseconds(10);
  digitalWrite(usTrig, LOW);    // keeping ultrasonic launching low voltage
  float Fdistance = pulseIn(usEcho, HIGH);  // time of error reading
  Serial.print("Fdistance");
  Serial.println(Fdistance);

  Fdistance = Fdistance / 5.8 / 10;  // converting time into distance（unit：cm）
  Fspeedd = Fdistance;              // reading-in Fspeedd(fore speed) with distance
}
void ask_pin_L()   // measuring left distance
{
  myservo.write(180);
  delay(delay_time);
  digitalWrite(usTrig, LOW);   // ultrasonic launching low voltage at 2μs
  delayMicroseconds(2);
  digitalWrite(usTrig, HIGH);  // ultrasonic launching high voltage at 10μs，at least at10μs
  delayMicroseconds(10);
  digitalWrite(usTrig, LOW);    // keeping ultrasonic launching low voltage
  float Ldistance = pulseIn(usEcho, HIGH);  // time of error reading
  Ldistance = Ldistance / 5.8 / 10;  // converting time into distance（unit：cm）
  Lspeedd = Ldistance;              //reading-in Lspeedd(left speed) with distance
}
void ask_pin_R()   // measuring right distance
{
  myservo.write(40);
  delay(delay_time);
  digitalWrite(usTrig, LOW);   // ultrasonic launching low voltage at 2μs
  delayMicroseconds(2);
  digitalWrite(usTrig, HIGH);  // ultrasonic launching high voltage at 10μs，at least at10μs
  delayMicroseconds(10);
  digitalWrite(usTrig, LOW);    // keeping ultrasonic launching low voltage
  float Rdistance = pulseIn(usEcho, HIGH);  // time of error reading
  Rdistance = Rdistance / 5.8 / 10;  // onverting time into distance（unit：cm）
  Rspeedd = Rdistance;              // reading-in Rspeedd(right speed) with distance
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

void servoLoop() {
//  myservo.write(110);  //making motor regression, being ready for next measurement
  detection();        //measuring angle and deciding which direction it moves towards

  if (directionn == 0) {
    delay(500);
  }
  if (directionn == Fgo) //supposing direction = 2(back up)
  {
    bcwd();
    delay(800);                    //  back up
    left() ;
    delay(200);              //moving slightly towards left(avoiding locked)
  }
  if (directionn == Rgo)          //supposing direction = 6(turning right)
  {
    bcwd();
    delay(100);
    right();
    delay(600);                 // turning right
  }
  if (directionn == Lgo)         //supposing direction = 4(turning left)
  {
    bcwd();
    delay(600);
    left();
    delay(600);                  // turning left
  }
  if (directionn == Bgo)         //supposing direction =  = 8(going forwards)
  {
    fwd();                 // going forwards normally
    delay(100);
  }

}

void loop()
{
  btLoop();
}
