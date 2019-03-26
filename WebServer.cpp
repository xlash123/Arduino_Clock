#include <VariableTimedAction.h>
#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

#include "LCD.h"

class WebServer : public VariableTimedAction {

  public:
    EthernetServer server = EthernetServer(80);

  private:
  
    unsigned long run(){
      EthernetClient client = server.available();
      if(client){
        Serial.println("New client:");
        // an http request ends with a blank line
        bool currentLineIsBlank;
        String HTTP_req = "";
        while (client.connected()) {
          if (client.available()) {
            char c = client.read();
            Serial.write(c);
            if(HTTP_req.length() < 80) HTTP_req += c;
            if (c == '\n' && currentLineIsBlank) {
              //Write response
              if(HTTP_req.indexOf("GET") >= 0){
                String sdFile = HTTP_req.substring(HTTP_req.indexOf("GET")+4, HTTP_req.indexOf(" "));
                String fileExtension = sdFile.substring(sdFile.lastIndexOf(".")+1);

                client.println("HTTP/1.1 200 OK");
                String mime = "Content-Type: ";
                if(fileExtension == "html"){
                  mime += "text/html";
                }else if(fileExtension == "js"){
                  mime += "text/javascript";
                }else mime += "text/text";
                client.println(mime);
                client.println("Connection: close");
                client.println();
                File webpage = SD.open(sdFile);
                if(webpage){
                  while (webpage.available()) {
                    client.write(webpage.read());
                  }
                  webpage.close();
                }
              }
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
