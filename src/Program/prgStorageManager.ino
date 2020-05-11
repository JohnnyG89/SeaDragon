//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static Logging stgLog;

void initStorageManager(void) {
  stgLog.begin(LOG_LEVEL_NOTICE, &Serial);
  stgLog.setPrefix(printTimestamp);
  stgLog.setSuffix(printNewline);
  stgLog.notice("StorageManager:: Initialized StorageManager Log ::");

  //SD Card Related Declarators
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  static const int chipSelect = SDCARD_SS_PIN;
  File dataFile;

  Log.trace("StorageManager:: Initializing Storage Manager...");

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    stgLog.error("StorageManager:: Unable to initialize SD Card");
  }
  else {
    stgLog.notice("StorageManager:: SD Card Initialized");
  }

  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      stgLog.notice("StorageManager:: Card Type: SD1");
      break;
    case SD_CARD_TYPE_SD2:
      stgLog.notice("StorageManager:: Card Type: SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      stgLog.notice("StorageManager:: Card Type: SDHC");
      break;
    default:
      stgLog.error("StorageManager:: Card Type: Unknown");
  }

  SD.begin();

  if (!SD.exists(LOG_FILE_NAME)) {
    stgLog.fatal("StorageManager:: Data File %s Doesn't exist", LOG_FILE_NAME);
    File dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
    dataFile.write("hello");
    dataFile.close();
    delay(100);
  }

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicStorageManager);
  Log.trace("StorageManager:: Initialized Storage Manager");
}

void cyclicStorageManager(void) {
  logTaskTimer("StorageManager", &Scheduler::currentScheduler().currentTask());
  Scheduler::currentScheduler().currentTask().disable();
}

void logTaskTimer(const char* TaskName, Task* pTask) {
  if (pTask->getStartDelay() > 50) {
    stgLog.notice("%s:: id %d :: Delay %d ms :: Iteration %d ::", TaskName, pTask->getId(), pTask->getStartDelay(), pTask->getRunCounter());
    return;
  }
  stgLog.verbose("%s:: id %d :: Delay %d ms :: Iteration %d ::", TaskName, pTask->getId(), pTask->getStartDelay(), pTask->getRunCounter());
}

void logScheduler(Scheduler* pscheduler) {
  if (pscheduler->isOverrun()) {
    stgLog.error("StorageManager:: Scheduler Overrun ::");
  }
  stgLog.verbose("Scheduler:: Overrun Status %t :: Idle Load %d :: Cycle Load %d :: Total Load %d ::", pscheduler->isOverrun(), pscheduler->getCpuLoadIdle(), pscheduler->getCpuLoadCycle(), pscheduler->getCpuLoadTotal());
  pscheduler->cpuLoadReset();
}
