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

int car_speed = 150;
int in_bluetooth = 0;

void forward(){ 
  analogWrite(ENA, car_speed);
  analogWrite(ENB, car_speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {
  analogWrite(ENA, car_speed);
  analogWrite(ENB, car_speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

/// Both forward, left slower, right faster
void left()
{
  analogWrite( ENA, car_speed - 50 );  // left wheels going slower
  analogWrite( ENB, car_speed );
  digitalWrite(IN1,HIGH);    
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);    
  digitalWrite(IN4,HIGH); 
  Serial.println("Left");
}

// Right Slower, Left faster
void right()
{ 
  analogWrite( ENA, car_speed );
  analogWrite( ENB, car_speed - 50 );   // Right wheels going slower
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  Serial.println("Right");
}

void back_left()
{
    analogWrite( ENA, car_speed - 50 );
    analogWrite( ENB, car_speed );
    digitalWrite( IN1, LOW );
    digitalWrite( IN2, HIGH );
    digitalWrite( IN3, HIGH );
    digitalWrite( IN4, LOW );
    Serial.println("Back Left");
}

void back_right ()
{
    analogWrite( ENA, car_speed );
    analogWrite( ENB, car_speed - 50 );
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
int distance_test() 
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

void setup() 
{ 
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

/// 0 <= direction <= 180 servo direction in degrees.
/// returns distance in cm 
int check_distance( int direction, char* what )
{  
  if ( myservo.read() != direction ) {
     myservo.write( direction );
     delay(300);
  }
  int dist_cm = distance_test();
  Serial.print( what );
  Serial.print( " (cm): " );
  Serial.println ( dist_cm );
  return dist_cm;
}

void loop() 
{ 
	// Check Bluetooth
	
  if (Serial.available() > 0) {
      int cmd = Serial.read();
      switch ( cmd ) {
          case 'f': forward(); break;
          case 'b': back();   break;
          case 'l': left();   break;
          case 'L': back_left(); break;
          case 'R': back_right(); break;
          case 'r': right();  break;
          case 's': stop();   break;
          case 'a': stateChange(); break;
          default:  break;
      }
      in_bluetooth = 0;
	    return;
  } 
  
  // Drive a bit if just given a bluetooth command, checking bluetooth every 0.1 sec
  if ( in_bluetooth < 100 ) {
    in_bluetooth += 10;
    delay(100);
    return;
  }
  
  // Ultrasonic obstical avoidance
  
  int left_distance = 0;
  int right_distance = 0;
  int middle_distance = 0;
	
  // check straight ahead
	
  middle_distance = check_distance( 90, "front" );
  
  if (middle_distance <= 20) {     // Something is close
    
    stop();
    
    right_distance = check_distance( 50, "right" );
    left_distance  = check_distance( 130, "left" );

    // direction control
    
    while ( (right_distance <= 20) or (left_distance <= 20) ) 
    {
      back();
      delay(2000);
      right_distance = check_distance( 50, "right" );
      left_distance  = check_distance( 130, "left" );
    }
    
    if(right_distance > left_distance) {
      right();
      delay(1000);
    }
    else if(right_distance < left_distance) {
      left();
      delay(1000);
    }
    return;
  }

  forward();                   
}

