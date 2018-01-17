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
  //checkWaves(0.6, true);
  checkWaves2(0.6, true);
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
    bounceBlink();
    if (reset == true){
      waveCount=0;
    }
    else{
      waveCount++;
    }
  }
}

void checkWaves2(float lightPercentage, bool reset){
  float luxReading = analogRead(LUXPIN);
  unsigned long elapsedTime;
  
  if (luxReading <= lightPercentage*baseLuxReading){
    if (waveState.firstWaveTime == 0){
      waveState.firstWaveTime = millis();
    }
    if (waveState.firstPauseTime > 0){
      waveState.firstWaveTime = millis();
      elapsedTime = waveState.firstWaveTime - waveState.firstPauseTime;
      if (elapsedTime < 2000){
        XBee.write("T");
        bounceBlink();
      }
      waveState.firstWaveTime = 0;
      waveState.firstPauseTime = 0;
    }
  }
  if (waveState.firstWaveTime > 0 && waveState.firstPauseTime == 0 && luxReading >= 0.85*baseLuxReading){
    waveState.firstPauseTime = millis();  
  }
}

void bounceBlink(){
  bounceLEDs(4, 50);
  blink(4);
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
