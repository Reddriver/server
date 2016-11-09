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
float teplotaDoma;
float vlhkostDoma;
int id_teploty = 3;
int id_vlhkosti = 4;
char serialCidla[] = "28FF5526440400E2";
char serialCidla2[] = "dht22_doma";
boolean start = false;
boolean konec = false;
String teplotaVenku = "";
Time t;
IPAddress server(192,168,2,2);
DHT dht(DHTPIN, DHTTYPE);
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 300L * 1000L; // delay between updates, in milliseconds
unsigned long lastConnectionTime2 = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval2 = 300L * 1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers
unsigned long lastConnectionTime3 = 0;   
const unsigned long postingInterval3 = 30L * 1000L;

void nastavCas(){
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
  lcd.begin(20,4);
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

  lcd.setCursor(0,0);
  lcd.print("Teplota doma: "); 
  lcd.setCursor(0,1);
  lcd.print("Vlhkost doma: ");
  lcd.setCursor(0,2);
  lcd.print("Teplota venku: "); 
  
  sensors.begin();
  delay(2000);
  nactiTeplotu();
  httpRequest(1);
  delay(4000);
  nactiVlhkost();
  httpRequest(2);
}

void loop() {
  if (client.available()) {
    zpracovaniRequest();
  }else {
    if (millis() - lastConnectionTime3 > postingInterval3) {
      nactiCas(); 
      nactiTeplotu();
      nactiVlhkost();  
      lastConnectionTime3= millis();
    }  
    delay(1000);   
  }    
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest(1);
  }

  if (millis() - lastConnectionTime2 > postingInterval2) {
    httpRequest(2);
  }  
}

void nactiTeplotu(){
  sensors.requestTemperatures(); 
  teplotaDoma = sensors.getTempCByIndex(0);
  lcd.setCursor(15,0);
  lcd.print("     ");
  lcd.setCursor(15,0);
  lcd.print(teplotaDoma,1);
}

void nactiVlhkost(){
  vlhkostDoma = dht.readHumidity();
  if (isnan(vlhkostDoma)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } 
  
  lcd.setCursor(15,1);
  lcd.print("     ");
  lcd.setCursor(15,1);
  lcd.print(vlhkostDoma,0); 
  lcd.print("%");
}

void nactiCas(){
  lcd.setCursor(0,3);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print(rtc.getDOWStr());
  lcd.print(" ");
  t = rtc.getTime();
  if (t.hour<10){   // Add a zero, if necessary, as above
    lcd.print(0);
  }
  lcd.print(t.hour, DEC); 
  lcd.print(":");
  if (t.min<10){   // Add a zero, if necessary, as above
    lcd.print(0);
  }
  lcd.print(t.min, DEC); 
  lcd.print(" ");
  lcd.print(t.date, DEC);
  lcd.print(".");
  lcd.print(t.mon);
  lcd.print(".");  
}
void zpracovaniRequest(){
  char c = client.read();
  Serial.print(c);
  if (c == '%'){
    start = true;
    teplotaVenku = "";
    lcd.setCursor(15,2);
    lcd.print("     ");
    lcd.setCursor(15,2);
  }
  if (c == '*'){
    konec = true;
  }  
  if (start == true && c != '%' && c != '*'){
    teplotaVenku += c;
  }  
  if(c == '*'){
    start = false;
    konec = false;
    float temp = teplotaVenku.toFloat();
    lcd.print(temp,1);
    Serial.println();
    Serial.print("teplota venku: ");
    Serial.println(teplotaVenku); 
  }
}

// this method makes a HTTP connection to the server:
void httpRequest(int cidlo) {
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println();
    Serial.print("connecting...cidlo: ");
    Serial.print(cidlo);
    Serial.print(" hodnota = ");
    client.print("GET /insert.php?");
    client.print("serial=");
    if(cidlo == 1){
      client.print(serialCidla);
      client.print("&&");
      client.print("id_veliciny=");
      client.print(id_teploty);
      client.print("&&");
      client.print("hodnota=");
      client.print(teplotaDoma);          
      lastConnectionTime = millis();
      Serial.println(teplotaDoma);
    }else{
      client.print(serialCidla2);
      client.print("&&");
      client.print("id_veliciny=");
      client.print(id_vlhkosti);
      client.print("&&");
      client.print("hodnota=");
      client.print(vlhkostDoma);
      Serial.println(vlhkostDoma);
      lastConnectionTime2
      = millis();
    }   
    client.println( " HTTP/1.1");
    client.println( "Host: www.xxx.com" );//ur web server
    client.println( "Content-Type: application/x-www-form-urlencoded" );
    client.println( "Connection: close" );
    client.println();    
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

