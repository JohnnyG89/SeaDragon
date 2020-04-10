//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

void initCommManager(void) {
  prglog("Beginning CommManager Initialization");
  
  Ethernet.init(5);   //CS pin for P1AM-ETH
  Ethernet.begin(mac);  // Get IP from DHCP
  Serial.println(Ethernet.localIP());

  tskCommManager.setCallback(&cyclicCommManager);

  gEthernetConnectionActive=true;

  prglog("Finished CommManager Initialization");
}

void cyclicCommManager(void) {
  logTaskTimer(&ts_high, "CommManager", "High");
   return;
}
