#include <SoftwareSerial.h>
#include "PinMapping.h"
// XBee's DOUT (TX) is connected to pin 10 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 11 (Arduino's Software TX)
SoftwareSerial XBee(RX, TX); // RX, TX

float baseLuxReading;
int waveCount;

void setup(){
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(9600);
  pinMode(24, OUTPUT);
  pinMode(INTLEDG, OUTPUT);
  pinMode(INTLED0, OUTPUT);
  pinMode(INTLED1, OUTPUT);
  pinMode(INTLED2, OUTPUT);
  pinMode(INTLED3, OUTPUT);
  pinMode(INTLED4, OUTPUT);
  pinMode(INTLED5, OUTPUT);
  pinMode(LUXPIN, INPUT);

  digitalWrite(INTLED0, HIGH);
  digitalWrite(INTLED1, HIGH);
  digitalWrite(INTLED2, HIGH);
  digitalWrite(INTLED3, HIGH);
  digitalWrite(INTLED4, HIGH);
  digitalWrite(INTLED5, HIGH);
  
  baseLuxReading = analogRead(LUXPIN);
}

void loop(){
  checkWaves(0.5, true);
  /*
  if (Serial.available())
  { // If data comes in from serial monitor, send it out to XBee
    XBee.write(Serial.read());
  }
  if (XBee.available())
  { // If data comes in from XBee, send it out to serial monitor
    Serial.write(XBee.read());

    
    blink(3);
  }
  */
}

void blink(int n){
  for (int i=0; i<n; i++){
    digitalWrite(INTLEDG, HIGH);
    delay(200);
    digitalWrite(INTLEDG, LOW);
    delay(200);
  }
}

void checkWaves(float lightPercentage, bool reset){
  float luxReading = analogRead(LUXPIN);
  if (luxReading <= lightPercentage*baseLuxReading){
    waveCount++;
    delay(400);
  }
  if (waveCount == 2){
    cycleLEDs(3, 200);
    blink(4);
    delay(1000);
    if (reset == true){
      waveCount=0;
    }
    else{
      waveCount++;
    }
  }
}

void cycleLEDs(int n, int wait){
  for (int i=0; i< n; i++){
    digitalWrite(INTLED0, LOW);
    digitalWrite(INTLED1, LOW);
    digitalWrite(INTLED2, LOW);
    digitalWrite(INTLED3, LOW);
    digitalWrite(INTLED4, LOW);
    digitalWrite(INTLED5, LOW);
    delay(wait);
    digitalWrite(INTLED0, HIGH);
    digitalWrite(INTLED1, HIGH);
    digitalWrite(INTLED2, LOW);
    digitalWrite(INTLED3, LOW);
    digitalWrite(INTLED4, LOW);
    digitalWrite(INTLED5, LOW);
    delay(wait);
    digitalWrite(INTLED0, LOW);
    digitalWrite(INTLED1, LOW);
    digitalWrite(INTLED2, HIGH);
    digitalWrite(INTLED3, HIGH);
    digitalWrite(INTLED4, LOW);
    digitalWrite(INTLED5, LOW);
    delay(wait);
    digitalWrite(INTLED0, LOW);
    digitalWrite(INTLED1, LOW);
    digitalWrite(INTLED2, LOW);
    digitalWrite(INTLED3, LOW);
    digitalWrite(INTLED4, HIGH);
    digitalWrite(INTLED5, HIGH);
    delay(wait);
    digitalWrite(INTLED0, LOW);
    digitalWrite(INTLED1, LOW);
    digitalWrite(INTLED2, LOW);
    digitalWrite(INTLED3, LOW);
    digitalWrite(INTLED4, LOW);
    digitalWrite(INTLED5, LOW);
  }
}

void resetLEDs(){
  digitalWrite(INTLED0, HIGH);
  digitalWrite(INTLED1, HIGH);
  digitalWrite(INTLED2, HIGH);
  digitalWrite(INTLED3, HIGH);
  digitalWrite(INTLED4, HIGH);
  digitalWrite(INTLED5, HIGH);
}

void drawBarGraph( int inputVar ){
  //int increment = (200 / 7);
  int increment = (baseLuxReading / 7);
  if ( inputVar > (increment * 1) ) {
    digitalWrite(INTLED0, 1);
  } else {
    digitalWrite(INTLED0, 0);
  }
  if ( inputVar > (increment * 2) ) {
    digitalWrite(INTLED1, 1);
  } else {
    digitalWrite(INTLED1, 0);
  }
  if ( inputVar > (increment * 3) ) {
    digitalWrite(INTLED2, 1);
  } else {
    digitalWrite(INTLED2, 0);
  }
  if ( inputVar > (increment * 4) ) {
    digitalWrite(INTLED3, 1);
  } else {
    digitalWrite(INTLED3, 0);
  }
  if ( inputVar > (increment * 5) ) {
    digitalWrite(INTLED4, 1);
  } else {
    digitalWrite(INTLED4, 0);
  }
  if ( inputVar > (increment * 6) ) {
    digitalWrite(INTLED5, 1);
  } else {
    digitalWrite(INTLED5, 0);
  }
}
