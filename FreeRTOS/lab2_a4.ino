
#include <Arduino_FreeRTOS.h>

const int led1 = 13;
const int led2 = 12;

void task1(){
  while(1){
    digitalWrite(led1, HIGH);
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    digitalWrite(led1, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void task2(){
  while(1){
    digitalWrite(led2, HIGH);
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    digitalWrite(led2, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  xTaskCreate(task1, "Task1", 128, NULL, 0, NULL);
  xTaskCreate(task2, "Task2", 128, NULL, 0, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
