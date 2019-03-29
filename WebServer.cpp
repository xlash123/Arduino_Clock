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
      LCD::clearRow(1);
      LCD::writeString("Getting IP...", 1);
      if(Ethernet.begin(mac) == 0){
        Serial.println("No DHCP");
        if(Ethernet.hardwareStatus() == EthernetNoHardware){
          Serial.println("No shield found");
        }else if(Ethernet.linkStatus() == LinkOFF){
          Serial.println("Ethernet is unplugged");
        }
      }
      ip = Ethernet.localIP();
      String ipS = "IP: " + getIpString(ip);
      Serial.println(ipS);
      LCD::writeString(ipS.c_str(), 3);
      Serial.println("Webserver initialized");
    }

  private:

    int count;

    String getIpString(IPAddress ip){
      
      return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
    }
  
    unsigned long run(){
      if(Ethernet.maintain() && count%15 == 0){
        ip = Ethernet.localIP();
        LCD::clearRow(1);
        String ipS = "IP: " + getIpString(ip);
        LCD::writeString(ipS.c_str(), 3);
        count = 0;
      }
      EthernetClient client = server.available();
      if(client){
        Serial.println("New client:\n");
        LCD::clearRow(1);
        LCD::writeString("Serving HTTP...", 1);
        String HTTP_req = "";
        while (client.connected()) {
          bool respond = false;
          if (client.available() && !respond) {
            char c = client.read();
            Serial.write(c);
            HTTP_req += c;
          }
          if(!client.available()) respond = true;
          if(respond){
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
              }else if(fileExtension == "ico"){
                mime += "image/x-icon";
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
                unsigned long size = webpage.size();
                Serial.println(size);
                while (webpage.available() && client.connected()) {
                  char cc = webpage.read();
                  client.write(cc);
                }
                if(client.connected()) Serial.println("Data sent");
                else Serial.println("Client disconnected");
                webpage.close();
              }
            }else if(HTTP_req.indexOf("POST") >= 0){
              Serial.println("POST request");
            }
            Serial.println("\nResponse terminated");
            client.flush();
            client.stop();
            respond = false;
          }
        }
        LCD::clearRow(1);
        client.stop();
      }
      count++;
      return 0;
    }
  
};
