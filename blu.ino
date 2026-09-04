#include <SoftwareSerial.h>

#define RX 11
#define TX 10
#define ENA 9
#define IN1 8 
#define IN2 7
#define IN3 5
#define IN4 4
#define ENB 3

SoftwareSerial blu (RX,TX);

#define power 110
#define leftpower 255

void setup() {
  // put your setup code here, to run once:
  blu.begin(9600);
  Serial.begin(9600);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
  char data;
  if(blu.available()>0)
  {
    data = blu.read();
    Serial.println(data);
  }

  if (data == 'F')
  {
    Forward();
  }else if (data == 'S')
  {
    Stop();
  }else if (data == 'R')
  {
    Right();
  }else if (data == 'L')
  {
    Left();
  }else if (data == 'B')
  {
     Backword();
  }
}



void Forward()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);

  analogWrite(ENA,power);
  analogWrite(ENB,leftpower);

}

void Right()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);

  analogWrite(ENA,power);
  analogWrite(ENB,leftpower);

}
void Left()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);

  analogWrite(ENA,power);
  analogWrite(ENB,leftpower);

}
void Stop()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);

  analogWrite(ENA,0);
  analogWrite(ENB,0);

}
void Backword()
{
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);

  analogWrite(ENA,power);
  analogWrite(ENB,leftpower);

}
