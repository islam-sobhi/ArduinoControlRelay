

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,20);

EthernetServer server(80);

// Relay state and pin7
String relay1State = "Off";
const int relay = 7;



// Client variables 
char linebuf[80];
int charcount=0;

void setup() { 
  // Relay module prepared 
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  
 
 
  
  // Open serial communication at a baud rate of 9600
  Serial.begin(9600);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

// Display dashboard page with on/off button for relay

void dashboardPage(EthernetClient &client) {
  client.println("<!DOCTYPE HTML><html><head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style='background-color:#123652;color:white'><center>");                                                             
 
  client.println("<h3>Control Gate 1 - <a href=\"/\">Refresh</a></h3>");
  // Generates buttons to control the relay
  client.println("<h4>Relay 1 - State: " + relay1State + "</h4><br />");
   
  // If relay is off, it shows the button to turn the output on          
  if(relay1State == "Off"){
    client.println("<a href=\"/relay1on\"><button style='background-color:#00FF00;color:white ;font-size:24px;  border-radius: 30px;padding:15px'>ON</button></a>");
  }
  // If relay is on, it shows the button to turn the output off         
  else if(relay1State == "On"){
    client.println("<a href=\"/relay1off\"><button style='background-color:#FF0000;color:white;font-size:24px;  border-radius: 30px; padding:15px'>OFF</button></a>");                                                                    
  }


 
  client.println("</center></body></html>"); 
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
       char c = client.read();
       //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          dashboardPage(client);
          break;
        }
        if (c == '\n') {
          if (strstr(linebuf,"GET /relay1off") > 0){
            digitalWrite(relay, HIGH);
            relay1State = "Off";
          }
          else if (strstr(linebuf,"GET /relay1on") > 0){
            digitalWrite(relay, LOW);
            relay1State = "On";
          }
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;          
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

