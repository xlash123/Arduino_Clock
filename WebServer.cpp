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
      char *ret = (char *) malloc(sizeof(char) * (4*3+4+3));
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
        int maxLen = client.available()+8;
        Serial.print("Max: ");
        Serial.println(maxLen);
        char *HTTP_req = (char *) malloc(maxLen*sizeof(char));
        HTTP_req[0] = '\0';
        int len = 0;
        while (client.connected()) {
          bool respond = false;
          if (client.available() && !respond) {
            char c = client.read();
            HTTP_req[len] = c;
            HTTP_req[len+1] = '\0';
            len++;
          }
          if(!client.available()) respond = true;
          if(respond){
            Serial.println("Now responding:");
            //Write response
            char *iGet = strstr(HTTP_req, "GET");
            char *iPost = strstr(HTTP_req, "POST");
            if(iGet){
              Serial.println("GET request");
              iGet += 4;
              char *endFile = strstr(iGet, " ");
              int fileLen = endFile-iGet;
              char *sdFile = (char *) malloc((fileLen+1)*sizeof(char));
              sdFile[0] = '\0';
              strncpy(sdFile, iGet, fileLen);
              sdFile[fileLen] = '\0';

              if(strcmp(sdFile, "/") == 0){
                realloc(sdFile, sizeof(char)*15);
                strcpy(sdFile, "/index.htm");
              }

              Serial.print("File: ");
              Serial.println(sdFile);

              char *fileExtension = strrchr(sdFile, '.')+1;

              Serial.print("File Extension: ");
              Serial.println(fileExtension);
  
              bool encode = false;
              client.println("HTTP/1.1 200 OK");
              char *mime = (char *) malloc(sizeof(char)*40);
              strcpy(mime, "Content-Type: ");
              if(strcmp(fileExtension, "htm")==0){
                strcat(mime, "text/html");
              }else if(strcmp(fileExtension, "js")==0){
                strcat(mime, "text/javascript");
                encode = true;
              }else if(strcmp(fileExtension, "ico")==0){
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
