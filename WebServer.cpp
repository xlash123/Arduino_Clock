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
      char *ipS = getIpString(ip);
      Serial.println(ipS);
      LCD::writeString(ipS, 3);
      free(ipS);
      Serial.println("Webserver initialized");
    }

  private:

    int count;

    char *getIpString(IPAddress ip){
      char *ret = malloc(sizeof(char) * (4*3+4+3));
      sprintf(ret, "IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
      return ret;
    }
  
    unsigned long run(){
      if(Ethernet.maintain() && count%15 == 0){
        ip = Ethernet.localIP();
        LCD::clearRow(1);
        char *ipS = getIpString(ip);
        LCD::writeString(ipS, 3);
        free(ipS);
        count = 0;
      }
      EthernetClient client = server.available();
      if(client){
        Serial.println("New client:\n");
        LCD::clearRow(1);
        LCD::writeString("Serving HTTP...", 1);
        char *HTTP_req = calloc(client.available()+2, sizeof(char));
        int len = 0;
        while (client.connected()) {
          bool respond = false;
          if (client.available() && !respond) {
            char c = client.read();
            Serial.write(c);
            HTTP_req[len] = c;
            len++;
          }
          if(!client.available()) respond = true;
          if(respond){
            Serial.println("\nNow responding:");
            //Write response
            char *iGet = strstr(HTTP_req, "GET");
            char *iPost = strstr(HTTP_req, "POST");
            if(iGet){
              Serial.println("GET request");
              iGet += 4;
              char *endFile = strstr(iGet, " ");
              char *sdFile = calloc(endFile-iGet+1, sizeof(char));
              strncpy(sdFile, iGet, endFile-iGet);
              sdFile[endFile-iGet+1] = '\0';

              if(strcmp(sdFile, "/") == 0){
                realloc(sdFile, sizeof(char)*11);
                strcpy(sdFile, "/index.htm");
              }
              char *iFileExt;
              for(iFileExt=endFile; iFileExt>iGet && *iFileExt=='.'; iFileExt--);
              char *fileExtension = malloc(sizeof(char)*(endFile-iFileExt+1));
              strncpy(fileExtension, iFileExt, endFile-iFileExt);
              fileExtension[endFile-iFileExt+1] = '\0';
  
              Serial.println(sdFile);
              Serial.println(fileExtension);
  
              bool encode = false;
              client.println("HTTP/1.1 200 OK");
              char *mime = malloc(sizeof(char)*40);
              strcpy(mime, "Content-Type: ");
              if(fileExtension == "htm"){
                strcat(mime, "text/html");
              }else if(fileExtension == "js"){
                strcat(mime, "text/javascript");
                encode = true;
              }else if(fileExtension == "ico"){
                strcat(mime, "image/x-icon");
              }else strcat(mime, "text/text");
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
            }else if(iPost){
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
