#include <ESP8266WiFi.h>
#include <OneWire.h>

const short int BUILTIN_LED1 = 2; //GPIO2
const short int BUILTIN_LED2 = 16;//GPIO16

WiFiServer server(80); //Initialize the server on Port 80

int cnt = 0;

void blink() {
pinMode(BUILTIN_LED1, OUTPUT); // Initialize the BUILTIN_LED1 pin as an output 
pinMode(BUILTIN_LED2, OUTPUT); // Initialize the BUILTIN_LED2 pin as an output

for(int i=1; i<10; i++) {
  digitalWrite(BUILTIN_LED1, LOW); // Turn the LED ON by making the voltage LOW digitalWrite(BUILTIN_LED2, HIGH); // Turn the LED off by making the voltage HIGH
  digitalWrite(BUILTIN_LED2, HIGH); // Turn the LED ON by making the voltage LOW 
  delay(30); // Wait for a second 
  digitalWrite(BUILTIN_LED1, HIGH); // Turn the LED off by making the voltage HIGH 
  digitalWrite(BUILTIN_LED2, LOW); // Turn the LED ON by making the voltage LOW 
  delay(30); // Wait for two seconds
}
}

void setup1() {
blink();

WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
WiFi.softAP("Hello_IoT", "12345678"); // Provide the (SSID, password); . 
server.begin(); // Start the HTTP Server

Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server 
Serial.print("Server IP is: "); // Print the IP to the monitor window 
Serial.println(HTTPS_ServerIP);

}

void loop1() { 
  
WiFiClient client = server.available();
if (!client) { 
return; 
} 
//Looking under the hood 
Serial.println("Somebody has connected :)");


//Read what the browser has sent into a String class and print the request to the monitor
String request = client.readStringUntil('\r');
//Looking under the hood 
Serial.println(request);  

// Handle the Request

if (request.indexOf("/OFF") != -1){ 
digitalWrite(BUILTIN_LED1, HIGH); }
else if (request.indexOf("/ON") != -1){ 
digitalWrite(BUILTIN_LED1, LOW); 
}

String s = "HTTP/1.1 200 OK\r\n";
s += "Content-Type: text/html\r\n\r\n";
s += "<!DOCTYPE HTML><HTML>";
s += String(cnt);
cnt++;
s += "<p/>";
s += "<a href=/ON>ON</a>&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
s += "<a href=/OFF>OFF</a>&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
s += "</HTML>";

client.flush(); //clear previous info in the stream 
client.print(s); // Send the response to the client 
delay(1); 
Serial.println("Client disonnected"); //Looking under the hood

}






// DS18S20 Temperature chip i/o
OneWire ds(3);  

int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;

void setup(void) {
  // initialize inputs/outputs
  // start serial port
  Serial.begin(9600);
  setup1();
}

void loop(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

  ds.reset_search();
  if ( !ds.search(addr)) {
      Serial.print("No more addresses.\n");
      ds.reset_search();
      return;
  }

  Serial.print("R=");
  for( i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
  }

  if ( addr[0] == 0x10) {
      Serial.print("Device is a DS18S20 family device.\n");
  }
  else if ( addr[0] == 0x28) {
      Serial.print("Device is a DS18B20 family device.\n");
  }
  else {
      Serial.print("Device family is not recognized: 0x");
      Serial.println(addr[0],HEX);
      return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("P=");
  Serial.print(present,HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print( OneWire::crc8( data, 8), HEX);
  Serial.println();

  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100 % 100;


  if (SignBit) // If its negative
  {
     Serial.print("-");
  }
  Serial.print(Whole);
  Serial.print(".");
  if (Fract < 10)
  {
     Serial.print("0");
  }
  Serial.print(Fract);

  Serial.print("\n");
  
}






