//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

R_TRIG TriggerTelemetryData;

void initThingsBoard(void) {
  prglog("Beginning ThingsBoard Initialization");

  if(!gEthernetConnectionActive){
    prglog("ThingsBoardTask:Ethernet Connection Not Active-Returning to try again later");
    return;
  }else{
    
  if (!tb.connected()) {
    // Connect to the ThingsBoard
    prglog("Connecting to: ");
    prglog(THINGSBOARD_SERVER);
    prglog(" with token ");
    prglog(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      prglog("Failed to connect");
      return;
    }else{
      prglog(String("Connected To: " + String(THINGSBOARD_SERVER)).c_str());
    }
  }
    
    tskThingsBoard.setCallback(&cyclicThingsBoard);
  }

  prglog("Finished ThingsBoard Initialization");
}

void cyclicThingsBoard(void) {
  logTaskTimer(&ts_low, "ThingsBoard", "Low");

  TriggerTelemetryData.process(switchState);
  if(TriggerTelemetryData.Q){
    int Temperature, Humidity;
    Temperature = rtc.getSeconds();
    Humidity = rtc.getMinutes();
    prglog(String("Sent Data: Temperature: " + String(Temperature) + " Humidity: " + String(Humidity) + ";").c_str());
    tb.sendTelemetryInt("temperature",Temperature);
    tb.sendTelemetryInt("humidity", Humidity);
  }
  

}
