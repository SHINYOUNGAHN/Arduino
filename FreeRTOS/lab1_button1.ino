int button2 = 2;
int button3 = 3;
int button4 = 4;
int led13 = 13;
int led12 = 12;
int led11 = 11;


int val = 0;
int val3 = 0;
int val4 = 0;

int old_val = 0;
int old_val3 = 0;
int old_val4 = 0;
int state = 0;
int state3 = 0;
int state4 = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(led13, OUTPUT);
  pinMode(led12, OUTPUT);
  pinMode(led11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  int state2 = digitalRead(button2);
//  int state3 = digitalRead(button3);
//  int state4 = digitalRead(button4);

  val = digitalRead(button2);
  val3 = digitalRead(button3);
  val4 = digitalRead(button4);
  
  if ((val==HIGH) && (old_val==LOW)){
    state = 1-state;
    delay(100);
  }
  old_val = val;
  if(state == 1){
    digitalWrite(led11, LOW);
  }
  else{
    digitalWrite(led11, HIGH);
  }

  // button3
   val3 = digitalRead(button3);
  
  if ((val3==HIGH) && (old_val3==LOW)){
    state3 = 1-state3;
    delay(100);
  }
  old_val3 = val3;
  if(state3 == 1){
    digitalWrite(led12, LOW);
  }
  else{
    digitalWrite(led12, HIGH);
  }

    // button4
    val4 = digitalRead(button4);
  
  if ((val4==HIGH) && (old_val4==LOW)){
    state4 = 1-state4;
    delay(100);
  }
  old_val4 = val4;
  if(state4 == 1){
    digitalWrite(led13, LOW);
  }
  else{
    digitalWrite(led13, HIGH);
  }
}
