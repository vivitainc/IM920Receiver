#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IM920.h"

IM920 im920;
const size_t usb_baudrate = 9600;
const size_t im920_baudrate = 19200;

// シリアル通信に使うRX,TXピン
#define IM920_RX_PIN 6
#define IM920_TX_PIN 5

SoftwareSerial IM920Serial(IM920_RX_PIN, IM920_TX_PIN);

void setup() {
  Serial.begin(usb_baudrate);
  IM920Serial.begin(im920_baudrate);
  im920.setup(IM920Serial);

  // im920.setup(Serial);
  im920.writable(true);
  im920.clearSettings();
  im920.charIO(true);
  im920.writable(false);

  Serial.println("complete im920 setup");
}
 
void loop() {
  im920.update();
  if (im920.available()) {
    // get remote device info
    // Serial.print(im920.remoteNode(), HEX); Serial.print(",");
    // Serial.print(im920.remoteUID(),  HEX); Serial.print(",");
    // Serial.print(im920.remoteRSSI(), HEX); Serial.print(":");

    // get & handle received data
    for (size_t i = 0; i < im920.size(); ++i) {
        Serial.print(im920.data(i), HEX);
    }
    //DEDD0001DADA00020003
    //test1data23
    //DEDDDADA
    //testdata
    Serial.println("");

    // go to next data
    im920.pop();
  }

  delay(1000);
}
