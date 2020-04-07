
//                      ____                                                                  _____       ______                   
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          | 
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      | 
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  | 
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``| 
//                                                                                                                                 
//                                                          Reef On

//User Program pre-compiler flags
#define _DEBUG true
#define _ENABLE_WATCHDOG true
#define _WATCHDOG_TIMEOUT 10000
#define _ENABLE_COMMS true

//Set task timer, logging settings in debug vs. production/deployment
#if _DEBUG
#define log(s) {LogEntry(F(s));}
#define _BASE_CYCLE_TIME 50
#else
#define log(s)
#define _BASE_CYCLE_TIME 25
#endif

#define _HIGH_LOW_PRIORITY_MULTIPLIER 2

//TaskScheduler Compiler Flags
#define _TASK_TIMECRITICAL          // Enable monitoring scheduling overruns
#define _TASK_SLEEP_ON_IDLE_RUN     // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST        // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
//#define _TASK_WDT_IDS             // Compile with support for wdt control points and task ids
#define _TASK_LTS_POINTER           // Compile with support for local task storage pointer
#define _TASK_PRIORITY              // Support for layered scheduling priority
// #define _TASK_MICRO_RES          // Support for microsecond resolution
// #define _TASK_STD_FUNCTION       // Support for std::function (ESP8266 ONLY)
#define _TASK_DEBUG                 // Make all methods and variables public for debug purposes
// #define _TASK_INLINE             // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
#define _TASK_TIMEOUT               // Support for overall task timeout
// #define _TASK_OO_CALLBACKS       // Support for callbacks via inheritance
// #define _TASK_DEFINE_MILLIS      // Force forward declaration of millis() and micros() "C" style
#define _TASK_EXPOSE_CHAIN          // Methods to access tasks in the task chain

//Pre-built libraries
#include <TaskSchedulerSleepMethods.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskScheduler.h>

bool inDiscrete_1, inWaterLevelSensor_1, inWaterLevelSensor_2, inWaterLevelSensor_3;
bool outATOPump, outSkimmer, outPowerhead_1, outPowerhead_2;
bool gTimeSet=false;

////Task Declarations
StatusRequest measure;
Scheduler ts;

//
//High Priority (machine control) tasks
//

//Watchdog Task
void initWatchdog();
void cyclicWatchdog();
Task tskWatchdog(_WATCHDOG_TIMEOUT/2, TASK_FOREVER, &initWatchdog, &ts, _ENABLE_WATCHDOG);

//Hardware Manager - Initialization of P1AM IO modules
void initHardwareManager();
void cyclicHardwareManager();
Task tskHardwareManager(_BASE_CYCLE_TIME, TASK_FOREVER, &initHardwareManager, &ts, true);

//IO Manager - Mapping program inputs/outputs to their IO modules
void initIOManager();
void cyclicIOManager();
Task tskIOManager(_BASE_CYCLE_TIME, TASK_FOREVER, &initIOManager, &ts, true);

//Example Task : Set the onboard LED to the state of the switch
void initMyFirstTask();
void cyclicMyFirstTask();
Task tskMyFirstTask(_BASE_CYCLE_TIME, TASK_FOREVER, &initMyFirstTask, &ts, true);

//ATO:  Auto top-off system
void initATO();
void cyclicATO();
Task tskATO(_BASE_CYCLE_TIME, TASK_FOREVER, &initATO, &ts, true);

//Alarms: Warnings, Faults, Messages that require operator attention
void initAlarms();
void cyclicAlarms();
Task tskAlarms(_BASE_CYCLE_TIME, TASK_FOREVER, &initAlarms, &ts, true);

//
//Low Priority tasks
//

//MQTT - Manage all of the MQTT handshaking, ethernet connectivity, etc.
void initMQTT();
void cyclicMQTT();
Task tskMQTT(_BASE_CYCLE_TIME*_HIGH_LOW_PRIORITY_MULTIPLIER, TASK_FOREVER, &initMQTT, &ts, _ENABLE_COMMS);

//NTP - Manage sync'ing the system time with the NIST time server
void initNTP();
void cyclicNTP();
Task tskNTP(_BASE_CYCLE_TIME*_HIGH_LOW_PRIORITY_MULTIPLIER, TASK_FOREVER, &initNTP, &ts, _ENABLE_COMMS);

//Storage Manager - Manage the SD card
void initStorageManager();
void cyclicStorageManager();
Task tskStorageManager(_BASE_CYCLE_TIME*_HIGH_LOW_PRIORITY_MULTIPLIER, TASK_FOREVER, &initStorageManager, &ts, _ENABLE_COMMS);


//Execution
void setup() {
  Serial.begin(115200);     //Initialize serial communication at 0.5M bits per second 
  while (!Serial) {}        //Wait for Serial Port to be opened
  log("Initialized Serial Communications");
}

void loop() {
  ts.execute();
//  logScheduler(&ts);
}
