#include <LinkedList.h>
#include <Gaussian.h>
#include <GaussianAverage.h>
#include <Servo.h>

#define trigPin1 22
#define echoPin1 23
#define trigPin2 24
#define echoPin2 25
#define trigPin3 26
#define echoPin3 27
#define trigPin4 28
#define echoPin4 29
#define trigPin5 30
#define echoPin5 31

#define servoPin1 6
#define servoPin2 2 //TBA
#define servoPin3 3//TBA
#define servoPin4 4
#define servoPin5 5

//later on add two more, use arduino mega

#define MAX_LED map(3.3, 0, 5, 0, 255) //maps 3.3V to the max PWM

long sensor1_val, sensor2_val, sensor3_val, sensor4_val, sensor5_val;
int servo1_state, servo2_state, servo3_state, servo4_state, servo5_state;
  
GaussianAverage sensor1_avg = GaussianAverage(3); //default 4 samples
GaussianAverage sensor2_avg = GaussianAverage(3); //default 4 samples
GaussianAverage sensor3_avg = GaussianAverage(3); //default 4 samples
GaussianAverage sensor4_avg = GaussianAverage(3); //default 4 samples
GaussianAverage sensor5_avg = GaussianAverage(3); //default 4 samples

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;

void setup()
{
  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  pinMode(trigPin5, OUTPUT);
  pinMode(echoPin5, INPUT);
  servo1_state = 0;
  servo2_state = 0;
  servo3_state = 0;
  servo4_state = 0;
  servo5_state = 0;
  servo_1.attach(servoPin1);
  servo_2.attach(servoPin2);
  servo_3.attach(servoPin3);
  servo_4.attach(servoPin4);
  servo_5.attach(servoPin5);

}

void loop(){
  sensor1_val = update_sensor(trigPin1, echoPin1, sensor1_avg);
  sensor2_val = update_sensor(trigPin2, echoPin2, sensor2_avg);
  sensor3_val = update_sensor(trigPin3, echoPin3, sensor3_avg);
  sensor4_val = update_sensor(trigPin4, echoPin4, sensor4_avg);
  sensor5_val = update_sensor(trigPin5, echoPin5, sensor5_avg);
  servo1_state = update_servo(sensor1_val, servo1_state, servo_1);
  servo2_state = update_servo(sensor2_val, servo2_state, servo_2);
  servo3_state = update_servo(sensor3_val, servo3_state, servo_3);
  servo4_state = update_servo(sensor4_val, servo4_state, servo_4); 
  servo5_state = update_servo(sensor5_val, servo5_state, servo_5); 
/*
  Serial.print(servo1_state);
  Serial.print(" - ");
  Serial.print(servo2_state);
  Serial.print(" - ");
  Serial.print(servo3_state);
  Serial.print(" - ");
  Serial.print(servo4_state);
  Serial.print(" - ");
  Serial.println(servo5_state);
*/
  
  Serial.print(sensor1_val);
  Serial.print(" - ");
  Serial.print(sensor2_val);
  Serial.print(" - ");
  Serial.print(sensor3_val);
  Serial.print(" - ");
  Serial.print(sensor4_val);
  Serial.print(" - ");
  Serial.println(sensor5_val);
  
}

long SonarSensor(int trigPin,int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  double duration;
  duration = pulseIn(echoPin, HIGH, 3000); //timeout in microseconds
  //distance = (duration/2)/29.1; //cm
  return duration;  
}

long update_sensor(int trigPin, int echoPin, GaussianAverage sensor_avg){
  sensor_avg.add(SonarSensor(trigPin, echoPin));
  sensor_avg.process();
  return sensor_avg.mean;
}

int update_servo (long sensor_val, int servo_state, Servo servo){
 if (sensor_val > 200){ 
    servo_state+=5; 
    if(servo_state >= 180){
      servo_state = 180; }
  }
  else{
    servo_state-=5;  
    if (servo_state <= 0){
      servo_state = 0;}
  }
  servo.write(servo_state);
  return servo_state;
}

void update_led (int ledPin, int led_state, int servo_state){
  led_state = map(servo_state, 0, 180, 0, MAX_LED);
  analogWrite(ledPin, led_state);
}

