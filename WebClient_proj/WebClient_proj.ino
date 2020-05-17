#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "www.google.com";    // name address for Google (using DNS)
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,20);
EthernetClient client;

    void setup() {
        Serial.begin(9600);
         while (!Serial) {
          ;
        }
      
        // start the Ethernet connection:
        if (Ethernet.begin(mac) == 0) {
          Serial.println("Failed to configure Ethernet using DHCP");
          Ethernet.begin(mac, ip);
        }
        delay(1000);
        Serial.println("connecting...");
      
        if (client.connect(server, 80)) {
          Serial.println("connected");
          // Make a HTTP request:
          client.println("GET /search?q=arduino HTTP/1.1");
          client.println("Host: www.google.com");
          client.println("Connection: close");
          client.println();
        } 
        else {
          // kf you didn't get a connection to the server:
          Serial.println("connection failed");
        }
    }

   void loop()
   {
    // if there are incoming bytes available 
        if (client.available()) {
          char c = client.read();
          Serial.print(c);
        }
      
        // if the server's disconnected, stop the client:
        if (!client.connected()) {
          Serial.println();
          Serial.println("disconnecting.");
          client.stop();
          // do nothing forevermore:
          while(true);
        }
    }

