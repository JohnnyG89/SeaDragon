//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static Logging wdgLog;

void initWatchdog(void) {
  wdgLog.begin(LOG_LEVEL_NOTICE, &Serial);
  wdgLog.setPrefix(printTimestamp);
  wdgLog.setSuffix(printNewline);
  wdgLog.notice("Watchdog:: Initialized Watchdog Log ::");

  wdgLog.trace("Watchdog:: Initializing Watchdog...");

  if (_ENABLE_WATCHDOG) {
    wdgLog.notice("Watchdog:: Enabling Watchdog Timer");
    P1.configWD(WATCHDOG_TIMEOUT, TOGGLE);
    P1.startWD();
  } else {
    wdgLog.warning("Watchdog:: Not Enabling Watchdog Timer");
  }

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicWatchdog);
  wdgLog.trace("Watchdog:: Initialized Watchdog.");
}

void cyclicWatchdog(void) {
  logTaskTimer("Watchdog", &Scheduler::currentScheduler().currentTask());

  if (_ENABLE_WATCHDOG) {
    wdgLog.verbose("Watchdog:: Pet Watchdog");
    P1.petWD();
  } else {
    Scheduler::currentScheduler().currentTask().disable();
  }
}
