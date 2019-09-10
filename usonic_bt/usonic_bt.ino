//Base code made by www.elegoo.com
//Code edited by https://danielgray.me
// Jan 2018


#include <Servo.h> //servo library
Servo myservo; // create servo object to control servo

int Echo = A4;  
int Trig = A5; 

// motor control
int in1 = 7;
int in2 = 8;
int in3 = 9;
int in4 = 11;
int ENA = 5;
int ENB = 6;
int SPEED = 110;

int rightDistance = 0,leftDistance = 0,middleDistance = 0 ;
volatile int state = LOW;
char getstr;

//#define send 0    // see distance in console


// Ultrasonic distance helpers

int Distance_test()   
{
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);
  float echo_time = pulseIn(Echo, HIGH);  
  float distance_cm = echo_time / 58;       
  return (int) distance_cm;
}  

int take_right_distance()
{
    myservo.write(30);          
    delay(500);      
    int d = Distance_test();
    #ifdef send
    Serial.print("Right_cm=");
    Serial.println(d);
    #endif
    return d;
}

int take_left_distance()
{                                               
    myservo.write(180);              
    delay(100); 
    int d = Distance_test();
    #ifdef send
    Serial.print("Left_cm=");
    Serial.println(d);
    #endif
    return d; 
}

/// Move helpers

void left_wheels_fwd()
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
}

void left_wheels_back()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void right_wheels_fwd()
{
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void right_wheels_back()
{
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void fwd_helper()
{
 left_wheels_fwd();
 right_wheels_fwd();
}

void _mForward()
{
 analogWrite(ENA,SPEED);
 analogWrite(ENB,SPEED);
 fwd_helper();
 Serial.println("go forward!");
}

void _mBack()
{
 analogWrite(ENA,SPEED);
 analogWrite(ENB,SPEED);
 left_wheels_back();
 right_wheels_back();
 Serial.println("go back!");
}

// forward and left
void left()
{
  analogWrite( ENA, SPEED );  // left wheels going slower
  analogWrite( ENB, 255 );
  fwd_helper();
  Serial.println("Left");
}

// forward and right
void right()
{
  analogWrite( ENB, SPEED );  // right wheels going slower
  analogWrite( ENA, 255 );     
  fwd_helper();
  Serial.println("Right");
}

void rotate_left()
{
 analogWrite(ENA,SPEED);
 analogWrite(ENB,SPEED);
 left_wheels_back();
 right_wheels_fwd();
 Serial.println("rotate left!");
}

void rotate_right()
{
 analogWrite(ENA,SPEED);
 analogWrite(ENB,SPEED);
 left_wheels_fwd();
 right_wheels_back();
 Serial.println("rotate right!");
} 

void _mStop()
{
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  Serial.println("Stop!");
} 

// Distance check and movement when stuck
void handle_stuck()
{
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(500);                      
  digitalWrite(LED_BUILTIN, LOW);    
  delay(500);
      
  //Move back a tiny bit                       
  _mBack();
  delay(250);
  _mStop();
  delay(500);
  
  rightDistance = take_right_distance();

  //Re-center ultrasonic sensor
  myservo.write(90);              
  delay(100);
  
  leftDistance = take_left_distance();
  
  //Re-center ultrasonic sensor
  myservo.write(90);              
  delay(300);

  if(rightDistance>leftDistance)  
  {
    rotate_right();
    delay(1000);
    middlecheck();
    delay(1500);
   }
   else if(rightDistance<leftDistance)
   {
    rotate_left();
    delay(1000);
    middlecheck();
    delay(1500);
   }
   else if((rightDistance<=30)||(leftDistance<=30))
   {
    _mBack();
    delay(1000);
   }
}  

// Go forward cautiously
int middlecheck()
{
  myservo.write(90);
  int d = Distance_test();
  #ifdef send
  Serial.print("mid_cm=");
  Serial.println(d);
  #endif
  
  //Just quickly check that we aren't driving straight into something
  if(d<=30)
  {
    handle_stuck();
  }
  else if (getstr != 's')
  {
    _mForward();
  }
  return d;
}

// Alters global intention in 'getstr'
void checkbtinput() 
{
  getstr = Serial.read();
  switch (getstr) {
    case 'g': _mForward(); break;
    case 'l': left(); break;
    case 'r': right(); break;
    case 'R': rotate_right(); break;
    case 'L': rotate_left(); break;
    case 'b': _mBack(); break;
    case 's': _mStop(); break;
    default: break;
  }
}

void setup() 
{ 
  myservo.attach(3);// attach servo on pin 3 to servo object
  Serial.begin(9600);     
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  _mStop();
} 


void loop() 
{ 
    checkbtinput();
    delay(1000);      // go for 1 sec

    #ifdef manual_control
    return;
    #endif
    
    // Not stuck distance check

    middlecheck();
    delay(1000);    // if nothing in front, go for another 1sec
    
    rightDistance = take_right_distance();
    if (rightDistance<=15) 
    {
      handle_stuck();
    }

    //Re-center ultrasonic sensor
    myservo.write(90);              
    delay(100);
  
    leftDistance = take_left_distance();
    if (leftDistance<=15)
    {
      handle_stuck();
    } 

    //Re-center ultrasonic sensor
    delay(300);
    myservo.write(90);              
    middlecheck();
    delay(200);
}
