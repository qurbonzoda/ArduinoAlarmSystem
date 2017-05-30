int alarmDistance = 20;  // in cm
 
int trigPin = 11;
int echoPin = 12;
long duration, cm;

int buzzerPin = 13;


int Uo_Pin = 3;
int A0_Pin = 10;
int RW_Pin = 9;
int E_Pin = 8;

int DB4_Pin = 4;
int DB5_Pin = 5;
int DB6_Pin = 6;
int DB7_Pin = 7;

void pulseEnable() {
  digitalWrite(E_Pin, LOW);
  delayMicroseconds(1);    
  digitalWrite(E_Pin, HIGH);
  delayMicroseconds(1); 
  digitalWrite(E_Pin, LOW);
  delayMicroseconds(2000);
}


void write4Bits(int DB7, int DB6, int DB5, int DB4) {
  digitalWrite(DB7_Pin, DB7);
  digitalWrite(DB6_Pin, DB6);
  digitalWrite(DB5_Pin, DB5);
  digitalWrite(DB4_Pin, DB4);

  pulseEnable();
}

void command(int DB7, int DB6, int DB5, int DB4) {
  digitalWrite(A0_Pin, LOW);
  digitalWrite(RW_Pin, LOW);

  write4Bits(DB7, DB6, DB5, DB4);
}


void setupBitCapacity() {
  for (int i = 0; i < 3; ++i) {
    command(0, 0, 1, 1);
    delayMicroseconds(4500);
  }
  command(0, 0, 1, 0);
  delayMicroseconds(4500);
}

 
void setup() {
  delayMicroseconds(50000); 

  Serial.begin (9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzerPin, OUTPUT);
  
  pinMode(A0_Pin, OUTPUT);
  pinMode(RW_Pin, OUTPUT);
  pinMode(E_Pin, OUTPUT);
  pinMode(DB4_Pin, OUTPUT);
  pinMode(DB5_Pin, OUTPUT);
  pinMode(DB6_Pin, OUTPUT);
  pinMode(DB7_Pin, OUTPUT);

  
  delay(50);
  
  setupBitCapacity();
  

  // setup parameters
  command(0, 0, 1, 0);
  command(1, 0, 0, 0);
  
  // turn on display
  command(0, 0, 0, 0);
  command(1, 1, 0, 0);

  // clear display
  command(0, 0, 0, 0);
  command(0, 0, 0, 1);
  
  // set output mode
  command(0, 0, 0, 0);
  command(0, 1, 1, 0);
}

void send(uint8_t value) {
  Serial.println((char)value);
  
  digitalWrite(A0_Pin, HIGH);
  digitalWrite(RW_Pin, LOW);

  int bigBits = value>>4;
  int smallBits = value & 0xf;

  write4Bits((bigBits & 8) != 0,   (bigBits & 4) != 0,   (bigBits & 2) != 0,   (bigBits & 1) != 0);
  write4Bits((smallBits & 8) != 0, (smallBits & 4) != 0, (smallBits & 2) != 0, (smallBits & 1) != 0);
}

void print(const char *s) {
  Serial.println(s);
  // clear display
  command(0, 0, 0, 0);
  command(0, 0, 0, 1);
  
  for (int i = 0; i < strlen(s); i++) {
    send((uint8_t)s[i]);
  }
}
 
void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
 
  cm = (duration/2) / 29.1;

  if (cm <= alarmDistance) {
    tone(buzzerPin, 40);
    print("Danger");
  } else {
    noTone(buzzerPin);
    print("Safe");
  }

  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(200);
}
