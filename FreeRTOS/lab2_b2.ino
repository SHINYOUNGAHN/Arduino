
#include <Arduino_FreeRTOS.h>
#include <queue.h>

const int led1 = 11;
const int led2 = 10;
float input = -1;
QueueHandle_t qu1;
QueueHandle_t qu2;

void task1(void *s1){
  Serial.println("Task1");
  float msg;
  BaseType_t xStatus;
    
  while(1){
    xStatus = xQueueReceive(qu1, &msg, pdMS_TO_TICKS(5000));
    if (xStatus == pdPASS){
      analogWrite(led1, (msg-1)*255);
      Serial.println(msg);
    }
    else{
      Serial.println("Could not receive from the queue1.");
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void task2(void *s2){
  Serial.println("Task2");
  float msg;
  BaseType_t xStatus;
    
  while(1){
    xStatus = xQueueReceive(qu2, &msg, pdMS_TO_TICKS(5000));
    if (xStatus == pdPASS){
      analogWrite(led2, (msg-2)*255);
      Serial.println(msg);
    }
    else{
      Serial.println("Could not receive from the queue2.");
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void task3(void *s3){
  Serial.println("Task3!");
  BaseType_t xStatus;
  
  while(1){
    while(Serial.available()){
      input = Serial.parseFloat();
      if (input>=1 && input<2){
        if (uxQueueMessagesWaiting(qu1) >= 5){
          Serial.println("Queue1 should have been empty!");
          break;
        }
        else {
          xStatus = xQueueSendToBack(qu1, &input, 0);
        }
      }
      
      else if (input>=2 && input<3){
        if (uxQueueMessagesWaiting(qu2) >= 5){
          Serial.println("Queue2 should have been empty!");
        }
        else {
          xStatus = xQueueSendToBack(qu2, &input, 0);
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  qu1 = xQueueCreate(5, sizeof(float));
  qu2 = xQueueCreate(5, sizeof(float));
  if (qu1 != NULL && qu2 != NULL) {
    xTaskCreate(task1, "Task1", 128, NULL, 2, NULL);
    xTaskCreate(task2, "Task2", 128, NULL, 1, NULL);
    xTaskCreate(task3, "Task3", 128, NULL, 0, NULL);
  } 
}

void loop() {
  // put your main code here, to run repeatedly:
}
