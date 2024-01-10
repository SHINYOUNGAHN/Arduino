#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <IRremote.h>

#define RECV_PIN 2
#define POWER 0x00FF629D
#define A 0x00FF22DD
#define B 0x00FF02FD
#define C 0x00FFC23D
#define UP 0x00FF9867
#define DOWN 0x00FF38C7
#define LEFT 0x00FF30CF
#define RIGHT 0x00FF7A85
#define SELECT 0x00FF18E7

#define ENABLE_A 10  
#define IN1_A 9   
#define IN2_A 8   
#define ENABLE_B 5  
#define IN3_B 7   
#define IN4_B 6  

#define trg 11
#define echo 12

byte speedDC;

long duration, distance;

IRrecv irrecv(RECV_PIN);
decode_results decodedSignal;

SemaphoreHandle_t mutex = NULL;

void motor_speed(uint32_t sp){
  switch(sp){
    case A:
      speedDC = 127;
      analogWrite(ENABLE_A, speedDC);
      analogWrite(ENABLE_B, speedDC);
      Serial.println("A");
      break;

    case B:
      speedDC = 179;
      analogWrite(ENABLE_A, speedDC);
      analogWrite(ENABLE_B, speedDC);
      Serial.println("B");
      break;

    case C:
      speedDC = 255;
      analogWrite(ENABLE_A, speedDC);
      analogWrite(ENABLE_B, speedDC);
      Serial.println("C");
      break;
  }
}

void motor_dir(uint32_t dir){
  switch(dir){
    case UP:
      digitalWrite(IN1_A,LOW);
      digitalWrite(IN2_A,HIGH);
      digitalWrite(IN3_B,LOW);
      digitalWrite(IN4_B,HIGH);
      Serial.println("UP");
      break;

    case DOWN:
      digitalWrite(IN1_A,HIGH);
      digitalWrite(IN2_A,LOW);
      digitalWrite(IN3_B,HIGH);
      digitalWrite(IN4_B,LOW);
      Serial.println("DOWN");
      break;

    case RIGHT:
      digitalWrite(IN1_A,LOW);
      digitalWrite(IN2_A,HIGH);
      digitalWrite(IN3_B,LOW);
      digitalWrite(IN4_B,LOW);
      Serial.println("RIGHT");
      break;
      
    case LEFT:
      digitalWrite(IN1_A,LOW);
      digitalWrite(IN2_A,LOW);
      digitalWrite(IN3_B,LOW);
      digitalWrite(IN4_B,HIGH);
      Serial.println("LEFT");
      break;
      
    case POWER:
      digitalWrite(IN1_A,LOW);
      digitalWrite(IN2_A,LOW);
      digitalWrite(IN3_B,LOW);
      digitalWrite(IN4_B,LOW);
      Serial.println("STOP");
      break;
  }
}

void motor(){
  for(;;){
    if (irrecv.decode(&decodedSignal)){
      Serial.println(decodedSignal.value, HEX);
      motor_dir(decodedSignal.value);
      xSemaphoreTake(mutex, portMAX_DELAY);
      motor_speed(decodedSignal.value);
      xSemaphoreGive(mutex);
      irrecv.resume();
    }
  }
}

void distance_(){
  for(;;){
    digitalWrite(trg, HIGH);
    delayMicroseconds(10);
    digitalWrite(trg, LOW);

    duration = pulseIn(echo, HIGH);
    distance = ((float)(340 * duration)/1000)/2;
    //Serial.println(distance);
    
    xSemaphoreTake(mutex, portMAX_DELAY);
    int sp = speedDC;
    xSemaphoreGive(mutex);

    switch(sp){
      case 127:
        if (distance < 6){
          digitalWrite(IN1_A,LOW);
          digitalWrite(IN2_A,LOW);
          digitalWrite(IN3_B,LOW);
          digitalWrite(IN4_B,LOW);
          Serial.println("STOP_A");
        }
        break;
        
      case 179:
        if (distance < 9){
          digitalWrite(IN1_A,LOW);
          digitalWrite(IN2_A,LOW);
          digitalWrite(IN3_B,LOW);
          digitalWrite(IN4_B,LOW);
          Serial.println("STOP_B");
        }
        break;

      case 255:
        if (distance < 12){
          digitalWrite(IN1_A,LOW);
          digitalWrite(IN2_A,LOW);
          digitalWrite(IN3_B,LOW);
          digitalWrite(IN4_B,LOW);
          Serial.println("STOP_C");
        }
        break;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENABLE_A,OUTPUT);  
  pinMode(IN1_A,OUTPUT);  
  pinMode(IN2_A,OUTPUT);  
  pinMode(ENABLE_B,OUTPUT);  
  pinMode(IN3_B,OUTPUT);  
  pinMode(IN4_B,OUTPUT); 
  pinMode(trg, OUTPUT);
  pinMode(echo, INPUT);

  // IR
  irrecv.enableIRIn();

  mutex = xSemaphoreCreateMutex();

  xTaskCreate(motor, "motor", 128, NULL, 0, NULL);
  xTaskCreate(distance_, "distance_", 128, NULL, 0, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
