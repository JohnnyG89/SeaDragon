void LogEntry(String s){
  Serial.println(s);

  #ifdef _DUMP_TO_SD
    dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
    if(dataFile){
      dataFile.println(s);
      dataFile.close();
    }else{
      Serial.println("Error Opening Data File");
    }
  #endif
}

void logTaskTimer(Scheduler* pscheduler, String TaskName) {
  #ifdef _DEBUG
    Task* pTask = pscheduler->getCurrentTask();
    String TaskNameString, TaskStart, TaskInterval, StartDelay, Overrun, RunCounter, IterationCount, TimeUntilNextRun, PrintString;
    if(pTask->getStartDelay() > _BASE_CYCLE_TIME/2){
      log(String(TaskName + " Task Delay of " + String(pTask->getStartDelay())).c_str());
    }
  #endif
  #ifdef _DEBUG_VERBOSE
    TaskNameString = String(TaskName);
    //TaskStart = String("Task started at: " + String(rtc.()) + ":"  + String(minute()) + ":" + String(second()) + ":" + String(millis()));
    TaskInterval = String("Interval: " + String(pTask->getInterval()));
    StartDelay = String("Start Delay: " + String(pTask->getStartDelay()));
    Overrun = String("Overrun: " + String(pTask->getOverrun()));
    RunCounter = String("Run Counter: " + String(pTask->getRunCounter()));
    IterationCount = String("Iteration Count: " + String(pTask->getIterations()));
    TimeUntilNextRun = String("Time Until Nex Run: " + String(pscheduler->timeUntilNextIteration(*pTask)));
    PrintString = String(TaskNameString + ":" + TaskStart + ", " + TaskInterval + ", " + StartDelay + ", " + Overrun + ", " + RunCounter + ", " + IterationCount + ";");
    log(PrintString.c_str());
  #endif
}

void logScheduler(Scheduler* pscheduler) {
  #ifdef _DEBUG
    if(pscheduler->isOverrun()){
      log(String("Scheduler Overrun").c_str());
     }
  #endif
  #ifdef _DEBUG_VERBOSE
    String CPULoad_idle, CPULoad_cycle, CPULoad_total, SchedulerIsOverran, PrintString;
    SchedulerIsOverran = String("Overrun Status: " + String(pscheduler->isOverrun()));
    CPULoad_idle = String("CPU Idle: " + String(pscheduler->getCpuLoadIdle()));
    CPULoad_cycle = String("CPU Cycle: " + String(pscheduler->getCpuLoadCycle()));
    CPULoad_total = String("CPU Total: " + String(pscheduler->getCpuLoadTotal()));
    PrintString = String("Scheduler Completed Pass." + CPULoad_idle + "," + CPULoad_cycle + "," + CPULoad_total + ", " + SchedulerIsOverran);
    log(PrintString.c_str());
  #endif
  pscheduler->cpuLoadReset();
}
