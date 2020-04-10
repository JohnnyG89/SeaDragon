//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"


////Task Declarations
StatusRequest measure;
Scheduler ts_high, ts_low;

//
//High Priority (machine control) tasks
//

//Watchdog Task
void initWatchdog();
void cyclicWatchdog();
Task tskWatchdog(           WATCHDOG_TIMEOUT / 2,     TASK_FOREVER,    &initWatchdog,          &ts_high,   _ENABLE_WATCHDOG);

//Hardware Manager - Initialization of P1AM IO modules
void initHardwareManager();
void cyclicHardwareManager();
Task tskHardwareManager(    BASE_CYCLE_TIME,          TASK_FOREVER,    &initHardwareManager,   &ts_high,   true);

//IO Manager - Mapping program inputs/outputs to their IO modules
void initIOManager();
void cyclicIOManager();
Task tskIOManager(          BASE_CYCLE_TIME,          TASK_FOREVER,    &initIOManager,         &ts_high,   true);

//IO Manager - Mapping program inputs/outputs to their IO modules
void initCommManager();
void cyclicCommManager();
Task tskCommManager(        BASE_CYCLE_TIME,          TASK_FOREVER,    &initCommManager,       &ts_high,   true);

//Example Task : Set the onboard LED to the state of the switch
void initMyFirstTask();
void cyclicMyFirstTask();
Task tskMyFirstTask(        BASE_CYCLE_TIME,          TASK_FOREVER,    &initMyFirstTask,       &ts_high,   true);

//ATO:  Auto top-off system
void initATO();
void cyclicATO();
Task tskATO(                BASE_CYCLE_TIME,          TASK_FOREVER,    &initATO,               &ts_high,   true);

//Alarms: Warnings, Faults, Messages that require operator attention
void initAlarms();
void cyclicAlarms();
Task tskAlarms(             BASE_CYCLE_TIME,          TASK_FOREVER,    &initAlarms,            &ts_high,   true);

//
//Low Priority tasks
//

//MQTT - Manage all of the MQTT handshaking, ethernet connectivity, etc.
void initMQTT();
void cyclicMQTT();
Task tskMQTT(               LOW_PRIORITY_CYCLE_TIME,   TASK_FOREVER,    &initMQTT,              &ts_low,    _ENABLE_COMMS);

//ThingsBoard - Connect with the ThingsBoard platform
void initThingsBoard();
void cyclicThingsBoard();
Task tskThingsBoard(        LOW_PRIORITY_CYCLE_TIME,   TASK_FOREVER,    &initThingsBoard,       &ts_low,    _ENABLE_COMMS);

//NTP - Manage sync'ing the system time
//with the NIST time server
void initNTP();
void cyclicNTP();
Task tskNTP(                LOW_PRIORITY_CYCLE_TIME,   TASK_FOREVER,    &initNTP,               &ts_low,    _ENABLE_COMMS);

//Storage Manager - Manage the SD card
void initStorageManager();
void cyclicStorageManager();
Task tskStorageManager(     LOW_PRIORITY_CYCLE_TIME,   TASK_FOREVER,    &initStorageManager,     &ts_low,   _ENABLE_COMMS);

//Execution
void setup() {
  Serial.begin(115200);     //Initialize serial communication at 0.5M bits per second
  while (!Serial) {}        //Wait for Serial Port to be opened
  prglog("Initialized Serial Communications");
}

void loop() {
  ts_low.setHighPriorityScheduler(&ts_high);
  ts_low.execute();
  logScheduler(&ts_low);
}
