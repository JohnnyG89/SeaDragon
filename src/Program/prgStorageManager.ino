//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

//SD Card Related Declarators
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = SDCARD_SS_PIN;
File dataFile;

void initStorageManager(void) {
  Log.trace("StorageManager:: Initializing Storage Manager...");

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Log.error("StorageManager:: Unable to initialize SD Card");
  }
  else {
    Log.verbose("StorageManager:: SD Card Initialized");
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
  
  Scheduler::currentScheduler().currentTask().setCallback(&cyclicStorageManager);

  Log.trace("StorageManager:: Initialized Storage Manager");
}

void cyclicStorageManager(void) {
  logTaskTimer("StorageManager");
  Scheduler::currentScheduler().currentTask().disable();
}
//
//void LogEntry(String s) {
//  Serial.println(s);
//
//  if (_DUMP_TO_SD) {
//    dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
//    if (dataFile) {
//      dataFile.println(s);
//      dataFile.close();
//    } else {
//      Serial.println("StorageManager::Error Opening Data File");
//    }
//  }
//}

void logTaskTimer(String TaskName) {
#if _DEBUG
  Scheduler &s = Scheduler::currentScheduler();
  Task &pTask = s.currentTask();
  if (pTask.getStartDelay() > 1000) {
//    Log.fatal("StorageManager:: Task %s :: id %d :: Delay %d ms ::", TaskName, Scheduler::currentScheduler().currentTask().getId(), Scheduler::currentScheduler().currentTask().getStartDelay());
  }
#endif

#if _DEBUG_VERBOSE
  String TaskStart, TaskInterval, StartDelay, Overrun, RunCounter, IterationCount, TimeUntilNextRun, PrintString;
  //      TaskNameString = String(TaskName);
  //      prglog(String("Time: " + String(rtc.getMonth()) + "/" + String(rtc.getDay()) + "/" + String(rtc.getYear()) + "-" + String(rtc.getHours()) + ":" + String(rtc.getMinutes()) + ":" + String(rtc.getSeconds())).c_str());
  TaskStart = String("Task started at: " + String(rtc.getHours()) + ":"  + String(rtc.getMinutes()) + ":" + String(rtc.getSeconds()));
  TaskInterval = String("Interval: " + String(pTask.getInterval()));
  StartDelay = String("Start Delay: " + String(pTask.getStartDelay()));
  Overrun = String("Overrun: " + String(pTask.getOverrun()));
  RunCounter = String("Run Counter: " + String(pTask.getRunCounter()));
  IterationCount = String("Iteration Count: " + String(pTask.getIterations()));
  TimeUntilNextRun = String("Time Until Nex Run: " + String(s.timeUntilNextIteration(pTask)));
  PrintString = String(TaskName + ":" + ", " + TaskInterval + ", " + StartDelay + ", " + Overrun + ", " + RunCounter + ", " + IterationCount + ";");
//  prglog(PrintString.c_str());


#endif
}

void logScheduler(Scheduler* pscheduler) {
#if _DEBUG
  if (pscheduler->isOverrun()) {
    Log.fatal(String("Scheduler Overrun").c_str());
  }
#endif

#if _DEBUG_VERBOSE
  if (pscheduler->getCpuLoadTotal() > 500) {
    String CPULoad_idle, CPULoad_cycle, CPULoad_total, SchedulerIsOverran, PrintString;
    SchedulerIsOverran = String("Overrun Status: " + String(pscheduler->isOverrun()));
    CPULoad_idle = String("CPU Idle: " + String(pscheduler->getCpuLoadIdle()));
    CPULoad_cycle = String("CPU Cycle: " + String(pscheduler->getCpuLoadCycle()));
    CPULoad_total = String("CPU Total: " + String(pscheduler->getCpuLoadTotal()));
    PrintString = String("Scheduler Completed Pass." + CPULoad_idle + "," + CPULoad_cycle + "," + CPULoad_total + ", " + SchedulerIsOverran);
//    prglog(PrintString.c_str());
  }
#endif
  pscheduler->cpuLoadReset();
}
