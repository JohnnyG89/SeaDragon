//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

////Task Declarations
//StatusRequest measure;
Scheduler ts_high, ts_low;

//
//High Priority (machine control) tasks
//

//Watchdog Task
void initWatchdog();
void cyclicWatchdog();
Task tskWatchdog(           WATCHDOG_TIMEOUT / 2,     TASK_FOREVER,    &initWatchdog,          &ts_high,   true);

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
Task tskMQTT(               LOW_PRIORITY_CYCLE_TIME,   TASK_FOREVER,    &initMQTT,             &ts_low,   true);

//ThingsBoard - Connect with the ThingsBoard platform
void initThingsBoard();
void cyclicThingsBoard();
Task tskThingsBoard(        LOW_PRIORITY_CYCLE_TIME,   TASK_FOREVER,    &initThingsBoard,      &ts_low,   true);

//NTP - Manage sync'ing the system time
//with the NIST time server
void initNTP();
void cyclicNTP();
Task tskNTP(                LOW_PRIORITY_CYCLE_TIME,   TASK_FOREVER,    &initNTP,              &ts_low,   true);

//Storage Manager - Manage the SD card
void initStorageManager();
void cyclicStorageManager();
Task tskStorageManager(     LOW_PRIORITY_CYCLE_TIME,   TASK_FOREVER,    &initStorageManager,    &ts_low,  true);

//TODO: Task Timeouts? (task.setTimeout())?

//Execution
void setup() {
  Serial.begin(115200);               //Initialize serial communication at 0.5M bits per second
  while (!Serial) {}                  //Wait for Serial Port to be opened
  prglog("Initialized Serial Communications");

//  setSyncProvider(rtc.now());       //TODO: Do we need to actually call this?  Only put it here bc it was in one of the Chronos examples
  prglog("Starting Task Timers");
  //TODO: Test, not 100% sure how this works w/ multiple task timers
  ts_high.startNow();
  ts_low.startNow();

  prglog("Setting High Priority Scheduler...");
  ts_low.setHighPriorityScheduler(&ts_high);

  //TODO: Make this call? Not sure if it's also going to enable the tasks disabled above^^^
//  ts_low.enableAll(true); // this will recursively enable the higher priority tasks as well
}

void loop() {
  ts_low.execute();
  logScheduler(&ts_low);
}
