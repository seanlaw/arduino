/*****************************************************************
XBee_Serial_Passthrough.ino

Set up a software serial port to pass data between an XBee Shield
and the serial monitor.

Hardware Hookup:
  The XBee Shield makes all of the connections you'll need
  between Arduino and XBee. If you have the shield make
  sure the SWITCH IS IN THE "DLINE" POSITION. That will connect
  the XBee's DOUT and DIN pins to Arduino pins 2 and 3.

*****************************************************************/

#define INTLEDR 12  // red channel
#define INTLEDG 13  // green channel
#define INTLEDB 14  // blue channel

#define INTLED0 15  // Left most
#define INTLED1 16  //
#define INTLED2 17  //
#define INTLED3 18  //
#define INTLED4 19  //
#define INTLED5 20  // Right most

// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
// XBee's DOUT (TX) is connected to pin 10 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 11 (Arduino's Software TX)
SoftwareSerial XBee(10, 11); // RX, TX

void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(9600);
  
  pinMode(INTLEDR, OUTPUT);
  pinMode(INTLEDG, OUTPUT);
  
  pinMode(INTLED0, OUTPUT);
  pinMode(INTLED1, OUTPUT);
  pinMode(INTLED2, OUTPUT);
  pinMode(INTLED3, OUTPUT);
  pinMode(INTLED4, OUTPUT);
  pinMode(INTLED5, OUTPUT);
}

void loop()
{
  digitalWrite(INTLED0, HIGH);
  digitalWrite(INTLED1, HIGH);
  digitalWrite(INTLED2, HIGH);
  digitalWrite(INTLED3, HIGH);
  digitalWrite(INTLED4, HIGH);
  digitalWrite(INTLED5, HIGH);

  if (Serial.available())
  { // If data comes in from serial monitor, send it out to XBee
    //Serial.write("HERE");
    digitalWrite(INTLEDR, HIGH);
    XBee.write(Serial.read());
  }
  if (XBee.available())
  { // If data comes in from XBee, send it out to serial monitor
    digitalWrite(INTLEDR, HIGH);
    Serial.write(XBee.read());
  }
}
