#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <ArduinoJson.h>
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
char guid[] = "temp2";
const char* jsonurl = "https://tmep.cz/vystup-json.php?id=423&export_key=byrl2kn1vq";

// initialize the library instance:
EthernetClient client;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float teplotaPokoj;
float vlhkostPokoj;
int id_teploty = 3;
int id_vlhkosti = 4;
char serialCidla[] = "28FF5526440400E2";
char serialCidla2[] = "dht22_doma";
boolean start = false;
boolean konec = false;
float teplotaVenku;
float teplotaObyvak;
boolean startProgramu = true;
String temp;
String temp2;
Time t;
char server[] = "reddriver2.tmep.cz";
DHT dht(DHTPIN, DHTTYPE);
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 600L * 1000L; // delay between updates, in milliseconds
unsigned long lastConnectionTime2 = 0;
const unsigned long postingInterval2 = 60L * 1000L;
void nastavCas() {
  rtc.setDOW(SUNDAY);
  rtc.setTime(18, 53, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(7, 11, 2016);   // Set the date to January 1st, 2014
}

void setup() {
  rtc.begin();
  //nastavCas();
  // start serial port:
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
  //nastavObrazovku1();

  sensors.begin();

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
    lcd.print("Teplota venku: ");
    lcd.setCursor(15, 0);
    lcd.print("     ");
    lcd.setCursor(15, 0);
    lcd.print(teplotaPokoj, 1);
    lcd.setCursor(15, 1);
    lcd.print("     ");
    lcd.setCursor(15, 1);
    lcd.print(vlhkostPokoj, 0);
    lcd.print("%");
    lcd.setCursor(15, 2);
    lcd.print(teplotaVenku, 1);
  } else {
    Serial.println("start");
    lcd.setCursor(3, 0);
    lcd.print("Startuji meteo");
    lcd.setCursor(7, 1);
    lcd.print("stanici");
    lcd.setCursor(2, 2);
    lcd.print("verze 4.12.2016");
    lcd.setCursor(2, 3);
    lcd.print("Lukas Prochazka");
    startProgramu = false;
  }
}

void nastavObrazovku2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Teplota obyvak: ");
  lcd.print(teplotaObyvak, 1);
}
void loop() {
  if (client.available() && !startProgramu) {
    zpracovaniRequest();
  }else{
    nastavObrazovku1();
    delay(5000);
    nastavObrazovku2(); 
    delay(5000);
  }
  
  if ((millis() - lastConnectionTime > postingInterval) || startProgramu) {
    nactiTeplotu();
    nactiVlhkost();
    Serial.println("httpRequest1");
    httpRequest();
  }
  if ((millis() - lastConnectionTime2 > postingInterval) || startProgramu) {
    Serial.println("httpRequest2");
    httpRequest2();
  }

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


void zpracovaniRequest() {
  char c = client.read();
  if (c == '{') {
    start = true;
    temp += c;
  }
  if (c == '}') {
    konec = true;
    temp += c;
  }
  if (start == true && c != '{' && c != '}') {
    temp += c;
  }
  if (konec == true) {
    start = false;
    konec = false;
    Serial.println(temp);
    StaticJsonBuffer<300> jsonBuffer; // možná dynamicBuffer
    JsonObject& root = jsonBuffer.parseObject(temp);
    if (!root.success())
    {
      Serial.print("ParseObject() failed");
      return;
    }
    temp = "";
    teplotaVenku = root["teplota"];
  }
}

void httpRequest() {
  client.stop();
  if (client.connect(server, 80)) {
    Serial.print("connecting...");
    // send the HTTP GET request:
    client.print("GET /?temp2=");
    client.print(teplotaPokoj);
    client.print("&humV=");
    client.print(vlhkostPokoj);
    client.print("");
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println( "Content-Type: application/x-www-form-urlencoded" );
    client.println("Connection: close");
    client.println();

    Serial.println(" done.");
    lastConnectionTime = millis();
  } else {
    Serial.println(" connection failed");
  }
}

void httpRequest2() {
  client.stop();
  char url[50];
  strncpy(url, &jsonurl[15], strlen(jsonurl) - 14);
  if (client.connect("tmep.cz", 80)) {
    // send the HTTP GET request:
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + "tmep.cz" + "\r\n" +
                 "Connection: close\r\n\r\n");
    Serial.println(" done.");
    lastConnectionTime2 = millis();
  } else {
    Serial.println(" connection failed");
  }
}



