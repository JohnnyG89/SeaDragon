//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

#if _USE_THINGSBOARD

static Logging tbtlmLog;
static EthernetClient  ethclient_tb1;
static ThingsBoard     tb1(ethclient_tb1);

void initThingsBoard_Telemetry(void) {

  tbtlmLog.begin(LOG_LEVEL_NOTICE, &Serial);
  tbtlmLog.setPrefix(printTimestamp);
  tbtlmLog.setSuffix(printNewline);
  tbtlmLog.notice("ThingsBoard_Telemetry:: Initialized ThingsBoard_Telemetry Log ::");
  tbtlmLog.trace("ThingsBoard_Telemetry:: Beginning ThingsBoard_Telemetry Initialization ::");

  if (!gEthernetConnectionActive) {
    tbtlmLog.warning("ThingsBoard_Telemetry:: Ethernet Connection Not Active-Returning to try again later ::");
    return;
  }

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicThingsBoard_Telemetry);
  Log.trace("ThingsBoard_Telemetry:: Finished ThingsBoard_Telemetry Initialization ::");
}

void cyclicThingsBoard_Telemetry(void) {
  logTaskTimer("ThingsBoard_Telemetry", &Scheduler::currentScheduler().currentTask());
  static int            ii = 0;
  bool                  ret = 0;
  int                   MessageRecordToSend = 0;
  static const  uint8_t CHANNEL_COUNT = 38;

  MessageRecordToSend = ii % CHANNEL_COUNT;

  if (!tb1.connected()) {
    Scheduler::currentScheduler().currentTask().setCallback(&initThingsBoard_Telemetry);
    tbtlmLog.notice("ThingsBoard_Telemetry:: Disconnected from ThingsBoard_Telemetry Server :: Returning To Initialization ::");
    return;
  }

  //  switch (MessageRecordToSend) {
  //    case 0: //oFreshwaterLightOutput
  //      ret = tb.sendTelemetryBool(		"oFreshwaterLight", 		!oFreshwaterLight);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oFreshwaterLight", 	!oFreshwaterLight);
  //      break;
  //    case 1: //oSkimmer
  //      ret = tb.sendTelemetryBool(		"oSkimmer", 					oSkimmer);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oSkimmer", 				oSkimmer);
  //      break;
  //    case 2: //oWaveMaker
  //      ret = tb.sendTelemetryBool(		"oWaveMaker", 					oWaveMaker);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oWaveMaker", 				oWaveMaker);
  //      break;
  //    case 3: //oPowerhead
  //      ret = tb.sendTelemetryBool(		"oPowerhead", 					oPowerhead);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oPowerhead", 				oPowerhead);
  //      break;
  //    case 4: //oSaltwaterLightOutput
  //      ret = tb.sendTelemetryBool(		"oSaltwaterLight", 		oSaltwaterLight);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oSaltwaterLight", 	oSaltwaterLight);
  //      break;
  //    case 5: //oReturnPump
  //      ret = tb.sendTelemetryBool(		"oReturnPump", 					oReturnPump);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oReturnPump", 				oReturnPump);
  //      break;
  //    case 6: //oFugeLight
  //      ret = tb.sendTelemetryBool(		"oFugeLight", 					oFugeLight);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oFugeLight", 				oFugeLight);
  //      break;
  //    case 7: //oAutoFeeder[0]
  //      ret = tb.sendTelemetryBool(		"oAutoFeeder1", 				oAutoFeeder[0]);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oAutoFeeder1", 			oAutoFeeder[0]);
  //      break;
  //    case 8: //oAutoFeeder[1]
  //      ret = tb.sendTelemetryBool(		"oAutoFeeder2", 				oAutoFeeder[1]);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oAutoFeeder2", 			oAutoFeeder[1]);
  //      break;
  //    case 9: //oDosePump[0]
  //      ret = tb.sendTelemetryBool(		"oDosePump1", 					oDosePump[0]);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oDosePump1", 				oDosePump[0]);
  //      break;
  //    case 10: //oDosePump[1]
  //      ret = tb.sendTelemetryBool(		"oDosePump2", 					oDosePump[1]);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oDosePump2", 				oDosePump[1]);
  //      break;
  //    case 11: //oATOPump
  //      ret = tb.sendTelemetryBool(		"oATOPump", 					oATOPump);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oATOPump", 				oATOPump);
  //      break;
  //    case 12: //oIndicatorLight_Red
  //      ret = tb.sendTelemetryBool(		"oIndicatorLight_Red", 			oIndicatorLight_Red);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oIndicatorLight_Red", 		oIndicatorLight_Red);
  //      break;
  //    case 13: //oIndicatorLight_Green
  //      ret = tb.sendTelemetryBool(		"oIndicatorLight_Green", 		oIndicatorLight_Green);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oIndicatorLight_Green", 	oIndicatorLight_Green);
  //      break;
  //    case 14: //oIndicatorLight_Blue
  //      ret = tb.sendTelemetryBool(		"oIndicatorLight_Blue", 		oIndicatorLight_Blue);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oIndicatorLight_Blue", 	oIndicatorLight_Blue);
  //      break;
  //    case 15: //iCabinetDoorSensor
  //      ret = tb.sendTelemetryBool(		"iCabinetDoorSensor", 			iCabinetDoorSensor);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iCabinetDoorSensor", 		iCabinetDoorSensor);
  //      break;
  //    case 16: //iATOReservoirEmpty
  //      ret = tb.sendTelemetryBool(		"iATOReservoirEmpty", 			iATOReservoirEmpty);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iATOReservoirEmpty", 		iATOReservoirEmpty);
  //      break;
  //    case 17: //iPanelDoor
  //      ret = tb.sendTelemetryBool(		"iPanelDoor", 					iPanelDoor);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iPanelDoor", 				iPanelDoor);
  //      break;
  //    case 18: //iTankOpticalSensor[0]
  //      ret = tb.sendTelemetryBool(		"iTankOpticalSensor1", 			iTankOpticalSensor[0]);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iTankOpticalSensor1", 		iTankOpticalSensor[0]);
  //      break;
  //    case 19: //iTankOpticalSensor[1]
  //      ret = tb.sendTelemetryBool(		"iTankOpticalSensor2", 			iTankOpticalSensor[1]);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iTankOpticalSensor2", 		iTankOpticalSensor[1]);
  //      break;
  //    case 20: //iTankOpticalSensor[2]
  //      ret = tb.sendTelemetryBool(		"iTankOpticalSensor3", 			iTankOpticalSensor[2]);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iTankOpticalSensor3", 		iTankOpticalSensor[2]);
  //      break;
  //    case 21: //iLeakDetector[0]
  //      ret = tb.sendTelemetryBool(		"iLeakDetector1", 				iLeakDetector[0]);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iLeakDetector1", 			iLeakDetector[0]);
  //      break;
  //    case 22: //iLeakDetector[1]
  //      ret = tb.sendTelemetryBool(		"iLeakDetector2", 				iLeakDetector[1]);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iLeakDetector2", 			iLeakDetector[1]);
  //      break;
  //    case 23: //iAnalogValue
  //      ret = tb.sendTelemetryFloat(		"iAnalogValue", 				iAnalogValue);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %f ::", ii,   THINGSBOARD_SERVER, 	"iAnalogValue", 			iAnalogValue);
  //      break;
  //    case 24: //smoothedSensorValueAvg
  //      ret = tb.sendTelemetryFloat(		"smoothedSensorValueAvg", 		smoothedSensorValueAvg);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %f ::", ii,   THINGSBOARD_SERVER, 	"smoothedSensorValueAvg", 	smoothedSensorValueAvg);
  //      break;
  //    case 25: //ipHSensorReading
  //      ret = tb.sendTelemetryFloat(		"ipHSensorReading", 			ipHSensorReading);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %f ::", ii,   THINGSBOARD_SERVER, 	"ipHSensorReading", 		ipHSensorReading);
  //      break;
  //    case 26: //iTemperature
  //      ret = tb.sendTelemetryFloat(		"iTemperature", 				iTemperature);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %f ::", ii,   THINGSBOARD_SERVER, 	"iTemperature", 			iTemperature);
  //      break;
  //    case 27: //iTemperature_Smoothed
  //      ret = tb.sendTelemetryFloat(		"iTemperature_Smoothed", 		iTemperature_Smoothed);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %f ::", ii,   THINGSBOARD_SERVER, 	"iTemperature_Smoothed", 	iTemperature_Smoothed);
  //      break;
  //    case 28: //imode_Auto
  //      ret = tb.sendTelemetryBool(		"imode_Lights_Auto", 					imode_Lights_Auto);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"imode_Lights_Auto", 				imode_Lights_Auto);
  //      break;
  //    case 29: //imode_manSkimmer
  //      ret = tb.sendTelemetryBool(		"iswt_Skimmer", 			iswt_Skimmer);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iswt_Skimmer", 		iswt_Skimmer);
  //      break;
  //    case 30: //imode_manWaveMaker
  //      ret = tb.sendTelemetryBool(		"iswt_Wavemaker", 			iswt_Wavemaker);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iswt_Wavemaker", 		iswt_Wavemaker);
  //      break;
  //    case 31: //imode_manPowerhead
  //      ret = tb.sendTelemetryBool(		"iswt_Powerhead", 			iswt_Powerhead);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iswt_Powerhead", 		iswt_Powerhead);
  //      break;
  //    /* case 32: //imode_manAutoFeeder
  //       ret = tb.sendTelemetryBool(		"imode_manAutoFeeder", 			imode_manAutoFeeder);
  //       tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"imode_manAutoFeeder", 		imode_manAutoFeeder);
  //       break;*/
  //    case 33: //iswt_FreshwaterLight
  //      ret = tb.sendTelemetryBool(		"iswt_FreshwaterLight", 	iswt_FreshwaterLight);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"iswt_FreshwaterLight", iswt_FreshwaterLight);
  //      break;
  //    case 34: //imode_Feed
  //      ret = tb.sendTelemetryBool(		"imode_Feed", 			imode_Feed);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"imode_Feed", 		imode_Feed);
  //      break;
  //    /*case 35: //imode_Storm
  //      ret = tb.sendTelemetryBool(		"imode_WebControl", 			imode_WebControl);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"imode_WebControl", 		imode_WebControl);
  //      break;*/
  //    case 36: //oCabinetLight
  //      ret = tb.sendTelemetryBool(		"oCabinetLight", 				oCabinetLight);
  //      tbtlmLog.verbose("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %T ::", ii,   THINGSBOARD_SERVER, 	"oCabinetLight", 			oCabinetLight);
  //      break;
  //    case 37://Send Message Index
  //      ret = tb.sendTelemetryInt(    "MessageIndex",                 	ii);
  //      tbtlmLog.notice("ThingsBoard_Telemetry:: Msg %d :: Sent Data To %s :: Channel %s :: Value %d ::", ii,   THINGSBOARD_SERVER,   "MessageIndex",             ii);
  //      /* ret= tb.sendAttributeBool("valueIndicator", imode_manSkimmer);*/
  //      break;
  //    default: //
  //      break;
  //  }
  //
  //  if (ret == 0) {
  //    tbtlmLog.error("ThingsBoard_Telemetry:: Error sending telemetry :: Msg %d :: Channel %d ::", ii, MessageRecordToSend);
  //  }

  ii++;
}

#endif
