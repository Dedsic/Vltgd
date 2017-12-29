#include <Arduino.h>
#include <ESP8266WiFi.h>
const char* ssid = "YOUR_SSID";        //type your ssid
const char* password = "YOUR_PASSWORD";        //type your password
WiFiServer server(80);
int relay0 = 2;         // prepare GPIO2 of ESP8266

void setup() {
    Serial.begin(115200);
    delay(10);
    pinMode(relay0, OUTPUT);
    digitalWrite(relay0, LOW);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.print("Use this URL to connect: http://");
    Serial.print(WiFi.localIP());
}

void loop() {
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    
    // Wait until the client sends some data
    Serial.println("new client");
    while(!client.available()) {
        delay(1);
    }
    
    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();
    
    // Match the request
    int status0 = LOW;
    if (request.indexOf("/switch0") != -1) {
        if (status0 == LOW)
            status0 = HIGH;
        else status0 = LOW;
    }

    // Set switch0 according to the request
    digitalWrite(relay0, status0);
    client.flush();
 
   // Return the response
   String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nRelay0 is ";
   s += (status0)?"ON":"OFF";
   s += "<a href=\"/switch0\">Switch</a> \n";
   s += "</html>\n";
   client.print(s);
   delay(1);
   Serial.println("Client disonnected");
}