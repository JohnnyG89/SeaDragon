//                      ____                                                                  _____       ______                   
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          | 
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      | 
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  | 
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``| 
//                                                                                                                                 
//                                                          Reef On


#define _DUMP_TO_SD false
#define LOG_FILE_NAME "Log.txt"
#define _DEBUG_VERBOSE false

#include <SD.h>

//SD Card Related Declarators
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = SDCARD_SS_PIN;
File dataFile;

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

void logTaskTimer(Scheduler* pscheduler, String TaskName) {
  if (_DEBUG || _DEBUG_VERBOSE) {
    Task* pTask = pscheduler->getCurrentTask();
    if (pTask->getStartDelay() > _BASE_CYCLE_TIME) {
      log(String(TaskName + " Task Delay of " + String(pTask->getStartDelay())).c_str());
    }
    if (_DEBUG_VERBOSE) {
      String TaskNameString, TaskInterval, StartDelay, Overrun, RunCounter, IterationCount, TimeUntilNextRun, PrintString;
      TaskNameString = String(TaskName);
      //TaskStart = String("Task started at: " + String(rtc.()) + ":"  + String(minute()) + ":" + String(second()) + ":" + String(millis()));
      TaskInterval = String("Interval: " + String(pTask->getInterval()));
      StartDelay = String("Start Delay: " + String(pTask->getStartDelay()));
      Overrun = String("Overrun: " + String(pTask->getOverrun()));
      RunCounter = String("Run Counter: " + String(pTask->getRunCounter()));
      IterationCount = String("Iteration Count: " + String(pTask->getIterations()));
      TimeUntilNextRun = String("Time Until Nex Run: " + String(pscheduler->timeUntilNextIteration(*pTask)));
      PrintString = String(TaskNameString + ":" + ", " + TaskInterval + ", " + StartDelay + ", " + Overrun + ", " + RunCounter + ", " + IterationCount + ";");
      log(PrintString.c_str());
    }
  }
}

void logScheduler(Scheduler* pscheduler) {
  if (_DEBUG || _DEBUG_VERBOSE) {
    if (pscheduler->isOverrun()) {
      log("Scheduler Overrun");
    }
  }
  if (_DEBUG_VERBOSE) {
    String CPULoad_idle, CPULoad_cycle, CPULoad_total, SchedulerIsOverran, PrintString;
    SchedulerIsOverran = String("Overrun Status: " + String(pscheduler->isOverrun()));
    CPULoad_idle = String("CPU Idle: " + String(pscheduler->getCpuLoadIdle()));
    CPULoad_cycle = String("CPU Cycle: " + String(pscheduler->getCpuLoadCycle()));
    CPULoad_total = String("CPU Total: " + String(pscheduler->getCpuLoadTotal()));
    PrintString = String("Scheduler Completed Pass." + CPULoad_idle + "," + CPULoad_cycle + "," + CPULoad_total + ", " + SchedulerIsOverran);
    log(PrintString.c_str());
  }
  pscheduler->cpuLoadReset();
}
