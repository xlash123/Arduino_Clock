#include <VariableTimedAction.h>
#include <Ethernet.h>
#include <SPI.h>
#include <SdFat.h>
#include <string.h>
#include <Time.h>
#include <Timezone.h>
#include <TMRpcm.h>

#include "LCD.h"
#include "Command.h"

class WebServer : public VariableTimedAction {

  public:
    EthernetServer server = EthernetServer(80);
    IPAddress ip;

    WebServer(Timezone *zone, TMRpcm *audio){
      byte mac[] = {0x8E, 0x5D, 0x7D, 0x93, 0x23, 0x13};
      Serial.println(F("Init Ethernet w/ DHCP"));
      LCD::clearRow(1);
      LCD::writeString("Getting IP...", 1);
      if(Ethernet.begin(mac) == 0){
        Serial.println(F("No DHCP"));
        if(Ethernet.hardwareStatus() == EthernetNoHardware){
          Serial.println(F("No shield found"));
        }else if(Ethernet.linkStatus() == LinkOFF){
          Serial.println(F("Ethernet is unplugged"));
        }
      }
      ip = Ethernet.localIP();
      char *ipS = getIpString(ip);
      Serial.println(ipS);
      LCD::writeString(ipS, 3);
      free(ipS);
      this->zone = zone;
      this->audio = audio;
      Serial.println(F("Webserver initialized"));
    }

  private:

    int count;
    Timezone *zone;
    TMRpcm *audio;
    

    char *getIpString(IPAddress ip){
      char *ret = (char *) malloc(sizeof(char) * (4*3+4+3));
      sprintf(ret, "IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
      return ret;
    }
  
    unsigned long run(){
      if(Ethernet.maintain() && count%5 == 0){
        ip = Ethernet.localIP();
        LCD::clearRow(1);
        char *ipS = getIpString(ip);
        LCD::writeString(ipS, 3);
        free(ipS);
        count = 0;
      }
      EthernetClient client = server.available();
      if(client){
        Serial.println(F("New client:\n"));
        LCD::clearRow(1);
        LCD::writeString("Serving HTTP...", 1);
        size_t maxLen = client.available()+8;
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
            audio->disable();
            Serial.println(F("\nNow responding:"));
            //Write response
            char *iGet = strstr(HTTP_req, "GET");
            char *iPost = strstr(HTTP_req, "POST");
            if(iGet){
              Serial.println(F("GET request"));
              iGet += 4;
              char *endFile = strstr(iGet, " ");
              int fileLen = endFile-iGet;
              char *sdFile = (char *) malloc((fileLen+1)*sizeof(char));
              sdFile[0] = '\0';
              strncpy(sdFile, iGet, fileLen);
              sdFile[fileLen] = '\0';

              if(strcmp(sdFile, "/") == 0){
                realloc(sdFile, sizeof(char)*15);
                strcpy(sdFile, "/index.html");
              }

              Serial.print("File: ");
              Serial.println(sdFile);

              char *fileExtension = strrchr(sdFile, '.')+1;

              Serial.print(F("File Extension: "));
              Serial.println(fileExtension);

              SdFile webpage;
              webpage.open(sdFile);
              time_t modTime = 0;
              if(webpage.isOpen()){
                dir_t d;
                webpage.dirEntry(&d);
                modTime = zone->toUTC(FATTimeDateToUnix(d.lastWriteTime, d.lastWriteDate));
              }
              webpage.close();
              
              char *ifMod = strstr(HTTP_req, "If-Modified-Since: ");
              if(ifMod){
                ifMod += 19;
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
                if(ifModTime >= modTime){
                  Serial.println(F("Cache still good"));
                  client.println(F("HTTP/1.1 304 NOT MODIFIED"));
                  client.println("");
                  client.flush();
                  client.stop();
                  break;
                }
                Serial.println(F("File was modified recently. Sending new one"));
              }
  
              client.println("HTTP/1.1 200 OK");
              char *mime = (char *) malloc(sizeof(char)*40);
              strcpy(mime, "Content-Type: ");
              if(strcmp(fileExtension, "html")==0){
                strcat(mime, "text/html");
              }else if(strcmp(fileExtension, "js")==0){
                strcat(mime, "text/javascript");
              }else if(strcmp(fileExtension, "ico")==0){
                strcat(mime, "image/x-icon");
              }else strcat(mime, "text/text");
              Serial.print("Mime: ");
              Serial.println(mime);
              client.println(mime);

              webpage.open(sdFile);
              uint32_t size = 0;
              if(webpage.isOpen()){
                client.print(F("Content-Length: "));
                dir_t d;
                webpage.dirEntry(&d);
                size = d.fileSize;
                client.println(size);
              }
              
              client.println("Connection: Close");
              client.println();
              if(webpage.isOpen()){
                Serial.print(F("Sending data to client: "));
                Serial.println(size);
                while (webpage.available() && client.connected()) {
                  char cc = webpage.read();
                  client.write(cc);
                }
                if(client.connected()) Serial.println(F("Data sent"));
                else Serial.println(F("Client disconnected"));
                webpage.close();
              }else Serial.println(F("File failed to open"));
            }else if(iPost){
              // Fill POST request
              Serial.println(F("POST request"));
              char* body = strstr(iPost, "\r\n\r\n") + 2;
              client.println(F("HTTP/1.1 200 OK"));
              client.println(F("Content-Type: text/plain"));
              client.println();
              size_t cmdLen = strlen(body);
              char* cmd = (char *)malloc(sizeof(char) * cmdLen);
              strcpy(cmd, body);
              Command::run(cmd, cmdLen, &client);
              free(cmd);
            }
            Serial.println(F("\nResponse terminated"));
            client.flush();
            client.stop();
            respond = false;
          }
        }
        LCD::clearRow(1);
        client.flush();
        client.stop();
      }
      count++;
      return 0;
    }

    static time_t FATTimeDateToUnix(uint16_t time, uint16_t date){
      TimeElements te;
      te.Second = FAT_SECOND(time);
      te.Minute = FAT_MINUTE(time);
      te.Hour = FAT_HOUR(time);
      te.Day = FAT_DAY(date);
      te.Month = FAT_MONTH(date);
      te.Year = (uint8_t) (FAT_YEAR(date)-1970);
      return makeTime(te);
    }
};
