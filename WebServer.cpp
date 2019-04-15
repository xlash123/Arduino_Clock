#include <VariableTimedAction.h>
#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>
#include <Time.h>
#include <Timezone.h>

#include "LCD.h"

class WebServer : public VariableTimedAction {

  public:
    EthernetServer server = EthernetServer(80);
    IPAddress ip;

    WebServer(Timezone *zone){
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
      this->zone = zone;
      Serial.println("Webserver initialized");
    }

  private:

    int count;
    Timezone *zone;

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
            Serial.print(c);
            HTTP_req[len] = c;
            HTTP_req[len+1] = '\0';
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

              File webpage = SD.open(sdFile);
              time_t modTime = 0;
              if(webpage){
                modTime = zone->toUTC(webpage.getModificationDate());
              }
              webpage.close();
              
              char *ifMod = strstr(HTTP_req, "If-Modified-Since: ");
              if(ifMod){
                Serial.println("I've detected a cache check");
                ifMod += strlen("If-Modified-Since: ");
                char dayName[10] = {0}, monthName[10] = {0};
                int dayy, yearr, h, m, s, mon = 1;
                sscanf(ifMod, "%s %d %s %d %d:%d:%d", dayName, &dayy, monthName, &yearr, &h, &m, &s);
                for(uint8_t i=1; i<=12; i++){
                  char monS[4];
                  strcpy(monS, monthShortStr(i));
                  if(strcmp(monthName, monS) == 0){
                    mon = i;
                    break;
                  }
                }
                TimeElements te;
                te.Second = (uint8_t) s;
                te.Minute = (uint8_t) m;
                te.Hour = (uint8_t) h;
                te.Day = (uint8_t) dayy;
                te.Month = (uint8_t) mon;
                te.Year = (uint8_t) (yearr-1970);
                time_t ifModTime = makeTime(te);
                Serial.print("Requesting file younger than: ");
                Serial.println(ifModTime);
                Serial.println(modTime);
                if(ifModTime >= modTime){
                  Serial.println("Cache still good");
                  client.println("HTTP/1.1 304 NOT MODIFIED");
                  client.println("");
                  client.flush();
                  client.stop();
                  return 0;
                }
                Serial.println("File was modified recently. Sending new one");
              }
  
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
              if(encode){
                client.println("Content-Encoding: gzip");
                client.println("Cache-Control:public, max-age=60");
                client.print("Last-Modified: ");

                char lastModDate[60] = {0};
                char dayName[4], monthName[4];
                strcpy(dayName, dayShortStr(weekday(modTime)));
                strcpy(monthName, monthShortStr(month(modTime)));
                sprintf(lastModDate, "%s, %.2d %s %d %.2d:%.2d:%.2d GMT", dayName, day(modTime), monthName, year(modTime), hour(modTime), minute(modTime), second(modTime));
                client.println(lastModDate);
                Serial.print("File date: ");
                Serial.println(lastModDate);
              }

              webpage = SD.open(sdFile);
              if(webpage){
                client.print("Content-Length: ");
                client.println(webpage.size());
              }
              
              client.println("Connection: Close");
              client.println();
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
