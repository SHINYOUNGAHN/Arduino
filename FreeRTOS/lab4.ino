
#include <Arduino_FreeRTOS.h>
#include <timers.h>

int intr1Pin = 2;//, led1 = 9, led2= 10, led3 = 11, led4 = 12;
int ledArr[4] = {9, 10, 11, 12};
//volatile int toggel1 = 0;
TimerHandle_t pattern1Handle, pattern2Handle;
int arr = 0;

void pattern1(TimerHandle_t timerHandle){
  static int i = 0;
  i++;
  if(i > 10){
    xTimerStart(pattern2Handle, 0);
    i = 0;
  }
  //static bool togel = 0;
  if (arr>4){
    digitalWrite(ledArr[arr], HIGH);
    digitalWrite(ledArr[0], LOW);
    arr=0;
  }
  else {
    digitalWrite(ledArr[arr], HIGH);
    digitalWrite(ledArr[arr-1], LOW);
    arr = arr+1;
  }
}

void pattern2(TimerHandle_t timerHandle){
  static bool togel = 0;
  if (togel == 0){
    digitalWrite(ledArr[0], HIGH);
    digitalWrite(ledArr[1], LOW);
    digitalWrite(ledArr[2], HIGH);
    digitalWrite(ledArr[3], LOW);

    togel = 1;
  }
  else{
    digitalWrite(ledArr[0], LOW);
    digitalWrite(ledArr[1], HIGH);
    digitalWrite(ledArr[2], LOW);
    digitalWrite(ledArr[3], HIGH);
    togel = 0;
  }
}


void ISR1(void){
  static bool togel = 0;
  if (togel == 0){
    xTimerStop(pattern1Handle, 0);
    xTimerStart(pattern2Handle, 0);
    togel = 1;
  }
  else {
    xTimerStop(pattern2Handle, 0);
    xTimerStart(pattern1Handle, 0);
    togel = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i=0; i<4; i++){
    pinMode(ledArr[i], OUTPUT);
  }
  pinMode(intr1Pin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(intr1Pin), ISR1, RISING);
  
  pattern1Handle = xTimerCreate("pattern1", pdMS_TO_TICKS(500), pdTRUE, NULL, pattern1);
  pattern2Handle = xTimerCreate("pattern2", pdMS_TO_TICKS(500), pdTRUE, NULL, pattern2);

  if (pattern1Handle != NULL){
    xTimerStart(pattern1Handle, 0);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
