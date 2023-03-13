
#include <Arduino.h>
#include <WiFi.h>
#include <string.h>
#include <HardwareSerial.h>

#define COMMAND_SIZE 4

void connectingInit();
void transMessage(String header, String message);

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

//ZMIENNE GLOBALNE
int value = 0;
unsigned char buffer[COMMAND_SIZE+1]; //taki rozmiar ponieważ najdłuzsza komenda 4 bajty + znak '\0'

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
    transMessage(header,message);
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

void transMessage(String header, String  message)
{
  /* funkcja odpowiadająca za tłuamczenie
  *  wiadomości HTML na użyteczne dane 
  */

  
  if(header == "POST /gripper/ON HTTP/1.1")
  {
    Serial.println("Dostlaismy posta gripper on");
  }
  else if(header == "POST /gripper/OFF HTTP/1.1")
  {
    Serial.println("Dostlaismy posta gripper off");
  }
  else if(header == "POST /Z1 HTTP/1.1")
  {
    Serial.println("Dostlaismy posta gripper Z1");
  }
  else if(header == "POST /Z2 HTTP/1.1")
  {
    Serial.println("Dostlaismy posta gripper Z2");
  }
  else if(header == "POST /Z3 HTTP/1.1")
  {
    Serial.println("Dostlaismy posta gripper Z3");
  }
//pobieramy 8 bajtów z nagłówka, dodajemy znak null i tworzymy obiekt String
  /*header.getBytes(buffer,COMMAND_SIZE+1);
  buffer[COMMAND_SIZE] = '\0';
  String str = String((char*)buffer);
  Serial.println(str);
  if(str == "POST")
  {
      Serial.println("\n dostalismy posta");
      if(header.indexOf("/"))
      {
        Serial.println("gripper on");
      }
  }
  else if(str == "GET ")
  {
      Serial.println("\n dostalismy geta");
  }*/
}
