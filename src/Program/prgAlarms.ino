//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

CronId id;

void initAlarms(void) {
  Log.trace("Alarms:: Initializing Alarms...");

  if (!_ENABLE_ALARMS) {
    Scheduler::currentScheduler().currentTask().disable();
    Log.warning("Alarms: :Disabling Alarms Task..");
    return;
  }

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicAlarms);

  Log.trace("Alarms:: Initialized Alarms");
}

void cyclicAlarms(void) {
  logTaskTimer("Alarms");

  if (!gTimeSet) {
    Log.warning("Alarms::Time was unset.  Returning to Initialization Routine");
    Scheduler::currentScheduler().currentTask().setCallback(&initAlarms);
    return;
  }
}
