
#include <Arduino.h>
#include <WiFi.h>
#include <string.h>
#include <HardwareSerial.h>

void connectingInit();

const char* ssid     = "dlink_DWR-932_0048";
const char* password = "DFeyg56489";

WiFiServer server(80);
HardwareSerial SerialPort(2);


void setup()
{ 
    Serial.begin(921600);
    SerialPort.begin(15200, SERIAL_8N1, 16, 17);
    pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

  connectingInit();

}

int value = 0;
unsigned char buffer[8];
void loop(){
 WiFiClient client = server.available();  // listen for incoming clients
  if(WiFi.status() == WL_CONNECTED)
  {
  if (client) {                             // if you get a client,
    Serial.println("New Client.");
    int messLineNumber = 0;
    String currentLineText = "";
    String header = "";
    String message = "";           // print a message out the serial port
    while(client.connected())
    {
      if(client.available())
      {
        char c = client.read();
        Serial.print(c);
        if(c=='\n')
        {
          if(messLineNumber == 0)
          {
            header = currentLineText;
          }
          if(messLineNumber == 9)
          {
            message = currentLineText;
          }
          currentLineText="";
          messLineNumber++;
        }
        else if(c!='\r')
        {
          currentLineText +=c;
        }
      }
      if(messLineNumber == 10)
      {
        break;
      }
    }
    Serial.println(header);
    Serial.println(message);
    SerialPort.print(message);

    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
  }
  else
  {
    Serial.println("Connecting is lost!!!");
    connectingInit();
  }
}

void connectingInit()
{
      Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();
}
