#include <SoftwareSerial.h>
#include "PinMapping.h"

struct WaveState {
  unsigned long firstWaveTime;
  unsigned long firstPauseTime;
};

// XBee's DOUT (TX) is connected to pin 10 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 11 (Arduino's Software TX)
SoftwareSerial XBee(RX, TX); // RX, TX

float baseLuxReading;
int waveCount;
int intLEDs[] = {INTLED0, INTLED1, INTLED2, INTLED3, INTLED4, INTLED5};
int intLEDSize = sizeof( intLEDs ) / sizeof( int );
WaveState waveState = {0, 0};

void setup(){
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(9600);

  pinMode(INTLEDG, OUTPUT);
  pinMode(INTLEDR, OUTPUT);

  for (int i=0; i< intLEDSize; i++){
    pinMode(intLEDs[i], OUTPUT);
    digitalWrite(intLEDs[i], HIGH);
  }
  pinMode(LUXPIN, INPUT);
  
  baseLuxReading = analogRead(LUXPIN);
}

void loop(){
  if (XBee.available()){
    while(XBee.available()){
      char c = XBee.read();
    }
    bounceBlink();
  }
  checkWaves(0.65, true);
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

void checkWaves(float lightPercentage, bool reset){
  float luxReading = analogRead(LUXPIN);
  unsigned long elapsedTime;
  
  if (luxReading <= lightPercentage*baseLuxReading){
    drawBarGraph(luxReading);
    if (waveState.firstWaveTime == 0){
      waveState.firstWaveTime = millis();
    }
    if (waveState.firstPauseTime > 0){
      waveState.firstWaveTime = millis();
      elapsedTime = waveState.firstWaveTime - waveState.firstPauseTime;
      if (elapsedTime < 1000){
        XBee.write("T");
        bounceBlink();
      }
      waveState.firstWaveTime = 0;
      waveState.firstPauseTime = 0;
    }
  }
  if (waveState.firstWaveTime > 0 && waveState.firstPauseTime == 0 && luxReading >= 0.85*baseLuxReading){
    waveState.firstPauseTime = millis();
    setAllLEDs(HIGH);
  }
}

void bounceBlink(){
  bounceLEDs(4, 50);
  blink(INTLEDG, 4, 200);
  setAllLEDs(HIGH);
  delay(1000);
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

void drawBarGraph( int inputVar ){
  int increment = (int(baseLuxReading) / 7);
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
