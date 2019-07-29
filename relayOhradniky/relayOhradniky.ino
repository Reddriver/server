#include <ESP8266WiFi.h>        // Include the Wi-Fi library

const char* ssid     = "xxx";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "xxx";     // The password of the Wi-Fi network

int relayInput = 2; // the input to the relay pin
void setup() {
  Serial.begin(115200);
  pinMode(relayInput, OUTPUT);
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void loop() {
  digitalWrite(relayInput, HIGH); // turn relay on

  delay(1000);

  digitalWrite(relayInput, LOW); // turn relay off

  delay(1000);
}
