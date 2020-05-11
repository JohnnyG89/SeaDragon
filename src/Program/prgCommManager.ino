//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static Logging  comLog;

static byte     mac[]     = {0x60, 0x52, 0xD0, 0x06, 0x68, 0x2E};

void initCommManager(void) {
  comLog.begin(LOG_LEVEL_NOTICE, &Serial);
  comLog.setPrefix(printTimestamp);
  comLog.setSuffix(printNewline);
  comLog.notice("CommManager:: Initialized CommManager Log ::");
  comLog.trace("Beginning CommManager Initialization");

  Ethernet.init(5);   //CS pin for P1AM-ETH
  Ethernet.begin(mac);  // Get IP from DHCP

  //  //TODO
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    comLog.fatal("CommManager:: Ethernet shield was not found.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5100) {
    comLog.warning("CommManager:: W5100 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5200) {
    comLog.warning("CommManager:: W5200 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5500) {
    comLog.warning("CommManager:: W5500 Ethernet controller detected.");
  }

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicCommManager);
  gEthernetConnectionActive = true;

  comLog.trace("CommManager::Finished CommManager Initialization");
}

void cyclicCommManager(void) {
  logTaskTimer("CommManager", &Scheduler::currentScheduler().currentTask());
  Ethernet.maintain();
}
