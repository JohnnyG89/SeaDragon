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
  prglog("StorageManager::Initializing Storage Manager...");
//  return;

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    prglog("StorageManager::Unable to initialize SD Card");
  }
  else {
    prglog("StorageManager::SD Card Initialized");
  }

  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      prglog("StorageManager::Card Type: SD1");
      break;
    case SD_CARD_TYPE_SD2:
      prglog("StorageManager::Card Type: SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      prglog("StorageManager::Card Type: SDHC");
      break;
    default:
      prglog("StorageManager::Card Type: Unknown");
  }

  SD.begin();

  if (!SD.exists(LOG_FILE_NAME)) {
    Serial.println("StorageManager::Data File Doesn't exist");
    File dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
    dataFile.write("hello");
    dataFile.close();
    delay(100);
  }

  tskStorageManager.setCallback(&cyclicStorageManager);

  prglog("StorageManager::Initialized Storage Manager");
}

void cyclicStorageManager(void) {
  logTaskTimer("StorageManager");
  tskStorageManager.disable();
}

void LogEntry(String s) {
  Serial.println(s);

  if (_DUMP_TO_SD) {
    dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
    if (dataFile) {
      dataFile.println(s);
      dataFile.close();
    } else {
      Serial.println("StorageManager::Error Opening Data File");
    }
  }
}

void logTaskTimer(String TaskName) {
#if _DEBUG
  Scheduler &s = Scheduler::currentScheduler();
  Task &pTask = s.currentTask();
  if (pTask.getStartDelay() > 2500) {
    prglog(String(TaskName + "::" + String(pTask.getId()) + " Task Delay of " + String(pTask.getStartDelay())).c_str());
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
  prglog(PrintString.c_str());


#endif
}

void logScheduler(Scheduler* pscheduler) {
#if _DEBUG
  if (pscheduler->isOverrun()) {
    prglog(String("Scheduler Overrun").c_str());
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
    prglog(PrintString.c_str());
  }
#endif
  pscheduler->cpuLoadReset();
}
