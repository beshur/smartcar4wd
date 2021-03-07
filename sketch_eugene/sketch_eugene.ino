int IN1=9;
int IN2=8;

int IN3=4;
int IN4=2;

int ENA=6;
int ENB=3;
void setup()
{

  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);  
  pinMode(IN3, OUTPUT);  
  pinMode(IN4, OUTPUT);  
//  pinMode(ENA, OUTPUT);  
//  pinMode(ENB, OUTPUT);  
}

void stop() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}

void firstTest() {
  //  analogWrite(ENA,100);
//  analogWrite(ENB,200);
  // rotate CW
 digitalWrite(IN1,LOW);
 digitalWrite(IN2,HIGH);
 digitalWrite(IN3,LOW);
 digitalWrite(IN4,HIGH);
 delay(1000);
 // pause for 1S
 stop();
// analogWrite(ENA,0);
//  analogWrite(ENB,0);
 delay(1000);
 // rotate CCW
// analogWrite(ENA,50);
// analogWrite(ENB,100);
 digitalWrite(IN1,HIGH);
 digitalWrite(IN2,LOW);
 digitalWrite(IN3,HIGH);
 digitalWrite(IN4,LOW);
 delay(1000);
 // pause for 1S
// analogWrite(ENA,0);
//analogWrite(ENB,0);
  stop();
 delay(1000);
  
}

void loop()
{
  // rotate left
 digitalWrite(IN1,LOW);
 digitalWrite(IN2,HIGH);
 digitalWrite(IN3,HIGH);
 digitalWrite(IN4,LOW);
 delay(1000);
 // pause for 1S
 stop();
  // rotate right
 digitalWrite(IN1,HIGH);
 digitalWrite(IN2,LOW);
 digitalWrite(IN3,LOW);
 digitalWrite(IN4,HIGH);
 delay(1000);
 // pause for 1S
 stop();

}
