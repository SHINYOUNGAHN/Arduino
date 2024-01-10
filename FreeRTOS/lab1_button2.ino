int button = 2;
int led = 13;

int val = 0;
int old_val = 0;

int state = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(button);
  
  if ((val==LOW) && (old_val==HIGH)){
    state = state + 1;
    delay(100);
    if (state>5) state = 1;
  }
  old_val = val;
  
  switch(state){
    case 1: 
      digitalWrite(led, LOW);
      delay(100);
      break;
    case 2:
      digitalWrite(led, 63);
      delay(100);
      break;
    case 3:
      digitalWrite(led, 127);
      delay(100);
      break;
    case 4:
      digitalWrite(led, 191);
      delay(100);
      break;
    case 5:
      digitalWrite(led, 255);
      delay(100);
      break;
    default:
      digitalWrite(led, LOW);
      delay(100);
      break;
  }
}
