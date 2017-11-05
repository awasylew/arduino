#include <ESP8266WiFi.h>

const short int BUILTIN_LED1 = 2; //GPIO2
const short int BUILTIN_LED2 = 16;//GPIO16

WiFiServer server(80); //Initialize the server on Port 80

int cnt = 0;

void setup() {

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

WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
WiFi.softAP("Hello_IoT", "12345678"); // Provide the (SSID, password); . 
server.begin(); // Start the HTTP Server

Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server 
Serial.print("Server IP is: "); // Print the IP to the monitor window 
Serial.println(HTTPS_ServerIP);



}

void loop() { 
  
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





