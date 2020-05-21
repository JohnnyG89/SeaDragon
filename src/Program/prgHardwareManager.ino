//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static Logging hdwLog;

static const uint8_t P1_MODULE_COUNT      = 7;

//Constants for the IO module slots
static const uint8_t P1_SLOT_SIM_1        = 1;
static const uint8_t P1_SLOT_SIM_2        = 2;
static const uint8_t P1_SLOT_RTD          = 3;
static const uint8_t P1_SLOT_AIO          = 4;
static const uint8_t P1_SLOT_DO           = 5;
static const uint8_t P1_SLOT_DI           = 6;
static const uint8_t P1_SLOT_RO           = 7;

//Hardware-specific parameters:
static const char* baseArray[]            = {"P1-08SIM", "P1-08SIM", "P1-04RTD", "P1-4ADL2DAL-2", "P1-08TD2", "P1-08ND3", "P1-08TRS"}; //Expected Modules

//IO Bits- Wired into slices
static const channelLabel ain_pH          = {P1_SLOT_AIO,   1};
static const channelLabel ain_Temp_1        = {P1_SLOT_RTD,   1};
static const channelLabel ain_Temp_2        = {P1_SLOT_RTD,   2};

static const char P1_04RTD_CONFIG[]       = {0x40, 0x00, 0x60, 0x01, 0x20, 0x01, 0x80, 0x00}; //Low-side burnout, temp in C, PT100, 33Hz filter

void initHardwareManager(void) {
  hdwLog.begin(LOG_LEVEL_NOTICE, &Serial);
  hdwLog.setPrefix(printTimestamp);
  hdwLog.setSuffix(printNewline);
  hdwLog.notice("HardwareManager:: Initialized HardwareManager Log ::");
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
  }
  P1.printModules();                              //print out all currently signed-on modules to the console
  P1.getFwVersion();                              //print the Base Controller's firmware version to the console

  int baseErrors = P1.rollCall(baseArray, P1_MODULE_COUNT);     //Pass in list of modules and the number to check
  hdwLog.notice("HardwareManager:: Base Errors=%x", baseErrors);

  hdwLog.verbose("HardwareManager:: Configuring Temperature Input, Slot=%d, Module=%s :: Value=%B ::", P1_SLOT_RTD, baseArray[P1_SLOT_RTD - 1], P1_04RTD_CONFIG);

  while (!P1.configureModule(P1_04RTD_CONFIG, P1_SLOT_RTD)) {};
  hdwLog.notice("HardwareManager:: Configured Temperature Input, Slot %d, Module %s, Value %X", P1_SLOT_RTD, baseArray[P1_SLOT_RTD - 1], P1_04RTD_CONFIG);

  configCheck();
  statusCheck();

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicHardwareManager);
  hdwLog.trace("HardwareManager::Finished P1AM Initialization");
}

