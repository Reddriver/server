    /*
     
     Simple sketch for sending data to the TMEP.cz 
     
     This sketch connects to a a web server and makes a request
     using a Wiznet Ethernet shield. You can use the Arduino Ethernet shield, or
     the Adafruit Ethernet shield, either one will work, as long as it's got
     a Wiznet Ethernet module on board.
     
     Uses code examples:
     
     http://www.arduino.cc/en/Tutorial/WebClientRepeating
     http://www.arduino.cc/en/Tutorial/DhcpAddressPrinter
     
     */
     
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
     
    // initialize the library instance:
    EthernetClient client;
     
    char server[] = "reddriver4.tmep.cz";   // domain.tmep.cz
    char guid[] = "temp1";        // guid
     
    unsigned long lastConnectionTime = 0;              // last time you connected to the server, in milliseconds
    const unsigned long postingInterval = 60L * 1000L; // delay between updates, in milliseconds
    // the "L" is needed to use long type numbers
     
    void setup() {
      // start serial port:
      Serial.begin(9600);
      while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
      }
     
      // give the ethernet module time to boot up:
      delay(1000);
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
     
     
     
      // if there's incoming data from the net connection.
      // send it out the serial port.  This is for debugging
      // purposes only:
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
      }
     
      // if ten seconds have passed since your last connection,
      // then connect again and send data:
      if (millis() - lastConnectionTime > postingInterval) {
        httpRequest();
      }
     
    }
     
    // this method makes a HTTP connection to the server:
    void httpRequest() {
      // close any connection before send a new request.
      // This will free the socket on the WiFi shield
      client.stop();
     
      // call sensors.requestTemperatures() to issue a global temperature 
      // request to all devices on the bus
      Serial.print("Requesting temperatures...");
      sensors.requestTemperatures(); // Send the command to get temperatures
      Serial.println("DONE");
      // After we got the temperatures, we can print them here.
      // We use the function ByIndex, and as an example get the temperature from the first sensor only.
      Serial.print("Temperature for the device 1 (index 0) is: ");
      float t = sensors.getTempCByIndex(0); // Read temperature in "t" variable
      if (t == -127.00) {                   // If you have connected it wrong, Dallas read this temperature! :)
        Serial.println("Error!");
        return;
      }
      Serial.println(t);
     
      // if there's a successful connection:
      if (client.connect(server, 80)) {
        Serial.print("connecting...");
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
     
        Serial.println(" done.");
        // note the time that the connection was made:
        lastConnectionTime = millis();
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