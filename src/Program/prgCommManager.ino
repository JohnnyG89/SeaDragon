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

  //TODO: Move the Ethernet intialization here from the MQTT task for conciseness
  //TODO: Set IP address?

  //NOTE: If you need to renew a DHCP lease, use Ethernet.maintain();

//  
//  Ethernet.begin(mac, ip);
//  IPAddress newIp(10, 0, 0, 178);
//  Ethernet.setLocalIP(newIp);  // change the IP address
  
  Ethernet.init(5);   //CS pin for P1AM-ETH
  Ethernet.begin(mac);  // Get IP from DHCP 
  Serial.println(String(Ethernet.localIP()));

//  //TODO
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    prglog("CommManager::Ethernet shield was not found.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5100) {
    prglog("CommManager::W5100 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5200) {
    prglog("CommManager::W5200 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5500) {
    prglog("CommManager::W5500 Ethernet controller detected.");
  }

  tskCommManager.setCallback(&cyclicCommManager);

  gEthernetConnectionActive=true;

  prglog("CommManager::Finished CommManager Initialization");
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
