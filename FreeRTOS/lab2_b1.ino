
#include <Arduino_FreeRTOS.h>

const int led1 = 11;
const int led2 = 10;
float input = -1;

//add mutex;

void task1(void *s1){
  Serial.println("Task1");
  while(1){
    if (input>=1 && input<2){
      //Serial.println(input);
      analogWrite(led1, (input-1)*255);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void task2(void *s2){
  Serial.println("Task2");
  while(1){
    if(input>=2 && input<3){
      //Serial.println(input);
      analogWrite(led2, (input-2)*255);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }  
}

void task3(void *s3){
  Serial.println("Task3!");

  while(1){
    while(Serial.available()){
      input = Serial.parseFloat();
      //Serial.println(input);
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  xTaskCreate(task1, "Task1", 128, NULL, 2, NULL);
  xTaskCreate(task2, "Task2", 128, NULL, 1, NULL);
  xTaskCreate(task3, "Task3", 128, NULL, 0, NULL);
 
}

void loop() {
  // put your main code here, to run repeatedly:
}
