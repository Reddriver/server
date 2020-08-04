#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 
// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAA
};

DeviceAddress senzorNameObyvak = {0x28, 0xB0, 0xEC, 0xE1, 0x04, 0x00, 0x00, 0xBA};
DeviceAddress senzorNameVenku = {0x28, 0xAA, 0xC1, 0xB3, 0x12, 0x13, 0x02, 0x7B};
DeviceAddress senzorNameDetskyPokoj = {0x28, 0xAA, 0xB1, 0xA3, 0x12, 0x13, 0x02, 0x2E};
DeviceAddress senzorNameLoznice = {0x28, 0xE1, 0x01, 0xE2, 0x04, 0x00, 0x00, 0x1D};
 
// initialize the library instance:
EthernetClient client;
 
char serverObyvak[] = "reddriver4.tmep.cz";
char guidObyvak[] = "temp1";

char serverDetskyPokoj[] = "reddriver6.tmep.cz";
char guidDetskyPokoj[] = "temp1";
 
char serverLoznice[] = "reddriver7.tmep.cz"; 
char guidLoznice[] = "temp1";

char serverVenku[] = "reddriver.tmep.cz";
char guidVenku[] = "temp1";

unsigned long lastConnectionTime = 0;              // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 60L * 1000L; // delay between updates, in milliseconds
 
void setup() {
  // start serial port:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Minutove cekani na pripravenost routeru po restartu elektriny.");
  // give the ethernet module time to boot up:
  delay(postingInterval);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
    }
  printIPAddress();
  sensors.begin();
}
 
void loop() {
  switch (Ethernet.maintain())
  {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;
 
    case 2:
      //renewed success
      Serial.println("Renewed success");
 
      //print your local IP address:
      printIPAddress();
      break;
 
    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;
 
    case 4:
      //rebind success
      Serial.println("Rebind success");
 
      //print your local IP address:
      printIPAddress();
      break;
 
    default:
      //nothing happened
      break;
  }  
//       if ten seconds have passed since your last connection,
//       then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    Serial.print("Teplota obyvak: ");
    httpRequest(senzorNameObyvak, serverObyvak, guidObyvak);
    Serial.print("Teplota venku: ");
    httpRequest(senzorNameVenku, serverVenku, guidVenku);
    Serial.print("Teplota detsky pokoj: ");
    httpRequest(senzorNameDetskyPokoj, serverDetskyPokoj, guidDetskyPokoj);
    Serial.print("Teplota loznice: ");
    httpRequest(senzorNameLoznice, serverLoznice, guidLoznice);
    lastConnectionTime = millis();
  }  
}
 
// this method makes a HTTP connection to the server:
void httpRequest(DeviceAddress senzorName, char server[], char guid[]) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();
  sensors.requestTemperatures(); // Send the command to get temperatures
  float t = sensors.getTempC(senzorName);  //
 // float t = sensors.getTempCByIndex(0); // Read temperature in "t" variable
  if (t == -127.00) {                   // If you have connected it wrong, Dallas read this temperature! :)
    Serial.println("Error!");
    return;
  }
  Serial.println(t);
 
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    // send the HTTP GET request:
    client.print("GET /?");
    client.print(guid);
    client.print("=");
    client.print(t);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
  } else {
    // if you couldn't make a connection:
    Serial.println(" connection failed");
  }    
}

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  } 
  Serial.println();
}
