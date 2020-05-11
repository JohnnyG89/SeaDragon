//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static Logging hdwLog;

static const uint8_t P1_MODULE_COUNT      = 6;

//Constants for the IO module slots
static const uint8_t P1_SLOT_SIM          = 1;
static const uint8_t P1_SLOT_RTD          = 2;
static const uint8_t P1_SLOT_AIO          = 3;
static const uint8_t P1_SLOT_DO           = 4;
static const uint8_t P1_SLOT_DI           = 5;
static const uint8_t P1_SLOT_RO           = 6;

//Constants for the IO module slots
//static const uint8_t P1_SLOT_SIM_1          = 1;
//static const uint8_t P1_SLOT_SIM_2          = 2;
//static const uint8_t P1_SLOT_RTD          = 3;
//static const uint8_t P1_SLOT_AIO          = 4;
//static const uint8_t P1_SLOT_DO           = 5;
//static const uint8_t P1_SLOT_DI           = 6;
//static const uint8_t P1_SLOT_RO           = 7;

//Hardware-specific parameters:
static const char* baseArray[]            = {"P1-08SIM", "P1-04RTD", "P1-4ADL2DAL-2", "P1-08TD2", "P1-08ND3", "P1-08TRS"}; //Expected Modules
//static const char* baseArray[]            = {"P1-08SIM", "P1-08SIM", "P1-04RTD", "P1-4ADL2DAL-2", "P1-08TD2", "P1-08ND3", "P1-08TRS"}; //Expected Modules

//IO Bits- Wired into slices
static const channelLabel ain_pH          = {P1_SLOT_AIO,   1};
static const channelLabel ain_Temp        = {P1_SLOT_RTD,   1};

static const char P1_04RTD_CONFIG[]       = {0x40, 0x00, 0x60, 0x01, 0x20, 0x01, 0x80, 0x00}; //Low-side burnout, temp in C, PT100, 33Hz filter

void initHardwareManager(void) {
  hdwLog.begin(LOG_LEVEL_NOTICE, &Serial);
  hdwLog.setPrefix(printTimestamp);
  hdwLog.setSuffix(printNewline);
  Log.notice("HardwareManager:: Initialized HardwareManager Log ::");
  hdwLog.trace("HardwareManager:: Initializing Hardware And Communications");

  pinMode(LED_BUILTIN, OUTPUT);                   //Set our LED (Pin 32) to be an output
  pinMode(SWITCH_BUILTIN, INPUT);                 //Set our Switch (Pin 31) to be an input

  if (!_REQUIRE_HARDWARE) {
    Scheduler::currentScheduler().currentTask().disable();
    hdwLog.warning("HardwareManager:: Hardware Disabled.");
    return;
  }

  while (!P1.init()) {
    hdwLog.fatal("HardwareManager:: Waiting For P1 Modules");
    return;
//    delay(100);
  }
  //P1.printModules();                              //print out all currently signed-on modules to the console
  P1.getFwVersion();                              //print the Base Controller's firmware version to the console

  int baseErrors = P1.rollCall(baseArray, P1_MODULE_COUNT);     //Pass in list of modules and the number to check
  hdwLog.notice("HardwareManager:: Base Errors=%x", baseErrors);

  hdwLog.verbose("HardwareManager:: Configuring Temperature Input, Slot=%d, Module=%s :: Value=%B ::", P1_SLOT_RTD, baseArray[P1_SLOT_RTD - 1], P1_04RTD_CONFIG);

  while (!P1.configureModule(P1_04RTD_CONFIG, P1_SLOT_RTD)) {};
  hdwLog.notice("HardwareManager:: Configured Temperature Input, Slot %d, Module %s, Value %X", P1_SLOT_RTD, baseArray[P1_SLOT_RTD - 1], P1_04RTD_CONFIG);

  configCheck();
  statusCheck();

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicHardwareManager);
  //Scheduler::currentScheduler().startNow();
  hdwLog.trace("HardwareManager::Finished P1AM Initialization");
}

