 /*
  Created by Igor Jarc
 See http://iot-playground.com for details
 Please use community fourum on website do not contact author directly

 Code based on https://github.com/DennisSc/easyIoT-ESPduino/blob/master/sketches/ds18b20.ino

 External libraries:
 - https://github.com/adamvr/arduino-base64
 - https://github.com/milesburton/Arduino-Temperature-Control-Library

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
#include <ESP8266WiFi.h>
#include <Base64.h>
//#include <OneWire.h>
#include <DallasTemperature.h>

//AP definitions
//#define AP_SSID "5thElement"
//#define AP_PASSWORD "xxx"
#include "AP.h"

// EasyIoT server definitions
// #define EIOT_USERNAME    "admin"
// #define EIOT_PASSWORD    "test"
#define EIOT_IP_ADDRESS  "ping-store.herokuapp.com" //"192.168.2.102" // "192.168.1.5"
#define EIOT_PORT        80 // 5000 // 80
#define EIOT_NODE        "N13S0"

#define REPORT_INTERVAL 1 // 60 // in sec


#define ONE_WIRE_BUS 2  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


#define USER_PWD_LEN 40
char unameenc[USER_PWD_LEN];
float oldTemp;


void setup() {
  Serial.begin(115200);

 /*
  wifiConnect();

  char uname[USER_PWD_LEN];
  String str = String(EIOT_USERNAME)+":"+String(EIOT_PASSWORD);
  str.toCharArray(uname, USER_PWD_LEN);
  memset(unameenc,0,sizeof(unameenc));
  base64_encode(unameenc, uname, strlen(uname));
  */
  oldTemp = -1;
}

void loop() {
  float temp;

  do {
    DS18B20.requestTemperatures();
    temp = DS18B20.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.println(temp);
  } while (temp == 85.0 || temp == (-127.0));

  if (temp != oldTemp)
  {
    sendTeperature(temp);
    oldTemp = temp;
  }

  int cnt = REPORT_INTERVAL;

  while(cnt--)
    delay(1000);
}

void wifiConnect()
{
    Serial.print("is connected: ");
    Serial.println(WiFi.isConnected());
    Serial.print("Connecting to AP");
    Serial.println(AP_SSID);
    Serial.println(AP_PASSWORD);
    WiFi.begin(AP_SSID, AP_PASSWORD);
    int status;
    while ((status = WiFi.status()) != WL_CONNECTED) {
      delay(50);
      Serial.print("[");
      Serial.print(status);
      Serial.print("]");
    Serial.print("is connected: ");
    Serial.println(WiFi.isConnected());
    }

  Serial.println("");
  Serial.println("WiFi connected");
    Serial.print("is connected: ");
    Serial.println(WiFi.isConnected());
}

void sendTeperature(float temp)
{

   WiFiClient client;

   wifiConnect();   // dodatkowe!!!


   while(!client.connect(EIOT_IP_ADDRESS, EIOT_PORT)) {
    Serial.println("connection failed");
    wifiConnect();
  }

  String url = "";
  url += "/pings-post?time=now&origin=ESP&target=fake&success=true&rtt=" + String(temp);

  Serial.print("POST data to URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + String(EIOT_IP_ADDRESS) + "\r\n" +
               "Connection: close\r\n" +
  //             "Authorization: Basic " + unameenc + " \r\n" +
               "Content-Length: 0\r\n" +
               "\r\n");


  delay(100);
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("Connection closed");

    Serial.print("is connected: ");
    Serial.println(WiFi.isConnected());
   WiFi.disconnect();
    Serial.print("is connected: ");
    Serial.println(WiFi.isConnected());

}
