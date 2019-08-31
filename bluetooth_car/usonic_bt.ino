//Base code made by www.elegoo.com
//Code edited by https://danielgray.me
// Jan 2018








#include <Servo.h> //servo library
Servo myservo; // create servo object to control servo
int Echo = A4;  
int Trig = A5; 
int in1 = 6;
int in2 = 7;
int in3 = 8;
int in4 = 9;
int ENA = 5;
int ENB = 11; 
int ABS = 110;
int rightDistance = 0,leftDistance = 0,middleDistance = 0 ;
volatile int state = LOW;
char getstr;

 /*Ultrasonic distance measurement Sub function*/
int Distance_test()   
{
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance/58;       
  return (int)Fdistance;
}  

void _mForward()
{
 analogWrite(ENA,ABS);
 analogWrite(ENB,ABS);
 digitalWrite(in1,HIGH);
 digitalWrite(in2,LOW);
 digitalWrite(in3,HIGH);
 digitalWrite(in4,LOW);
 Serial.println("go forward!");
}

void _mBack()
{
 analogWrite(ENA,ABS);
 analogWrite(ENB,ABS);
 digitalWrite(in1,LOW);
 digitalWrite(in2,HIGH);
 digitalWrite(in3,LOW);
 digitalWrite(in4,HIGH);
 Serial.println("go back!");
}

void _mleft()
{
 analogWrite(ENA,ABS);
 analogWrite(ENB,ABS);
 digitalWrite(in1,LOW);
 digitalWrite(in2,HIGH);
 digitalWrite(in3,HIGH);
 digitalWrite(in4,LOW);
 Serial.println("go left!");
}

void _mright()
{
 analogWrite(ENA,ABS);
 analogWrite(ENB,ABS);
 digitalWrite(in1,HIGH);
 digitalWrite(in2,LOW);
 digitalWrite(in3,LOW);
 digitalWrite(in4,HIGH);
 Serial.println("go right!");
} 
void _mStop()
{
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  Serial.println("Stop!");
} 
//Checking function
void ultrasonicCheck()
{
  digitalWrite(LED_BUILTIN, HIGH);   
      delay(500);                      
      digitalWrite(LED_BUILTIN, LOW);    
      delay(500);
          
      //Move back a tiny bit
      _mStop();
      delay(300);                         
      _mBack();
      delay(150);
      _mStop();
      delay(500);
      // Take right measurement                     
      myservo.write(30);          
      delay(1000);      
      rightDistance = Distance_test();

      #ifdef send
      Serial.print("rightDistance=");
      Serial.println(rightDistance);
      #endif

      // Take left measurement
      delay(500);
      myservo.write(90);      
      delay(1000);                                                  
      myservo.write(180);              
      delay(1000); 
      leftDistance = Distance_test(); 

      #ifdef send
      Serial.print("leftDistance=");
      Serial.println(leftDistance);
      #endif

      //Re-center ultrasonic sensor
      delay(300);
      myservo.write(90);              
      delay(300);


      if(rightDistance>leftDistance)  
      {
        _mright();
        delay(100);
        middlecheck();
        delay(1500);
       }
       else if(rightDistance<leftDistance)
       {
        _mleft();
        delay(100);
        middlecheck();
        delay(1500);
       }
       else if((rightDistance<=30)||(leftDistance<=30))
       {
        _mBack();
        delay(500);
       }
       else if (middleDistance<=30) {
        _mForward();
        delay(200);
        middlecheck(); 
      }
       else
       {
        _mForward();
        delay(100);
        middlecheck();
       }
    }  

void middlecheck()
{
  myservo.write(90);
  middleDistance = Distance_test();
      //Just quickly check that we aren't driving straight into something
      if(middleDistance<=30)
      {
      ultrasonicCheck();
      } else {
        if (getstr == 'g') {
        _mForward();
      }
  }
}

void checkbtinput() 
{
  getstr = Serial.read();
    if(getstr == 'g'){
      _mForward();
    } else if (getstr == 's') {
      _mStop();
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
    middlecheck();
    delay(1000);
    middlecheck();
    delay(1000);
    #ifdef send
    Serial.print("middleDistance=");
    Serial.println(middleDistance);
    #endif
      // Take right measurement                     
      myservo.write(30);          
      delay(500);      
      rightDistance = Distance_test();
      if (rightDistance<=15) 
      {
        ultrasonicCheck();
      }
      checkbtinput();
      #ifdef send
      Serial.print("rightDistance=");
      Serial.println(rightDistance);
      #endif

      // Take left measurement
      delay(100);
      myservo.write(90);   
      delay(100);
      middlecheck();
      delay(100);                                                  
      myservo.write(180);              
      delay(100); 
      checkbtinput();
      leftDistance = Distance_test(); 
      if (leftDistance<=15)
      {
        ultrasonicCheck();
      } 
      #ifdef send
      Serial.print("leftDistance=");
      Serial.println(leftDistance);
      #endif

      //Re-center ultrasonic sensor
      delay(300);
      myservo.write(90);              
      middlecheck();
      delay(200);
   
}