void cyclicHardwareManager(void) {
  logTaskTimer("Hardware Manager", &Scheduler::currentScheduler().currentTask());

  if(!P1.isBaseActive()){
    Scheduler::currentScheduler().currentTask().setCallback(&initHardwareManager);
    hdwLog.error("HardwareManager:: Lost base controller :: Returning to initialization ::");
    return;
  }

  ioWrite_OUT();//Write previous cycle outputs:
  ioRead_IN();//Read this cycle's inputs:
}

static void statusCheck(void) {
  char statusByteArray[P1_MODULE_COUNT + 1][32];
  for (int ii = 1; ii < P1_MODULE_COUNT + 1; ii++) {
    P1.readStatus(statusByteArray[ii], ii);
    if (statusByteArray[ii] != 0) {
      hdwLog.trace("HardwareManager:: Status Byte, Module %d-%s, Value %b", ii, baseArray[ii - 1], statusByteArray[ii]);
    }
  }
}

static void configCheck(void) {
  char configInputBuffer[P1_MODULE_COUNT + 1][32];
  for (int ii = 1; ii < P1_MODULE_COUNT + 1; ii++) {
    P1.readModuleConfig(configInputBuffer[ii], ii);
    hdwLog.notice("HardwareManager:: Read module configuration :: Slot %d :: Module %s :: Value %b ::", ii, baseArray[ii - 1], configInputBuffer[ii]);
  }
  hdwLog.trace("HardwareManager::Finished Config Check");
}

static void ioWrite_OUT() {
  //LED On the PLC itself
  digitalWrite(               LED_BUILTIN,                    oLEDIndicator); //Update the LED

  //Write to the relay output module
  bitWrite(                   oRelayOutputWord,   0,          oFreshwaterLight);
  bitWrite(                   oRelayOutputWord,   1,          oCabinetLight);
  bitWrite(                   oRelayOutputWord,   2,          oSkimmer);
  bitWrite(                   oRelayOutputWord,   3,          oWaveMaker);
  bitWrite(                   oRelayOutputWord,   4,          oPowerhead);
  bitWrite(                   oRelayOutputWord,   5,          oSaltwaterLight);
  bitWrite(                   oRelayOutputWord,   6,          oReturnPump);
  bitWrite(                   oRelayOutputWord,   7,          oFugeLight);
  P1.writeDiscrete(           oRelayOutputWord,               P1_SLOT_RO);

  //Write to the discrete output module
  bitWrite(                   oDiscreteOutputWord, 0,         oAutoFeeder[0]);
  bitWrite(                   oDiscreteOutputWord, 1,         oAutoFeeder[1]);
  bitWrite(                   oDiscreteOutputWord, 2,         oDosePump[0]);
  bitWrite(                   oDiscreteOutputWord, 3,         oDosePump[1]);
  bitWrite(                   oDiscreteOutputWord, 4,         oATOPump);
  bitWrite(                   oDiscreteOutputWord, 5,         oIndicatorLight_Red);
  bitWrite(                   oDiscreteOutputWord, 6,         oIndicatorLight_Green);
  bitWrite(                   oDiscreteOutputWord, 7,         oIndicatorLight_Blue);
  P1.writeDiscrete(           oDiscreteOutputWord,            P1_SLOT_DO);

  hdwLog.verbose("HardwareManager:: Writing Outputs :: Freshwater Light=%T :: Cabinet Light=%T :: LED Indicator=%T ::", oFreshwaterLight, oCabinetLight, oLEDIndicator);
}

