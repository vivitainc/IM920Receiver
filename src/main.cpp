#include <Arduino.h>
#include <SoftwareSerial.h>
#include "IM920.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "ita24"; //"Extender-2.4";        //  *** 書き換え必要 ***
const char *pass = "3331itashin";    //  *** 書き換え必要（8文字以上）***

IM920 im920;
const size_t usb_baudrate = 9600;
const size_t im920_baudrate = 19200;

// シリアル通信に使うRX,TXピン
#define IM920_RX_PIN 4
#define IM920_TX_PIN 5

SoftwareSerial IM920Serial(IM920_RX_PIN, IM920_TX_PIN);

String imageBase64;
int notReceiveCount = 0;

void dataRecieve() {
  String stream = IM920Serial.readStringUntil('\n');
  // String receiveStrings[];
  String dataStrings;
  if (stream != ""){
    notReceiveCount = 0;
    // Serial.println(stream); // 受信データそのまま

    int dataLength = stream.length() + 1;
    char dataChar[dataLength];

    stream.toCharArray(dataChar, dataLength);
    char *receiveDataStrings = strtok(dataChar, ": ");
    receiveDataStrings = strtok(NULL, ": ");
    dataStrings = receiveDataStrings;
    if (dataStrings != NULL) { 
      // Serial.println(dataStrings.indexOf("aaa"));
      Serial.println(dataStrings);  // 受信データのデータ部分
      imageBase64 += dataStrings;
      Serial.print('R');
    }
  }else{
    notReceiveCount += 1;
    if(notReceiveCount >= 10){
      Serial.println(imageBase64.length());
      notReceiveCount = 0;

      if(imageBase64 != ""){
        Serial.println("send to server!");
        HTTPClient http;
        // http.begin("http://192.168.11.11:5001/viviware-dev2/asia-northeast1/fileupload");
        http.begin("http://asia-northeast1-viviware-dev2.cloudfunctions.net/fileupload");
        http.addHeader("Content-Type", "image/jpeg");
        int httpResponseCode = http.POST(imageBase64);
        Serial.println(httpResponseCode);
      }
      
      imageBase64 = "";
    }
    Serial.print('.');
  }
}

void setup() {
  Serial.begin(usb_baudrate);
  IM920Serial.begin(im920_baudrate);
  im920.setup(IM920Serial);

  // uint16_t cameraDeviceId = 76E4;

  im920.writable(true);
  delay(500);
  im920.charIO(true);
  delay(500);
  // im920.recvID(cameraDeviceId);
  // delay(500);

  Serial.println("complete im920 setup");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);             //  接続確立まで待つこと
  Serial.println("Connecting...");
  Serial.println(WiFi.status());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Connected");
  Serial.println(WiFi.localIP());     //  ESP 自身の IP アドレスをログ出力
  delay(5000);
}
 
void loop() {
  dataRecieve();
  delay(50);
}
