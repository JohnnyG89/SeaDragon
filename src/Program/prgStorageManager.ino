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
  prglog("Initializing Storage Manager...");

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    prglog("Unable to initialize SD Card");
  }
  else {
    prglog("SD Card Initialized");
  }

  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      prglog("Card Type: SD1");
      break;
    case SD_CARD_TYPE_SD2:
      prglog("Card Type: SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      prglog("Card Type: SDHC");
      break;
    default:
      prglog("Card Type: Unknown");
  }

  SD.begin();

  while (!SD.exists(LOG_FILE_NAME)) {
    Serial.println("Data File Doesn't exist");
    File dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
    dataFile.write("hello");
    dataFile.close();
    delay(100);
  }

  tskStorageManager.setCallback(&cyclicStorageManager);

  prglog("Initialized Storage Manager");
}

void cyclicStorageManager(void) {
  logTaskTimer("StorageManager");
}

void LogEntry(String s) {
  Serial.println(s);

  if (_DUMP_TO_SD) {
    dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
    if (dataFile) {
      dataFile.println(s);
      dataFile.close();
    } else {
      Serial.println("Error Opening Data File");
    }
  }
}

//void logTaskTimer(Scheduler* pscheduler, String TaskName, String Priority) {
//  Scheduler &s = Scheduler::currentScheduler();
//  if (_DEBUG || _DEBUG_VERBOSE) {
//    Task* pTask = s.getCurrentTask();
//    if (pTask->getStartDelay() > BASE_CYCLE_TIME) {
//      prglog(String(TaskName + " Task Delay of " + String(pTask->getStartDelay())).c_str());
//    }
//    if (_DEBUG_VERBOSE) {
//      String TaskNameString, TaskInterval, StartDelay, Overrun, RunCounter, IterationCount, TimeUntilNextRun, PrintString;
//      TaskNameString = String(TaskName);
//      //TaskStart = String("Task started at: " + String(rtc.()) + ":"  + String(minute()) + ":" + String(second()) + ":" + String(millis()));
//      TaskInterval = String("Interval: " + String(pTask->getInterval()));
//      StartDelay = String("Start Delay: " + String(pTask->getStartDelay()));
//      Overrun = String("Overrun: " + String(pTask->getOverrun()));
//      RunCounter = String("Run Counter: " + String(pTask->getRunCounter()));
//      IterationCount = String("Iteration Count: " + String(pTask->getIterations()));
//      TimeUntilNextRun = String("Time Until Nex Run: " + String(pscheduler->timeUntilNextIteration(*pTask)));
//      PrintString = String("Priority: " + Priority + ":" + TaskNameString + ":" + ", " + TaskInterval + ", " + StartDelay + ", " + Overrun + ", " + RunCounter + ", " + IterationCount + ";");
//      prglog(PrintString.c_str());
//    }
//  }
//}

void logTaskTimer(String TaskName) {
  Scheduler &s = Scheduler::currentScheduler();
  
  if (_DEBUG || _DEBUG_VERBOSE) {
//    Task* pTask = s.getCurrentTask();
    Task &pTask = s.currentTask();
    if (pTask.getStartDelay() > BASE_CYCLE_TIME) {
      prglog(String(TaskName + "::" + String(pTask.getId()) + " Task Delay of " + String(pTask.getStartDelay())).c_str());
    }
    if (_DEBUG_VERBOSE) {
      String TaskNameString, TaskInterval, StartDelay, Overrun, RunCounter, IterationCount, TimeUntilNextRun, PrintString;
      TaskNameString = String(TaskName);
      //TaskStart = String("Task started at: " + String(rtc.()) + ":"  + String(minute()) + ":" + String(second()) + ":" + String(millis()));
      TaskInterval = String("Interval: " + String(pTask.getInterval()));
      StartDelay = String("Start Delay: " + String(pTask.getStartDelay()));
      Overrun = String("Overrun: " + String(pTask.getOverrun()));
      RunCounter = String("Run Counter: " + String(pTask.getRunCounter()));
      IterationCount = String("Iteration Count: " + String(pTask.getIterations()));
      TimeUntilNextRun = String("Time Until Nex Run: " + String(s.timeUntilNextIteration(pTask)));
      PrintString = String(TaskNameString + ":" + ", " + TaskInterval + ", " + StartDelay + ", " + Overrun + ", " + RunCounter + ", " + IterationCount + ";");
      prglog(PrintString.c_str());
    }
  }
}

void logScheduler(Scheduler* pscheduler) {
  //TODO:
  //  Scheduler &s = Scheduler::currentScheduler();
  //  Task &t = s.currentTask();


  if (_DEBUG || _DEBUG_VERBOSE) {
    if (pscheduler->isOverrun()) {
      prglog(String("Scheduler Overrun").c_str());
    }
  }
  if (_DEBUG_VERBOSE) {
    if (pscheduler->getCpuLoadTotal() > 500) {
      String CPULoad_idle, CPULoad_cycle, CPULoad_total, SchedulerIsOverran, PrintString;
      SchedulerIsOverran = String("Overrun Status: " + String(pscheduler->isOverrun()));
      CPULoad_idle = String("CPU Idle: " + String(pscheduler->getCpuLoadIdle()));
      CPULoad_cycle = String("CPU Cycle: " + String(pscheduler->getCpuLoadCycle()));
      CPULoad_total = String("CPU Total: " + String(pscheduler->getCpuLoadTotal()));
      PrintString = String("Scheduler Completed Pass." + CPULoad_idle + "," + CPULoad_cycle + "," + CPULoad_total + ", " + SchedulerIsOverran);
      prglog(PrintString.c_str());
    }


  }
  pscheduler->cpuLoadReset();
}