static void ioRead_IN() {
  //Read Discrete Inputs
  iDiscreteInputWord =        P1.readDiscrete(                P1_SLOT_DI);
  iCabinetDoorSensor =        bitRead(iDiscreteInputWord,     0);
  iATOReservoirEmpty =        bitRead(iDiscreteInputWord,     1);
  iPanelDoor =                bitRead(iDiscreteInputWord,     2);
  iTankOpticalSensor[0] =     bitRead(iDiscreteInputWord,     3);
  iTankOpticalSensor[1] =     bitRead(iDiscreteInputWord,     4);
  iTankOpticalSensor[2] =     bitRead(iDiscreteInputWord,     5);
  iLeakDetector[0] =          bitRead(iDiscreteInputWord,     6);
  iLeakDetector[1] =          bitRead(iDiscreteInputWord,     7);

  //Read Analog/RTD Values
  iTemperature =              (1.8 * P1.readTemperature(ain_Temp)) + 32;
  iAnalogValue =              P1.readAnalog(          ain_pH);

  //Read Switch on the PLC itself
  iSwitchState =              digitalRead(            SWITCH_BUILTIN);//Read the state of the switch

  //Read Machine Mode Inputs
  iMachineModeSwitchWord =    P1.readDiscrete(                  P1_SLOT_SIM);
  imode_Auto =                bitRead(iMachineModeSwitchWord,   0);
  imode_manSkimmer =          bitRead(iMachineModeSwitchWord,   1);
  imode_manWaveMaker =        bitRead(iMachineModeSwitchWord,   2);
  imode_manPowerhead =        bitRead(iMachineModeSwitchWord,   3);
  imode_manAutoFeeder =       bitRead(iMachineModeSwitchWord,   4);
  imode_manFreshwaterLight =  bitRead(iMachineModeSwitchWord,   5);
  imode_manDosePump =         bitRead(iMachineModeSwitchWord,   6);
  imode_WebControl =          bitRead(iMachineModeSwitchWord,   7);
  
  hdwLog.verbose("HardwareManager:: Reading Inputs :: Door Switch=%T :: Temperature=%F :: pH Analog Voltage=%d :: PLC Switch=%T :: Mode Switch=%B ::", iCabinetDoorSensor, iTemperature, iAnalogValue, iSwitchState, iMachineModeSwitchWord);
}
  /*
	
  iMachineModeSwitchWord[0] = P1.readDiscrete(P1_SLOT_SIM_1);
  iMachineModeSwitchWord[1] = P1.readDiscrete(P1_SLOT_SIM_2);
  
  imode_Lights_Auto =                bitRead(iMachineModeSwitchWord[0],   0);
  imode_Lights_Remote =          bitRead(iMachineModeSwitchWord[0],   1);
  imode_Autofeeder_Auto =        bitRead(iMachineModeSwitchWord[0],   2);
  imode_Autofeeder_Remote =        bitRead(iMachineModeSwitchWord[0],   3);
  imode_DosePumps_Auto =       bitRead(iMachineModeSwitchWord[0],   4);
  imode_DosePumps_Remote =  bitRead(iMachineModeSwitchWord[0],   5);
  imode_Skimmer =         bitRead(iMachineModeSwitchWord[0],   6);
  imode_ReturnPump =          bitRead(iMachineModeSwitchWord[0],   7);
  
  iswt_Autofeeder[0] =                bitRead(iMachineModeSwitchWord[1],   0);
  iswt_Autofeeder[1] =          bitRead(iMachineModeSwitchWord[1],   1);
  iswt_DosePump[0] =        bitRead(iMachineModeSwitchWord[1],   2);
  iswt_DosePump[1] =        bitRead(iMachineModeSwitchWord[1],   3);
  iswt_FreshwaterLight =       bitRead(iMachineModeSwitchWord[1],   4);
  iswt_SaltwaterLight =  bitRead(iMachineModeSwitchWord[1],   5);
  iswt_Powerhead =         bitRead(iMachineModeSwitchWord[1],   6);
  iswt_ReturnPump =          bitRead(iMachineModeSwitchWord[1],   7);

  */
