
//#include <SoftwareSerial.h>

//Center RGB LED, PWM capable
#define INTLEDR 12  // red channel
#define INTLEDG 13  // green channel

//White bar-graph LEDs
#define INTLED0 15  // Left most
#define INTLED1 16  //
#define INTLED2 17  //
#define INTLED3 18  //
#define INTLED4 19  //
#define INTLED5 20  // Right most

//SoftwareSerial xBee(10, 11);  //RX, TX

void setup() {
  // put your setup code here, to run once:
  //xBee.begin(9600);
  //xBee.println("Hello, Jooeun?");

  pinMode(INTLEDR, OUTPUT);
  pinMode(INTLEDG, OUTPUT);
  
  pinMode(INTLED0, OUTPUT);
  pinMode(INTLED1, OUTPUT);
  pinMode(INTLED2, OUTPUT);
  pinMode(INTLED3, OUTPUT);
  pinMode(INTLED4, OUTPUT);
  pinMode(INTLED5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  active();
  success(3);
  fail(2);
}

void success( int blinks ){
  for (int i=1; i<=blinks; i++){
    digitalWrite(INTLEDG, HIGH);
    delay(200);
    digitalWrite(INTLEDG, LOW);
    delay(200);
  }
  delay(5000);
}

void fail( int blinks ){
  for (int i=1; i<=blinks; i++){
    digitalWrite(INTLEDR, HIGH);
    delay(200);
    digitalWrite(INTLEDR, LOW);
    delay(200);
  }
  delay(5000);
}

void active( void ){
  digitalWrite(INTLED0, HIGH);
  digitalWrite(INTLED1, HIGH);
  digitalWrite(INTLED2, HIGH);
  digitalWrite(INTLED3, HIGH);
  digitalWrite(INTLED4, HIGH);
  digitalWrite(INTLED5, HIGH);
}


