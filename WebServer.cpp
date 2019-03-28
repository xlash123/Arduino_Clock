#include <VariableTimedAction.h>
#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

#include "LCD.h"

class WebServer : public VariableTimedAction {

  public:
    EthernetServer server = EthernetServer(80);
    IPAddress ip;

    WebServer(){
      byte mac[] = {0x8E, 0x5D, 0x7D, 0x93, 0x23, 0x13};
      Serial.println("Init Ethernet w/ DHCP");
      if(Ethernet.begin(mac) == 0){
        Serial.println("No DHCP");
        if(Ethernet.hardwareStatus() == EthernetNoHardware){
          Serial.println("No shield found");
        }else if(Ethernet.linkStatus() == LinkOFF){
          Serial.println("Ethernet is unplugged");
        }
      }
      ip = Ethernet.localIP();
      Serial.print("IP: ");
      Serial.println(ip);
      Serial.println("Webserver initialized");
      delay(2000);
    }

  private:
  
    unsigned long run(){
      EthernetClient client = server.available();
      if(client){
        Serial.println("New client:\n");
        // an http request ends with a blank line
        bool currentLineIsBlank;
        String HTTP_req = "";
        while (client.connected()) {
          if (client.available()) {
            char c = client.read();
            Serial.write(c);
            if(HTTP_req.length() < 80) HTTP_req += c;
            if (c == '\n' && currentLineIsBlank) {
              Serial.println("\nNow responding:");
              //Write response
              int iGet = HTTP_req.indexOf("GET");
              if(iGet >= 0){
                Serial.println("GET request");
                String sdFile = HTTP_req.substring(iGet+4, HTTP_req.indexOf(" ", iGet+4));

                if(sdFile == "/") sdFile = "/index.htm";
                String fileExtension = sdFile.substring(sdFile.lastIndexOf(".")+1);

                Serial.println(sdFile);
                Serial.println(fileExtension);

                bool encode = false;
                client.println("HTTP/1.1 200 OK");
                String mime = "Content-Type: ";
                if(fileExtension == "htm"){
                  mime += "text/html";
                }else if(fileExtension == "js"){
                  mime += "text/javascript";
                  encode = true;
                }else mime += "text/text";
                Serial.print("Mime: ");
                Serial.println(mime);
                client.println(mime);
                if(encode) client.println("Content-Encoding: gzip");
                client.println("Connection: Close");
                client.println();
                File webpage = SD.open(sdFile);
                if(webpage){
                  Serial.print("Sending data to client: ");
                  Serial.println(webpage.available());
                  int i = 0;
                  while (webpage.available()) {
                    char cc = webpage.read();
                    client.write(cc);
                  }
                  Serial.println("Data sent");
                  webpage.close();
                }
              }
              Serial.println("\nResponse terminated");
              client.flush();
              client.stop();
            }
            if (c == '\n') {
              // you're starting a new line
              currentLineIsBlank = true;
            } else if (c != '\r') {
              // you've gotten a character on the current line
              currentLineIsBlank = false;
            }
          }
        }

        delay(1);
        client.stop();
      }
      
      return 0;
    }
  
};
