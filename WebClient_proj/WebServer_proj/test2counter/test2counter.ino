

#include <SPI.h>
#include <Ethernet.h>
int a=2;
int b=3;
int c=4;
int d=5;
int e=6;
int f=7;
int g=8;
int h=A1; 
int dotPin=9;
int x=0;

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,20);

EthernetServer server(80);

// Relay state and pin7
String relay1State = "Off";




// Client variables 
char linebuf[80];
int charcount=0;

void setup() { 
  // Relay module prepared 
  pinMode(a, OUTPUT);
pinMode(b, OUTPUT);
pinMode(c, OUTPUT);
pinMode(d, OUTPUT);
pinMode(e, OUTPUT);
pinMode(f, OUTPUT);
pinMode(g, OUTPUT);
pinMode(h, OUTPUT);
pinMode(dotPin, OUTPUT);// put your setup code here, to run once:

  
 
 
  
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
  client.println("<meta charset='UTF-8' name=\"viewport\" content=\"width=device-width, initial-scale=1\"><script>function getRed(){ var r=document.getElementById('red').value;alert(r);}</script></head><body style='background-color:#123652;color:white'><center>");                                                             
   client.println("<h1><strong><center>GSR Company</center></strong> </h1>");
  client.println("<h1>التحكم باشارة المرور <a href=\"/\">تحديث</a></h1>");
  // Generates buttons to control the relay
  client.println("<h4>Relay 1 - State: " + relay1State + "</h4><br />");
   client.println("<form method='GET'>");
       client.println("<input type='text' id='red'style='background-color:#00FF00;color:white ;font-size:24px;  border-radius: 30px;padding:15px' placeholder='enter number green'>");
       client.println("<input type='text' style='background-color:#FF0000;color:white;font-size:24px;  border-radius: 30px; padding:15px' placeholder='enter number Red'>");                                                                    
       client.println("<button style='background-color:#FF0000;color:white;font-size:24px;  border-radius: 30px; padding:15px' onclick='getRed()'>حفظ</button>");                                                                    

   
  // If relay is off, it shows the button to turn the output on          
 /* if(relay1State == "Off"){
    client.println("<a href=\"/relay1on\"><input type='text' style='background-color:#00FF00;color:white ;font-size:24px;  border-radius: 30px;padding:15px' placeholder='enter number green'></a>");
  }
  // If relay is on, it shows the button to turn the output off         
  else if(relay1State == "On"){
    client.println("<a href=\"/relay1off\"><input type='text' style='background-color:#FF0000;color:white;font-size:24px;  border-radius: 30px; padding:15px' placeholder='enter number Red'></a>");                                                                    
  }
  */


 client.println("</form>");
  client.println("</center></body></html>"); 
}


void digit(int digit)
{
   
  if (digit == 1)
  {
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, HIGH);
    digitalWrite(dotPin, LOW);
  }
  else if(digit == 2)
  {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, HIGH);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, HIGH);
    digitalWrite(g, LOW);
    digitalWrite(dotPin, LOW);
  }
  else if(digit == 3)
  {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, LOW);
    digitalWrite(dotPin, LOW);
  }
   else if(digit == 4)
  {
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dotPin, LOW);
}
 else if(digit == 5)
  {
    digitalWrite(a, LOW);
    digitalWrite(b, HIGH);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dotPin, LOW);

}
else if(digit == 6)
  {
    digitalWrite(a, LOW);
    digitalWrite(b, HIGH);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dotPin, LOW);

}
else if(digit == 7)
  {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(g, HIGH);
    digitalWrite(dotPin, LOW);

}
else if(digit == 8)
  {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dotPin, LOW);

}
else if(digit == 9)
  {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    digitalWrite(dotPin, LOW);

}
else if(digit == 0)
  {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, HIGH);
    digitalWrite(dotPin, LOW);

}

}

void led(){
 digitalWrite(h, HIGH);
   delay(500);
    digitalWrite(h, LOW);
    delay(500); 
}

void loop() {

  for( int i =0; i<=9; i++)
  {
    digit(i);

    led();
  }
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
              for( int i =0; i<=9; i++)
                {
                  digit(i);
                }
  
            relay1State = "Off";
          }
          else if (strstr(linebuf,"GET /relay1on") > 0){

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


