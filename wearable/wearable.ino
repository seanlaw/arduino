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

void blink(int n){
  for (int i=0; i<n; i++){
    digitalWrite(INTLEDG, HIGH);
    delay(200);
    digitalWrite(INTLEDG, LOW);
    delay(200);
  }

}

void loop(){
  checkWaves();
  //Serial.write(luxReading);
  if (Serial.available())
  { // If data comes in from serial monitor, send it out to XBee
    XBee.write(Serial.read());
  }
  if (XBee.available())
  { // If data comes in from XBee, send it out to serial monitor
    Serial.write(XBee.read());

    
    blink(3);
  }
}

void checkWaves(){
  float luxReading = analogRead(LUXPIN);
  if (luxReading < baseLuxReading/4.0){
    waveCount++;
    delay(500);
  }
  if (waveCount >= 2){
    blink(3);
    delay(500);
    waveCount=0;
  }
}

