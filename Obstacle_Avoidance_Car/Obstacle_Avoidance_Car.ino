// Fionn and Daddy's version, Oct-17, Jan-18
// Adapted from original, www.elegoo.com

#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

int Echo = A4;  
int Trig = A5; 

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define LED 13

int carSpeed = 150;
int rightDistance = 0, leftDistance = 0, middleDistance = 0;

void forward(){ 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

/// Both forward, left slower, right faster
void left()
{
  analogWrite( ENA, carSpeed - 50 );  // left wheels going slower
  analogWrite( ENB, carSpeed );
  digitalWrite(IN1,HIGH);    
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);    
  digitalWrite(IN4,HIGH); 
  Serial.println("Left");
}

// Right Slower, Left faster
void right()
{ 
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed - 50 );   // Right wheels going slower
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  Serial.println("Right");
}

void back_left()
{
	analogWrite( ENA, carSpeed - 50 );
	analogWrite( ENB, carSpeed );
	digitalWrite( IN1, LOW );
	digitalWrite( IN2, HIGH );
	digitalWrite( IN3, HIGH );
	digitalWrite( IN4, LOW );
	Serial.println("Back Left");
}

void back_right ()
{
	analogWrite( ENA, carSpeed );
	analogWrite( ENB, carSpeed - 50 );
	digitalWrite( IN1, LOW );
	digitalWrite( IN2, HIGH );
	digitalWrite( IN3, HIGH );
	digitalWrite( IN4, LOW );
	Serial.println("Back Right");
}


void toggle_light()
{
  static bool state = 0; 
  state = !state;
  digitalWrite(LED, state);
  Serial.println("Light");  
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
} 

// Ultrasonic distance measurement. Returns distance in cm.
int distance_test() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float echo_time = pulseIn(Echo, HIGH);  
  float distance_cm = echo_time / 58;       
  return (int) distance_cm;
}  

void setup() { 
  myservo.attach(3);  // attach servo on pin 3 to servo object
  Serial.begin(9600);     
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
} 

void loop() { 
    myservo.write(90);  //setservo position according to scaled value
    delay(500); 
    middleDistance = distance_test();
    Serial.print( "Distance (cm): " );
    Serial.println ( middleDistance );
    
    if (middleDistance <= 20) {     // Something is close
        
      // scan echos
      stop();
      myservo.write(50);          
      delay(200);      
      rightDistance = distance_test();
      
      delay(100);
      myservo.write(90); 
      delay(500); 
      myservo.write(130);
      delay(500); 
      leftDistance = distance_test();

      Serial.print("Distance left: ");
      Serial.print(leftDistance);
      Serial.print(" right ");
      Serial.println(rightDistance);
      
      delay(100);
      myservo.write(90);

        // direction control
      if(rightDistance > leftDistance) {
        right();
        delay(1000);
      }
      else if(rightDistance < leftDistance) {
        left();
        delay(1000);
      }
	  // TODO: move first
      else if ((rightDistance <= 20) or (leftDistance <= 20)) {
        back();
        delay(2000);
      }
	  /*
	  while ( right < 5 or left < 5 ) {
	    measure
	    back(500)
	  }
	  */
      return;
    }
    
    forward();                   
}

