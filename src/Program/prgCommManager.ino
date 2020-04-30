//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"


byte mac[] = {0x60, 0x52, 0xD0, 0x06, 0x68, 0x2E};

void initCommManager(void) {
  Log.trace("Beginning CommManager Initialization");
  
  Ethernet.init(5);   //CS pin for P1AM-ETH
  Ethernet.begin(mac);  // Get IP from DHCP 
  Serial.println(String(Ethernet.localIP()));

//  //TODO
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Log.notice("CommManager::Ethernet shield was not found.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5100) {
    Log.notice("CommManager::W5100 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5200) {
    Log.notice("CommManager::W5200 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5500) {
    Log.notice("CommManager::W5500 Ethernet controller detected.");
  }
  
  Scheduler::currentScheduler().currentTask().setCallback(&cyclicCommManager);

  gEthernetConnectionActive=true;

  Log.trace("CommManager::Finished CommManager Initialization");
}

void cyclicCommManager(void) {
  logTaskTimer("CommManager");

//  auto link = Ethernet.linkStatus();
//  switch (link) {
//    case Unknown:
//      prglog("CommManager::Unknown Link Status");
//      break;
//    case LinkON:
////      Serial.println("ON");
//      break;
//    case LinkOFF:
////      tskCommManager.setCallback(&initCommManager);
////      prglog("CommManager::Ethernet Link Down.  Returning to CommManager init() routine");
//      break;
//  }
//
//  if(!client.connected()){
//    prglog("CommManager::Not connected.  Returning to comm manager initialization");
//    tskCommManager.setCallback(&initCommManager);
//  }

  Ethernet.maintain();
}
