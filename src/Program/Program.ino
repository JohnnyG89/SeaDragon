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
//This is a basic high/low scheduler.  High priority scheduler always goes first.
//
//Watchdog Task

#if _ENABLE_WATCHDOG
void initWatchdog();
Task tskWatchdog(	WATCHDOG_TIMEOUT / 2, TASK_FOREVER,     &initWatchdog,         			&ts_high,   true);
#endif

//Hardware Manager - Initialization of P1AM IO modules, cyclic IO Read/Write, Power removal handling
void initHardwareManager();
Task tskHardwareManager(		200,      	TASK_FOREVER,     &initHardwareManager,  			&ts_high,   true);

//ATO:  Auto top-off system
void initEquipment();
Task tskEquipment(          200,      	TASK_FOREVER,    	&initEquipment,             &ts_high,   true);

//AutoFeeders
void initAutoFeeders();
Task tskAutoFeeders(       	200,        TASK_FOREVER,    	&initAutoFeeders,           &ts_high,   true);

//Lights
void initLights();
Task tskLights(       			100,      	TASK_FOREVER,    	&initLights,            		&ts_high,   true);

////Alarms: Warnings, Faults, Messages that require operator attention, Scheduling of AutoFeeders, dosepumps, etc.
void initAlarms();
Task tskAlarms(             200,      	TASK_FOREVER,    	&initAlarms,            		&ts_high,   true);

//ProbeMonitor - Manage the calculations, filtering, scaling of the various analog probes
void initProbeMonitor();
Task tskProbeMonitor(       500,      	TASK_FOREVER,    	&initProbeMonitor,      		&ts_high,   true);

//
//Low-Priority Tasks
//Note prgThingsBoard tasks need to be used in an "Either/or" fashion with the prgMQTT task for now.
//...And you get a shiny nickel if you can figure out how to make it happen!

#if _USE_THINGSBOARD

//ThingsBoard_Telemetry - Connect with the ThingsBoard platform and sample onboard data for logging
void initThingsBoard_Telemetry();
Task tskThingsBoard_Telemetry(500,        TASK_FOREVER,    	&initThingsBoard_Telemetry,		&ts_low,   	_USE_THINGSBOARD);

////ThingsBoard_RPC - Connect with the ThingsBoard platform and manage RPC callbacks
void initThingsBoard_RPC();
Task tskThingsBoard_RPC(     	100,   		TASK_FOREVER,    	&initThingsBoard_RPC,      		&ts_low,   	_USE_THINGSBOARD);

#endif

#if _USE_PUBSUB
//MQTT - Connect with the Shiftr.io platform and manage Telemetry, RPC's
void initMQTT();
Task tskMQTT(     	        2000,   		  TASK_FOREVER,     &initMQTT,      		&ts_low,   	_USE_PUBSUB);

void initRPC();
Task tskRPC(     	          100,   		    TASK_FOREVER,     &initRPC,      		&ts_low,   	_USE_PUBSUB);
#endif
//NTP - Manage sync'ing the system time
//with the NIST time server
void initNTP();
Task tskNTP(                	50,   		TASK_FOREVER,    	&initNTP,           &ts_low,   	true);

//Execution
void setup() {
  Serial.begin(115200);               			//Initialize serial communication
  //while (!Serial && !Serial.available()) {}   //Wait for Serial Port to be opened and comm. active (use one or the other), Use for debug if you want to make the program wait here while the serial port opens
  while (!Serial) {}                  			//Wait for Serial Port to be opened (use one or the other).  Will continue whether or not there's a listener.
  Log.begin			(LOG_LEVEL_NOTICE, &Serial);
  Log.setPrefix	(printTimestamp);
  Log.setSuffix	(printNewline);
  Log.notice		("Initialized Serial Communications");

  //SD Card Related Declarators
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  static const int chipSelect = SDCARD_SS_PIN;
  File dataFile;

  Log.trace("StorageManager:: Initializing Storage Manager...");

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Log.error("StorageManager:: Unable to initialize SD Card");
  }
  else {
    Log.notice("StorageManager:: SD Card Initialized");
  }

  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Log.notice("StorageManager:: Card Type: SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Log.notice("StorageManager:: Card Type: SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Log.notice("StorageManager:: Card Type: SDHC");
      break;
    default:
      Log.error("StorageManager:: Card Type: Unknown");
  }

  SD.begin();

  if (!SD.exists(LOG_FILE_NAME)) {
    Log.fatal("StorageManager:: Data File %s Doesn't exist", LOG_FILE_NAME);
    File dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
    dataFile.write("hello");
    dataFile.close();
    delay(100);
  }
  
  Ethernet.init(5);   //CS pin for P1AM-ETH
  Ethernet.begin(mac);  // Get IP from DHCP
  gEthernetConnectionActive = true;

  ts_low.setHighPriorityScheduler(&ts_high);
}

void loop() {
  ts_low.execute();
  Ethernet.maintain();
}

//Random global functions for the logger.
void printNewline(Print* _logOutput) {
  _logOutput->print('\n');
}

void printTimestamp(Print* _logOutput) {
  char c[12];
  int m = sprintf(c, "%10lu ", millis());
  _logOutput->print(c);
}


void logTaskTimer(const char* TaskName, Task* pTask) {
//  if (pTask->getStartDelay() > 100) {
    //    stgLog.notice("%s:: id %d :: Delay %d ms :: Iteration %d ::", TaskName, pTask->getId(), pTask->getStartDelay(), pTask->getRunCounter());
    return;
//  }
//  Log.verbose("%s:: Delay %d ms :: Iteration %d ::", TaskName, pTask->getStartDelay(), pTask->getRunCounter());
}
