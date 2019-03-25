#include <VariableTimedAction.h>
#include <Ethernet.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include "LcdUtils.h"

class WebServer : public VariableTimedAction {

  public:
    EthernetServer *server;
    LiquidCrystal_I2C *lcd;
    
    WebServer(EthernetServer *server, LiquidCrystal_I2C *lcd){
      this->server = server;
      this->lcd = lcd;
    }

    int getCount(){
      return count;
    }

  private:
    int count = 0;
  
    unsigned long run(){
      if(server != NULL){
        EthernetClient client = server->available(); 
      }else{
        lcd->clear();
        writeString(lcd, "No Internet access");
      }
      
      return 0;
    }
  
};
