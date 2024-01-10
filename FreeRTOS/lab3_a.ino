#include <Arduino_FreeRTOS.h>
#include <semphr.h>

const int MAX_SIZE = 5;
int buffer[MAX_SIZE];
int* x = 0;
SemaphoreHandle_t task1 = NULL;

typedef struct Monitor{
  SemaphoreHandle_t mutex;
  SemaphoreHandle_t bufFullSem;
  SemaphoreHandle_t bufEmptySem;
  int buffer[MAX_SIZE];
  int index = 0;
}MyMonitor;

MyMonitor monitor;

void init(MyMonitor* monitor){
  monitor -> mutex = xSemaphoreCreateMutex();
  monitor -> bufFullSem = xSemaphoreCreateCounting(MAX_SIZE, MAX_SIZE);
  monitor -> bufEmptySem = xSemaphoreCreateCounting(MAX_SIZE, 0);
}

void read(MyMonitor* monitor, int x){
  xSemaphoreTake(monitor -> bufEmptySem, portMAX_DELAY);
  xSemaphoreTake(monitor -> mutex, portMAX_DELAY);
  x = monitor -> buffer[--(monitor -> index)];
  Serial.print("Read: ");
  Serial.println(x);
  xSemaphoreGive(monitor -> mutex);
  xSemaphoreGive(monitor -> bufFullSem);
}

void add(MyMonitor* monitor, int x){
  xSemaphoreTake(monitor -> bufFullSem, portMAX_DELAY);
  xSemaphoreTake(monitor -> mutex, portMAX_DELAY);
  monitor -> buffer[monitor -> index++] = x;
  Serial.print("Add:  ");
  Serial.println(monitor->buffer[monitor->index-1]);
  xSemaphoreGive(monitor -> mutex);
  xSemaphoreGive(monitor -> bufEmptySem);
}

void task(void *t){
  int i = 0;
  while(1){
    xSemaphoreTake(task1, portMAX_DELAY);
   
    add(&monitor, i);
    read(&monitor, i);
    i = i+1;
    
    xSemaphoreGive(task1);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init(&monitor);
  
  task1 = xSemaphoreCreateMutex();
  xTaskCreate(task, "task", 128, NULL, 0, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
