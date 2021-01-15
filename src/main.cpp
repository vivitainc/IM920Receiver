#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IM920.h"

IM920 im920;
const size_t usb_baudrate = 9600;
const size_t im920_baudrate = 19200;

// シリアル通信に使うRX,TXピン
#define IM920_RX_PIN 4
#define IM920_TX_PIN 5

SoftwareSerial IM920Serial(IM920_RX_PIN, IM920_TX_PIN);

String receiveAsciis[] = {""};

void dataRecieve() {
  String stream = IM920Serial.readStringUntil('\n');
  // String receiveStrings[];
  String dataStrings;
  if (stream != ""){
    // Serial.println(stream); // 受信データそのまま

    int dataLength = stream.length() + 1;
    char dataChar[dataLength];

    stream.toCharArray(dataChar, dataLength);
    char *receiveDataStrings = strtok(dataChar, ": ");
    receiveDataStrings = strtok(NULL, ": ");
    dataStrings = receiveDataStrings;
    if (dataStrings != NULL) { 
      Serial.println(dataStrings);  // 受信データのデータ部分
    }
  }
}

void setup() {
  Serial.begin(usb_baudrate);
  IM920Serial.begin(im920_baudrate);
  im920.setup(IM920Serial);

  im920.writable(true);
  delay(500);
  im920.charIO(true);
  delay(500);

  Serial.println("complete im920 setup");
}
 
void loop() {
  dataRecieve();
  delay(50);
}