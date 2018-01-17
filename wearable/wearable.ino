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
  if (XBee.available()){
    while(XBee.available()){
      char c = XBee.read();
    }
    bounceLEDs(4, 50);
    blink(4);
    setAllLEDs(HIGH);
  }
  checkWaves(0.5, true);
}

void blink(int n){
  for (int i=0; i<n; i++){
    digitalWrite(INTLEDG, HIGH);
    setAllLEDs(HIGH);
    delay(200);
    digitalWrite(INTLEDG, LOW);
    setAllLEDs(LOW);
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
    XBee.write("T");
    bounceLEDs(4, 50);
    setAllLEDs(HIGH);
    delay(1000);
    if (reset == true){
      waveCount=0;
    }
    else{
      waveCount++;
    }
  }
}

void bounceLEDs(int n, int wait){
  int arrayLEDs[6];
  for (int i=0; i< n; i++){
    for (int j=5; j>=0; j--){
      for(int k=0; k< 6; k++){
        arrayLEDs[k] = LOW;
      }
      arrayLEDs[j] = HIGH;
      digitalWrite(INTLED0, arrayLEDs[0]);
      digitalWrite(INTLED1, arrayLEDs[1]);
      digitalWrite(INTLED2, arrayLEDs[2]);
      digitalWrite(INTLED3, arrayLEDs[3]);
      digitalWrite(INTLED4, arrayLEDs[4]);
      digitalWrite(INTLED5, arrayLEDs[5]);
      delay(wait);
    }
    for (int j=0; j< 6; j++){
      for(int k=0; k< 6; k++){
        arrayLEDs[k] = LOW;
      }
      arrayLEDs[j] = HIGH;
      digitalWrite(INTLED0, arrayLEDs[0]);
      digitalWrite(INTLED1, arrayLEDs[1]);
      digitalWrite(INTLED2, arrayLEDs[2]);
      digitalWrite(INTLED3, arrayLEDs[3]);
      digitalWrite(INTLED4, arrayLEDs[4]);
      digitalWrite(INTLED5, arrayLEDs[5]);
      delay(wait);
    }

  }
}

void setAllLEDs(int value){
  digitalWrite(INTLED0, value);
  digitalWrite(INTLED1, value);
  digitalWrite(INTLED2, value);
  digitalWrite(INTLED3, value);
  digitalWrite(INTLED4, value);
  digitalWrite(INTLED5, value);
}
