#include <SoftwareSerial.h>
#include "PinMapping.h"

// XBee's DOUT (TX) is connected to pin 10 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 11 (Arduino's Software TX)
SoftwareSerial XBee(RX, TX); // RX, TX

int intLEDs[] = {INTLED0, INTLED1, INTLED2, INTLED3, INTLED4, INTLED5};
int intLEDSize = sizeof( intLEDs ) / sizeof( int );

struct SIGNAL {
  bool t;
  bool d;
  bool a;
  unsigned long lastTime;
};

SIGNAL signal = {false, false, false, 0};

void setup(){
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(9600);

  pinMode(INTLEDG, OUTPUT);

  for (int i=0; i< intLEDSize; i++){
    pinMode(intLEDs[i], OUTPUT);
    digitalWrite(intLEDs[i], HIGH);
  }
  pinMode(BTNPIN, INPUT_PULLUP);
  
}

void loop(){
  if (signalReceived() || buttonPressed()){
    bounceBlink();
  }
}

void blink(int ledPin, int n, int sleep){
  for (int i=0; i<n; i++){
    digitalWrite(ledPin, HIGH);
    setAllLEDs(HIGH);
    delay(sleep);
    digitalWrite(ledPin, LOW);
    setAllLEDs(LOW);
    delay(sleep);
  }
}

void bounceBlink(){
  bounceLEDs(4, 50);
  blink(INTLEDG, 4, 200);
  setAllLEDs(HIGH);
}

void bounceLEDs(int n, int wait){
  for (int i=0; i< n; i++){
    // Bounce right to left
    for (int j=intLEDSize-1; j>=0; j--){
      setAllLEDs(LOW);
      digitalWrite(intLEDs[j], HIGH);
      delay(wait);
    }
    // Bounce left to right
    for (int j=0; j< intLEDSize; j++){
      setAllLEDs(LOW);
      digitalWrite(intLEDs[j], HIGH);
      delay(wait);
    }
  }
}

void setAllLEDs(int value){
  for (int i=0; i< intLEDSize; i++){
    digitalWrite(intLEDs[i], value);
  }
}

bool buttonPressed(){
  // Return true is button is pressed
  // and sends signal through XBee
  if ( digitalRead( BTNPIN ) == 0 ){
    XBee.write('T');
    XBee.write('D');
    XBee.write('A');
    return true;
  }
  
  return false;
}

bool signalReceived(){
  if (XBee.available()){
    char c = XBee.read();
    switch (c){
      case 'T':
        signal.t = true;
        signal.lastTime = millis();
        break;
      case 'D':
        signal.d = true;
        signal.lastTime = millis();
        break;
      case 'A':
        signal.a = true;
        signal.lastTime = millis();
        break;   
    }
  }

  // Ensure that "TDA" signal is received 
  if (signal.t && signal.d && signal.a){
    signal = {false, false, false, 0};
    return true;
  }

  // Ensure that "TDA" signal is received in one second
  // Otherwise, reset signal
  if (signal.t || signal.d || signal.a){
    if ((millis() - signal.lastTime) > 1000){
      signal = {false, false, false, 0};
    }
  }
  
  return false;
}

