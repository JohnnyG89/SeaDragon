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
Task tskWatchdog(           5000,     TASK_FOREVER,       &initWatchdog,         &ts_high,   true);

//Hardware Manager - Initialization of P1AM IO modules
void initHardwareManager();
void cyclicHardwareManager();
Task tskHardwareManager(    200,      TASK_FOREVER,      &initHardwareManager,  &ts_high,   true);

//Comm Manager - Manage the ethernet connections for the various other programs that rely on it
void initCommManager();
void cyclicCommManager();
Task tskCommManager(        500,      TASK_FOREVER,      &initCommManager,      &ts_high,   true);

//ATO:  Auto top-off system
void initATO();
void cyclicATO();
Task tskATO(                200,      TASK_FOREVER,    &initATO,              &ts_high,   true);

//TODO: If you're going to use Chronos, it's incompatible with RTCZero
////Alarms: Warnings, Faults, Messages that require operator attention
void initAlarms();
void cyclicAlarms();
Task tskAlarms(             500,      TASK_FOREVER,    &initAlarms,            &ts_high,   true);

//Alarms: Warnings, Faults, Messages that require operator attention
void initProbeMonitor();
void cyclicProbeMonitor();
Task tskProbeMonitor(       200,      TASK_FOREVER,    &initProbeMonitor,      &ts_high,   true);

//
//Low Priority tasks
//
//
////MQTT - Manage all of the MQTT handshaking, ethernet connectivity, etc.
//void initMQTT();
//void cyclicMQTT();
//Task tskMQTT(               LOW_PRIORITY_CYCLE_TIME,   TASK_FOREVER,    &initMQTT,             &ts_low,   true);

//ThingsBoard - Connect with the ThingsBoard platform
void initThingsBoard();
void cyclicThingsBoard();
Task tskThingsBoard(        2500,   TASK_FOREVER,    &initThingsBoard,      &ts_low,   true);

//NTP - Manage sync'ing the system time
//with the NIST time server
void initNTP();
void cyclicNTP();
Task tskNTP(                50,   TASK_FOREVER,    &initNTP,              &ts_low,   true);

//Storage Manager - Manage the SD card
void initStorageManager();
void cyclicStorageManager();
Task tskStorageManager(     100,   TASK_FOREVER,    &initStorageManager,    &ts_low,  true);

//Execution
void setup() {
  Serial.begin(115200);               //Initialize serial communication at 0.5M bits per second
  while (!Serial) {}                  //Wait for Serial Port to be opened
  prglog("Initialized Serial Communications");
//  prglog("Starting Task Timers");
  //TODO: Test, not 100% sure how this works w/ multiple task timers
  //TODO: Call ts.init()?
  ts_high.startNow();
  ts_low.startNow();

//  prglog("Setting High Priority Scheduler...");
  ts_low.setHighPriorityScheduler(&ts_high);
}

void loop() {
  ts_low.execute();
//  logScheduler(&ts_low);
}

//GENERAL TODO:
//CNC-like plotter for feeding corals lol
//Status Indication on the front LED
//Ring-buffer
//switches may need debouncing
//blinking pattern of the lights
//ArduinoUnit unittesting library
//Scheduler &s = Scheduler::currentScheduler();
//  Task &t = s.currentTask();
//libs
//AD_Sensors
//Adafruit ZeroTimer Library
//ArduinoLog
//dbg-trace
//Debug
//ESP Logger
//JsonLogger
//Log
//Logger
//M2M Solutions Logger Library
//Unified Log
//Arduino_DebugUtils
//Scheduler
//Difference in cooperative and collaborative task schedulers?
