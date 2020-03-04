#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include "DHT.h"

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

DS3231  rtc(SDA, SCL);
#define ONE_WIRE_BUS 8
#define DHTPIN 7
#define DHTTYPE DHT22

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
IPAddress ip(192, 168, 2, 30);
IPAddress myDns(192, 168, 2, 1);

// initialize the library instance:
EthernetClient client;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float teplotaPokoj;
float vlhkostPokoj;
char serialCidla[] = "28FF5526440400E2";
char serialCidla2[] = "dht22_doma";
boolean startProgramu = true;
Time t;
char server[] = "reddriver2.tmep.cz";   // domain.tmep.cz
char guid[] = "temp2";        // guid
DHT dht(DHTPIN, DHTTYPE);
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 60L * 1000L; // delay between updates, in milliseconds

void nastavCas() {
  rtc.setDOW(THURSDAY);
  rtc.setTime(17, 57, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(04, 03, 2020);   // Set the date to January 1st, 2014
}

void setup() {
  rtc.begin();
//  nastavCas();
  Serial.begin(9600);
  dht.begin();
  lcd.begin(20, 4);
  sensors.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip, myDns);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  nastavObrazovku1();  
  startProgramu = false;
  provedCinnosti();
  sensors.begin();
  delay(4000);  
}

void nastavObrazovku1() {
  lcd.clear();
  if (startProgramu == false) {
    nactiCas();
    lcd.setCursor(0, 0);
    lcd.print("Teplota pokoj: ");
    lcd.setCursor(0, 1);
    lcd.print("Vlhkost pokoj: ");
    lcd.setCursor(0, 2);
    lcd.print("     ");
    lcd.setCursor(15, 0);
    lcd.print(teplotaPokoj, 1);
    lcd.setCursor(15, 1);
    lcd.print("     ");
    lcd.setCursor(15, 1);
    lcd.print(vlhkostPokoj, 0);
    lcd.print("%");
  } else {
    Serial.println("start");
    lcd.setCursor(3, 0);
    lcd.print("Startuji meteo");
    lcd.setCursor(7, 1);
    lcd.print("stanici");
    lcd.setCursor(2, 2);
    lcd.print("verze 4.3.2020");
    lcd.setCursor(2, 3);
    lcd.print("Lukas Prochazka");
    startProgramu = false;
  }
}

void loop() {  
  if (millis() - lastConnectionTime > postingInterval) {
    provedCinnosti();
  }
  delay(10000);
}

void provedCinnosti(){
  nactiTeplotu();
  nactiVlhkost();
  nastavObrazovku1();
  httpRequest();
}

void nactiTeplotu() {
  sensors.requestTemperatures();
  teplotaPokoj = sensors.getTempCByIndex(0);
}

void nactiVlhkost() {
  vlhkostPokoj = dht.readHumidity();
  if (isnan(vlhkostPokoj)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void nactiCas() {
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print(rtc.getDOWStr());
  lcd.print(" ");
  t = rtc.getTime();
  if (t.hour < 10) { // Add a zero, if necessary, as above
    lcd.print(0);
  }
  lcd.print(t.hour, DEC);
  lcd.print(":");
  if (t.min < 10) { // Add a zero, if necessary, as above
    lcd.print(0);
  }
  lcd.print(t.min, DEC);
  lcd.print(" ");
  lcd.print(t.date, DEC);
  lcd.print(".");
  lcd.print(t.mon);
  lcd.print(".");
}


void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.print("connecting...");
    // send the HTTP GET request:
    client.print("GET /?");
    client.print(guid);
    client.print("=");
    client.print(teplotaPokoj);
    client.print("&humV=");
    client.print(vlhkostPokoj);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
 
    Serial.println(" done.");
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println(" connection failed");
  }    
}
