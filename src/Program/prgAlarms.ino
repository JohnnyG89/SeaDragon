//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

void initAlarms(void) {

  if (!_ENABLE_ALARMS) {
    tskAlarms.disable();
    prglog("Alarms::Disabling Alarms Task..");
    return;
  }

  prglog("Alarms::Initializing Alarms...");

  tskAlarms.setCallback(&cyclicAlarms);

  prglog("Alarms::Initialized Alarms");
}

void cyclicAlarms(void) {
  logTaskTimer("Alarms");

  if (!gTimeSet) {
    prglog("Alarms::Time was unset.  Returning to Initialization Routine");
    tskAlarms.setCallback(&initAlarms);
    return;
  }

}
