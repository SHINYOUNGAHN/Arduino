#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <time.h>
#include <stdlib.h>

const int MAX_SIZE = 10;
int buffer[MAX_SIZE];

SemaphoreHandle_t mutex1 = NULL;
SemaphoreHandle_t mutex2 = NULL;
SemaphoreHandle_t binarySem1 = NULL;
SemaphoreHandle_t binarySem2 = NULL;
TaskHandle_t prodThrd1, prodThrd2, consThrd1, consThrd2; 
volatile int *r1 = 0;
volatile int *r2 = 0;

typedef struct Monitor{
  SemaphoreHandle_t mutex;
  SemaphoreHandle_t bufFullSem;
  SemaphoreHandle_t bufEmptySem;
  int buffer[MAX_SIZE];
  int index = 0;
}MyMonitor;

MyMonitor m1, m2;

void init(MyMonitor* monitor){
  monitor -> mutex = xSemaphoreCreateMutex();
  monitor -> bufFullSem = xSemaphoreCreateCounting(MAX_SIZE, MAX_SIZE);
  monitor -> bufEmptySem = xSemaphoreCreateCounting(MAX_SIZE, 0);
}

void read(MyMonitor* monitor, int* x){
  xSemaphoreTake(monitor -> bufEmptySem, portMAX_DELAY);
  xSemaphoreTake(monitor -> mutex, portMAX_DELAY);
  *x = monitor -> buffer[--(monitor -> index)];
  Serial.println(*x);
  xSemaphoreGive(monitor -> mutex);
  xSemaphoreGive(monitor -> bufFullSem);
}

void add(MyMonitor* monitor, int x){
  xSemaphoreTake(monitor -> bufFullSem, portMAX_DELAY);
  xSemaphoreTake(monitor -> mutex, portMAX_DELAY);
  monitor -> buffer[monitor -> index++] = x;
  Serial.println(monitor->buffer[monitor->index-1]);
  xSemaphoreGive(monitor -> mutex);
  xSemaphoreGive(monitor -> bufEmptySem);
}

void producer1(void *arg){
  for(;;){
    r1 = rand();
    Serial.print("Producer1:  ");
    add(&m1, r1);

    vTaskDelay(pdMS_TO_TICKS(500));
  }
  vTaskDelete(NULL);
}

void producer2(void *arg){
  for(;;){
    r2 = rand();
    Serial.print("Producer2:  ");
    add(&m2, r2);

    vTaskDelay(pdMS_TO_TICKS(500));
  }
  vTaskDelete(NULL);
}

void consumer1(void *arg){
  for(;;){
    Serial.print("Consumer1:  ");
    read(&m1, *r1);

    vTaskDelay(pdMS_TO_TICKS(500));
    
  }
  vTaskDelete(NULL);
}

void consumer2(void *arg){
  for(;;){
    Serial.print("Comsumer2: ");
    read(&m2, *r2);  
    
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  vTaskDelete(NULL);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  srand(time(NULL));
  init(&m1);
  init(&m2);
  
  xTaskCreate(producer1, "producer1", 100, NULL, 0, NULL);
  xTaskCreate(producer2, "producer2", 100, NULL, 0, NULL);
  xTaskCreate(consumer1, "consumer1", 100, NULL, 0, NULL);
  xTaskCreate(consumer2, "consumer2", 100, NULL, 0, NULL); 

}

void loop() {
  // put your main code here, to run repeatedly:
}
