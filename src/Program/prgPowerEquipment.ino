//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static        Logging    peqLog;

void initPowerEquipment(void) {
  peqLog.begin(LOG_LEVEL_NOTICE, &Serial);
  peqLog.setPrefix(printTimestamp);
  peqLog.setSuffix(printNewline);
  peqLog.notice("PowerEquipment:: Initialized PowerEquipment Log ::");
  peqLog.trace("PowerEquipment:: Beginning PowerEquipment Initialization ::");

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicPowerEquipment);
  peqLog.trace("PowerEquipment:: Finished PowerEquipment Initialization ::");
}

void cyclicPowerEquipment(void) {
logTaskTimer("PowerEquipment", &Scheduler::currentScheduler().currentTask());
}