//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

void initWatchdog(void) {
  Log.trace("Watchdog:: Initializing Watchdog...");

  if (_ENABLE_WATCHDOG) {
    Log.notice("Watchdog:: Enabling Watchdog Timer");
    P1.configWD(10000, HOLD);
    P1.startWD();
  } else {
    Log.warning("Watchdog:: Not Enabling Watchdog Timer");
  }
  
  Scheduler::currentScheduler().currentTask().setCallback(&cyclicWatchdog);
  Log.trace("Watchdog:: Initialized Watchdog.");
}

void cyclicWatchdog(void) {
  logTaskTimer("Watchdog");

  if (_ENABLE_WATCHDOG) {
    Log.verbose("Watchdog:: Pet Watchdog");
    P1.petWD();
  } else {
    Scheduler::currentScheduler().currentTask().disable();
  }
}