void cyclicHardwareManager(void) {
  logTaskTimer("Hardware Manager", &Scheduler::currentScheduler().currentTask());

  if (!P1.isBaseActive()) {
    Scheduler::currentScheduler().currentTask().setCallback(&initHardwareManager);
    hdwLog.error("HardwareManager:: Lost base controller :: Returning to initialization ::");
    bitWrite(gFaultWord, FLT_HARDWARE_TIMEOUT, 1);
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
  bitWrite(              oRelayOutputWord,   0,       ioFreshwaterLight._out);
  bitWrite(              oRelayOutputWord,   1,       oCabinetLight);
  bitWrite(              oRelayOutputWord,   2,       ioSkimmer._out);
  bitWrite(              oRelayOutputWord,   3,       ioWavemaker._out);
  bitWrite(              oRelayOutputWord,   4,       ioPowerhead._out);
  bitWrite(              oRelayOutputWord,   5,       ioSaltwaterLight._out);
  bitWrite(              oRelayOutputWord,   6,       ioReturn._out);
  bitWrite(              oRelayOutputWord,   7,       ioFugeLight._out);
  P1.writeDiscrete(      oRelayOutputWord,            P1_SLOT_RO);

  //Write to the discrete output module
  bitWrite(              oDiscreteOutputWord, 0,      ioAutofeeder[0]._out);
  bitWrite(              oDiscreteOutputWord, 1,      ioAutofeeder[1]._out);
  bitWrite(              oDiscreteOutputWord, 2,      ioDosePump[0]._out);
  bitWrite(              oDiscreteOutputWord, 3,      ioDosePump[1]._out);
  bitWrite(              oDiscreteOutputWord, 4,      ioATOPump._out);
  bitWrite(              oDiscreteOutputWord, 5,      oIndicatorLight_Red);
  bitWrite(              oDiscreteOutputWord, 6,      oIndicatorLight_Green);
  bitWrite(              oDiscreteOutputWord, 7,      oIndicatorLight_Blue);
  P1.writeDiscrete(      oDiscreteOutputWord,         P1_SLOT_DO);

  //  hdwLog.verbose("HardwareManager:: Writing Outputs :: Freshwater Light=%T :: Cabinet Light=%T :: LED Indicator=%T ::", oFreshwaterLight, oCabinetLight, oLEDIndicator);
}

static void ioRead_IN() {
  //Read Discrete Inputs
  iDiscreteInputWord =              P1.readDiscrete(                P1_SLOT_DI);
  iCabinetDoorSensor =              bitRead(iDiscreteInputWord,     0);
  iSkimmerLevelHigh =              bitRead(iDiscreteInputWord,     1);
  iPanelDoor =                      bitRead(iDiscreteInputWord,     2);
  iTankOpticalSensor[0] =           bitRead(iDiscreteInputWord,     3);
  iTankOpticalSensor[1] =           bitRead(iDiscreteInputWord,     4);
  iTankOpticalSensor[2] =           bitRead(iDiscreteInputWord,     5);
  iLeakDetector[0] =                bitRead(iDiscreteInputWord,     6);
  iLeakDetector[1] =                bitRead(iDiscreteInputWord,     7);

  //Read Analog/RTD Values
  ioTemp[0].raw =                      P1.readTemperature(ain_Temp_1);
  ioTemp[0].scaled =                   ((1.8 * ioTemp[0].raw) + 32);
  ioTemp[1].raw =                      P1.readTemperature(ain_Temp_2);
  ioTemp[1].scaled =                   ((1.8 * ioTemp[1].raw) + 32);
  iopH.raw =                        P1.readAnalog(ain_pH);

  //Read Switch on the PLC itself
  iSwitchState =                    digitalRead(SWITCH_BUILTIN);//Read the state of the switch

  iMachineModeSwitchWord[0] =       P1.readDiscrete(P1_SLOT_SIM_1);
  iMachineModeSwitchWord[1] =       P1.readDiscrete(P1_SLOT_SIM_2);

  ioSaltwaterLight.mode._Auto =   	bitRead(iMachineModeSwitchWord[0], 0);
  ioSaltwaterLight.mode._Remote = 	bitRead(iMachineModeSwitchWord[0], 1);
  ioSaltwaterLight.mode._Manual = 	!ioSaltwaterLight.mode._Auto;

  ioFreshwaterLight.mode._Auto =  	bitRead(iMachineModeSwitchWord[0], 0);
  ioFreshwaterLight.mode._Remote =	bitRead(iMachineModeSwitchWord[0], 1);
  ioFreshwaterLight.mode._Manual = 	!ioFreshwaterLight.mode._Auto;

  ioFugeLight.mode._Auto =        	bitRead(iMachineModeSwitchWord[0], 0);
  ioFugeLight.mode._Remote =      	bitRead(iMachineModeSwitchWord[0], 1);

  ioAutofeeder[0].mode._Auto =    	bitRead(iMachineModeSwitchWord[0], 2);
  ioAutofeeder[0].mode._Remote =  	bitRead(iMachineModeSwitchWord[0], 3);

  ioAutofeeder[1].mode._Auto =    	bitRead(iMachineModeSwitchWord[0], 2);
  ioAutofeeder[1].mode._Remote =  	bitRead(iMachineModeSwitchWord[0], 3);

  ioDosePump[0].mode._Auto =      	bitRead(iMachineModeSwitchWord[0], 4);
  ioDosePump[0].mode._Remote =    	bitRead(iMachineModeSwitchWord[0], 5);

  ioDosePump[1].mode._Auto =      	bitRead(iMachineModeSwitchWord[0], 4);
  ioDosePump[1].mode._Remote =    	bitRead(iMachineModeSwitchWord[0], 5);

  ioATOPump.mode._Auto =          	bitRead(iMachineModeSwitchWord[0], 4);
  ioATOPump.mode._Remote =        	bitRead(iMachineModeSwitchWord[0], 5);

  ioSkimmer.mode._Auto =          	bitRead(iMachineModeSwitchWord[0], 6);
  ioSkimmer.mode._Remote =        	bitRead(iMachineModeSwitchWord[0], 7);

  ioWavemaker.mode._Auto =        	bitRead(iMachineModeSwitchWord[0], 6);
  ioWavemaker.mode._Remote =      	bitRead(iMachineModeSwitchWord[0], 7);

  ioPowerhead.mode._Auto =        	bitRead(iMachineModeSwitchWord[0], 6);
  ioPowerhead.mode._Remote =      	bitRead(iMachineModeSwitchWord[0], 7);

  ioReturn.mode._Auto =           	bitRead(iMachineModeSwitchWord[0], 6);
  ioReturn.mode._Remote =         	bitRead(iMachineModeSwitchWord[0], 7);

  ioAutofeeder[0].ctrl._Manual =  	bitRead(iMachineModeSwitchWord[1], 0);
  ioAutofeeder[1].ctrl._Manual =  	bitRead(iMachineModeSwitchWord[1], 1);

  ioDosePump[0].ctrl._Manual =    	bitRead(iMachineModeSwitchWord[1], 2);
  ioDosePump[1].ctrl._Manual =    	bitRead(iMachineModeSwitchWord[1], 3);

  ioFreshwaterLight.ctrl._Manual =  bitRead(iMachineModeSwitchWord[1], 4);
  ioSaltwaterLight.ctrl._Manual =  	bitRead(iMachineModeSwitchWord[1], 4);
  ioFugeLight.ctrl._Manual =  		  bitRead(iMachineModeSwitchWord[1], 4);

  ioSkimmer.ctrl._Manual =  		    bitRead(iMachineModeSwitchWord[1], 5);
  ioWavemaker.ctrl._Manual =  		  bitRead(iMachineModeSwitchWord[1], 6);
  ioPowerhead.ctrl._Manual =  		  bitRead(iMachineModeSwitchWord[1], 6);
  ioReturn.ctrl._Manual =  			    bitRead(iMachineModeSwitchWord[1], 7);

  //  hdwLog.verbose("HardwareManager:: Reading Inputs :: Door Switch=%T :: Temperature=%F :: pH Analog Voltage=%d :: PLC Switch=%T :: Mode Switch=%B ::", iCabinetDoorSensor, iTemperature, iAnalogValue, iSwitchState, iMachineModeSwitchWord);
}
