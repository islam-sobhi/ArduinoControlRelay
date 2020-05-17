
//zoomkat 12-08-12
//get submit box code
//for use with IDE 1.0
//open serial monitor to see what the arduino receives
//use the \ slash to escape the " in the html or use a '
//address will look like http://192.168.1.102:84 when submited
//for use with W5100 based ethernet shields
//note that the below bug fix may be required
// http://code.google.com/p/arduino/issues/detail?id=605 

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 0, 20 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(84); //server port

String readString; 

//////////////////////

void setup(){

  pinMode(5, OUTPUT); //pin selected to control
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, gateway, subnet);
  server.begin();

  //enable serial data print 
  Serial.begin(9600); 
  Serial.println("server text box test1"); // so I can keep track of what is loaded
}

void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string 
          readString += c; 
          //Serial.print(c);
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          Serial.println(readString); //see what was captured

          //now output HTML data header

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino GET test page</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");

          client.println("<H1>HTML form GET example</H1>");

          client.println("<FORM ACTION='/' method=get >"); //uses IP/port of web page

          client.println("Pin 5 'on5' or 'off5': <INPUT TYPE=TEXT NAME='LED' VALUE='' SIZE='25' MAXLENGTH='50'><BR>");

          client.println("<INPUT TYPE=SUBMIT NAME='submit' VALUE='Change Pin 5!'>");

          client.println("</FORM>");

          client.println("<BR>");

          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
          //stopping client
          client.stop();

          /////////////////////
          if(readString.indexOf("on5") >0)//checks for on
          {
            digitalWrite(5, HIGH);    // set pin 5 high
            Serial.println("Led On");
          }
          if(readString.indexOf("off5") >0)//checks for off
          {
            digitalWrite(5, LOW);    // set pin 5 low
            Serial.println("Led Off");
          }
          //clearing string for next read
          readString="";

        }
      }
    }
  }
} 

