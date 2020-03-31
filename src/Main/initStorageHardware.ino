void initStorageHardware(void){
	//SD Card Handlers:
  log("Initializing SD Module...")
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    log("Unable to initialize SD Card");
  }
  else {
    log("SD Card Initialized");
  }

  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      log("Card Type: SD1");
      break;
    case SD_CARD_TYPE_SD2:
      log("Card Type: SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      log("Card Type: SDHC");
      break;
    default:
      log("Card Type: Unknown");
  }

  SD.begin();

  while(!SD.exists(LOG_FILE_NAME)){
    Serial.println("Data File Doesn't exist");
    File dataFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
    dataFile.write("hello");
    dataFile.close();
    delay(1000);
  }
}
