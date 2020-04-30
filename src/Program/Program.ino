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
Task tskThingsBoard(        2000,   TASK_FOREVER,    &initThingsBoard,      &ts_low,   true);

//NTP - Manage sync'ing the system time
//with the NIST time server
void initNTP();
void cyclicNTP();
Task tskNTP(                50,   TASK_FOREVER,    &initNTP,              &ts_low,   true);

//Storage Manager - Manage the SD card
void initStorageManager();
void cyclicStorageManager();
Task tskStorageManager(     100,   TASK_FOREVER,    &initStorageManager,    &ts_low,  true);

// Available levels are:
// LOG_LEVEL_SILENT, LOG_LEVEL_FATAL, LOG_LEVEL_ERROR, LOG_LEVEL_WARNING, LOG_LEVEL_NOTICE, LOG_LEVEL_TRACE, LOG_LEVEL_VERBOSE

//Execution
void setup() {
  Serial.begin(115200);               //Initialize serial communication at 0.5M bits per second
  while (!Serial && !Serial.available()) {}                  //Wait for Serial Port to be opened
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.setSuffix(printNewline);
  Log.notice("Initialized Serial Communications");

  ts_low.startNow();
  ts_low.setHighPriorityScheduler(&ts_high);
}

void loop() {
  ts_low.execute();
  //  logScheduler(&ts_low);
}

void printNewline(Print* _logOutput) {
  _logOutput->print('\n');
}

/**
 * Logging is a helper class to output informations over
 * RS232. If you know log4j or log4net, this logging class
 * is more or less similar ;-) <br>
 * Different loglevels can be used to extend or reduce output
 * All methods are able to handle any number of output parameters.
 * All methods print out a formated string (like printf).<br>
 * To reduce output and program size, reduce loglevel.
 * 
 * Output format string can contain below wildcards. Every wildcard
 * must be start with percent sign (\%)
 * 
 * ---- Wildcards
 * 
 * %s  replace with an string (char*)
 * %c replace with an character
 * %d replace with an integer value
 * %l replace with an long value
 * %x replace and convert integer value into hex
 * %X like %x but combine with 0x123AB
 * %b replace and convert integer value into binary
 * %B like %x but combine with 0b10100011
 * %t replace and convert boolean value into "t" or "f"
 * %T like %t but convert into "true" or "false"
 * 
 * ---- Loglevels
 * 
 * 0 - LOG_LEVEL_SILENT     no output
 * 1 - LOG_LEVEL_FATAL      fatal errors
 * 2 - LOG_LEVEL_ERROR      all errors
 * 3 - LOG_LEVEL_WARNING    errors and warnings
 * 4 - LOG_LEVEL_NOTICE     errors, warnings and notices
 * 5 - LOG_LEVEL_TRACE      errors, warnings, notices, traces
 * 6 - LOG_LEVEL_VERBOSE    all
 */

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
//Scheduler
//Difference in cooperative and collaborative task schedulers?
