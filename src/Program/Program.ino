//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

////Task Declarations
Scheduler ts_high, ts_low;
//
//High Priority (machine control) tasks
//
//Watchdog Task
void initWatchdog();
Task tskWatchdog(	WATCHDOG_TIMEOUT / 2, TASK_FOREVER,     	&initWatchdog,         			&ts_high,   true);

//Hardware Manager - Initialization of P1AM IO modules, cyclic IO Read/Write, Power removal handling
void initHardwareManager();
Task tskHardwareManager(		100,      	TASK_FOREVER,     	&initHardwareManager,  			&ts_high,   true);

//Comm Manager - Manage the ethernet connections for the various other programs that rely on it
void initCommManager();
Task tskCommManager(   			500,      	TASK_FOREVER,     	&initCommManager,      			&ts_high,   true);

//ATO:  Auto top-off system
void initATO();
Task tskATO(           			100,      	TASK_FOREVER,    	&initATO,              			&ts_high,   true);

//AutoFeeders
void initAutoFeeders();
Task tskAutoFeeders(       		100,      	TASK_FOREVER,    	&initAutoFeeders,           	&ts_high,   true);

//DosePumps
void initDosePumps();
Task tskDosePumps(       		100,      	TASK_FOREVER,    	&initDosePumps,            		&ts_high,   true);

//Lights
void initLights();
Task tskLights(       			100,      	TASK_FOREVER,    	&initLights,            		&ts_high,   true);

////Alarms: Warnings, Faults, Messages that require operator attention, Scheduling of AutoFeeders, dosepumps, etc.
void initAlarms();
Task tskAlarms(             	100,      	TASK_FOREVER,    	&initAlarms,            		&ts_high,   true);

//Power Equipment - Pumps, Powerheads, Wavemakers, etc.
void initPowerEquipment();
Task tskPowerEquipment(         100,      	TASK_FOREVER,    	&initPowerEquipment,        	&ts_high,   true);

//ProbeMonitor - Manage the calculations, filtering, scaling of the various analog probes
void initProbeMonitor();
Task tskProbeMonitor(       	250,      	TASK_FOREVER,    	&initProbeMonitor,      		&ts_high,   true);

//
//Low-Priority Tasks
//
//ThingsBoard_Telemetry - Connect with the ThingsBoard platform and sample onboard data for logging
void initThingsBoard_Telemetry();
Task tskThingsBoard_Telemetry(	100,        TASK_FOREVER,    	&initThingsBoard_Telemetry,		&ts_low,   	true);

//ThingsBoard_RPC - Connect with the ThingsBoard platform and manage RPC callbacks
void initThingsBoard_RPC();
Task tskThingsBoard_RPC(     	25,   		TASK_FOREVER,    	&initThingsBoard_RPC,      		&ts_low,   	true);

//NTP - Manage sync'ing the system time
//with the NIST time server
void initNTP();
Task tskNTP(                	50,   		TASK_FOREVER,    	&initNTP,             			&ts_low,   	true);

//Storage Manager - Manage the SD card, logging, scheduler functions
void initStorageManager();
Task tskStorageManager(     	100,   		TASK_FOREVER,    	&initStorageManager,  			&ts_low,  	true);

//Execution
void setup() {
  Serial.begin(115200);               			//Initialize serial communication
  while (!Serial && !Serial.available()) {}   //Wait for Serial Port to be opened and comm. active (use one or the other)
//  while (!Serial) {}                  			//Wait for Serial Port to be opened (use one or the other)
  Log.begin			(LOG_LEVEL_VERBOSE, &Serial);
  Log.setPrefix		(printTimestamp);
  Log.setSuffix		(printNewline);
  Log.notice		("Initialized Serial Communications");

  ts_low.setHighPriorityScheduler(&ts_high);
}

void loop() {
  ts_low.execute();
  logScheduler(&ts_low);
  logScheduler(&ts_high);
}

void printNewline(Print* _logOutput) {
  _logOutput->print('\n');
}

void printTimestamp(Print* _logOutput) {
  char c[12];
  int m = sprintf(c, "%10lu ", millis());
  _logOutput->print(c);
}
