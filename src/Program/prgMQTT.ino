//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

#if _USE_PUBSUB

static Logging mqttLog;
static EthernetClient  ethclient_mqtt;
static const char* mqttClientID = "SeaDragon_Telemetry";
static const char* mqttUserID = "PutYourUserIDHere";
static const char* mqttPassword = "PutYourPasswordHere";
static const char* mqttBroker = "broker.shiftr.io";
static const int 	mqttPort = 1883;
static const int mqttPrintBuffer = 100;
static const int mqttDocSize = 200;
static const int numMQTTGroups = 1;
static PubSubClient	mqttClient(mqttBroker, mqttPort, mqttCallback, ethclient_mqtt);

//Last Cycle Values
static ioModule_typ _ioSkimmer, _ioWavemaker, _ioPowerhead, _ioReturn;     //Pumps/powerheads
static ioModule_typ _ioAutofeeder[2], _ioDosePump[2], _ioATOPump;         //Dispensers
static ioModule_typ _ioSaltwaterLight, _ioFreshwaterLight, _ioFugeLight;  //Lights

void initMQTT(void) {
  if (!gEthernetConnectionActive) {
    mqttLog.warning("MQTT:: No ethernet comm yet.  Returning to try again on next pass ::");
    return;
  }
  mqttLog.begin(LOG_LEVEL_NOTICE, &Serial);
  mqttLog.setPrefix(printTimestamp);
  mqttLog.setSuffix(printNewline);
  mqttLog.notice("MQTT:: Initialized MQTT Log ::");
  mqttLog.trace("MQTT:: Beginning MQTT Initialization");

  if (mqttClient.connect(mqttClientID, mqttUserID, mqttPassword)) {
    mqttLog.notice("MQTT:: Connected to %s:%d :: State %d :: Username %s :: Password %s :: ClientID %s ::", mqttBroker, mqttPort, mqttClient.state(), mqttUserID, mqttPassword, mqttClientID);
  } else {
    mqttLog.error("MQTT:: Not able to connect to %s :: State %d :: Trying again on next pass ::", mqttBroker, mqttClient.state());
    return;
  }

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicMQTT);
  mqttLog.trace("MQTT:: Finished MQTT Initialization");
}

void cyclicMQTT(void) {
  static int ii = 0;
  int currentChannel = ii % numMQTTGroups;
  int moduleCount;
  logTaskTimer("MQTT", &Scheduler::currentScheduler().currentTask());
  char printbuff[mqttPrintBuffer];
  CStringBuilder sb_print(printbuff, sizeof(printbuff));
  StaticJsonDocument<mqttDocSize> doc;
  JsonObject robjAquarium = doc.to<JsonObject>();

  if (!mqttClient.connected()) {
    mqttLog.warning("MQTT:: Connecting to MQTT Client ::");
    Scheduler::currentScheduler().currentTask().setCallback(&initMQTT);
  }
//  switch (currentChannel) {
//    case 0:
//      //Group 1 - skimmer, wavemaker, powerhead status
      AddIOModule(ii, robjAquarium, "pmp", "skm", "1", ioSkimmer, _ioSkimmer);
//      AddIOModule(ii, robjAquarium, "pmp", "wvm", "1", ioWavemaker, _ioWavemaker);
//      AddIOModule(ii, robjAquarium, "pmp", "pwh", "1", ioPowerhead, _ioPowerhead);
//      break;
//      //Group 2 - return pump, autofeeders
////      AddIOModule(ii, robjAquarium, "pmp", "rtn", "1", ioReturn, _ioReturn);
////      AddIOModule(ii, robjAquarium, "dsp", "afd", "1", ioAutofeeder[0], _ioAutofeeder[0]);
//    case 1:
//      AddIOModule(ii, robjAquarium, "dsp", "afd", "2", ioAutofeeder[1], _ioAutofeeder[1]);
//      break;
//
//      //Group 3 - dosing pumps, ato
//      AddIOModule(ii, robjAquarium, "dsp", "dos", "1", ioDosePump[0], _ioDosePump[0]);
//      AddIOModule(ii, robjAquarium, "dsp", "dos", "2", ioDosePump[1], _ioDosePump[1]);
//      AddIOModule(ii, robjAquarium, "dsp", "ato", "1", ioATOPump, _ioATOPump);
//      break;
//      //Group 4 - Lights
//      AddIOModule(ii, robjAquarium, "lit", "slt", "1", ioSaltwaterLight, _ioSaltwaterLight);
//    case 2:
//      AddIOModule(ii, robjAquarium, "lit", "fsh", "1", ioFreshwaterLight, _ioFreshwaterLight);
//      AddIOModule(ii, robjAquarium, "lit", "fug", "1", ioFugeLight, _ioFugeLight);
//      break;
//      //Group 5 - Analog, sensors, indicators, etc.
//      AddAnalogModule(robjAquarium, "anl", "tmp", "1", ioTemp[0]);
//      AddAnalogModule(robjAquarium, "anl", "tmp", "2", ioTemp[1]);
//      AddAnalogModule(robjAquarium, "anl", "phx", "1", iopH);
//      AddValue(robjAquarium, "snr", "cbn", iCabinetDoorSensor);
//      AddValue(robjAquarium, "snr", "skm", iSkimmerLevelHigh);
//      AddValue(robjAquarium, "snr", "pnl", iPanelDoor);
//      AddValue(robjAquarium, "snr", "lv1", iTankOpticalSensor[0]);
//      AddValue(robjAquarium, "snr", "lv2", iTankOpticalSensor[1]);
//      AddValue(robjAquarium, "snr", "lv3", iTankOpticalSensor[2]);
//      AddValue(robjAquarium, "snr", "lk1", iLeakDetector[0]);
//      AddValue(robjAquarium, "snr", "lk2", iLeakDetector[1]);
//      AddValue(robjAquarium, "ind", "red", oIndicatorLight_Red);
//      AddValue(robjAquarium, "ind", "grn", oIndicatorLight_Green);
//      AddValue(robjAquarium, "ind", "blu", oIndicatorLight_Blue);
//      AddValue(robjAquarium, "lit", "cbn", oCabinetLight);
//      break;
//    default:
//      break;
//  };
  int numBytes = serializeJson(robjAquarium, sb_print);
  mqttLog.notice("MQTT:: Channel %d :: Message %d :: Doc Mem = %d :: Buffer Len %d :: Bytes=%d :: Free Mem = %d :: %s ::", currentChannel, ii, doc.memoryUsage(), sb_print.length(), numBytes, freeMemory(), printbuff);
  bool ret = mqttClient.publish("sd", printbuff);

  ii++;
  mqttClient.loop();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  mqttLog.notice("MQTT:: Callback %s :: Payload %s :: Length %d ::", topic, payload, length);
};

static void AddValue(JsonObject &obj, const char* inBuff, const char* spec, bool val) {
  char buff[30];
  sprintf(buff, "%s/%s", inBuff, spec);
  obj[buff] = val;
};

static void AddInfo(JsonObject &obj, const char* inBuff, const char* typeName, opMode_typ &mode) {
  char buff[30];
  sprintf(buff, "%s/%s", inBuff, typeName);
  AddValue(obj, buff, "aut", mode._Auto);
  AddValue(obj, buff, "man", mode._Manual);
  AddValue(obj, buff, "rem", mode._Remote);
  mqttLog.verbose("JSON:: Buffer :: %s ::", buff);
};

static void AddIOModule(int ii, JsonObject &obj, const char *equipClass, const char *equipName, const char* instName, ioModule_typ &mod, ioModule_typ &_mod) {
  char buff[30];
  sprintf(buff, "%s/%s/%s", equipClass, equipName, instName);

  //reduce some of the traffic
  //  if ((memcmp(&mod, &_mod, sizeof(ioModule_typ)) == 0) && ((ii/5)%10)!=0) {
  //    mqttLog.notice("MQTT:: No change, not sending value :: %s ::", buff);
  //    return;
  //  }
//  AddInfo(obj, buff, "mod", mod.mode);
//  AddInfo(obj, buff, "ctr", mod.ctrl);
//  AddInfo(obj, buff, "out", mod.out);
  AddValue(obj, buff, "_out", mod._out);

  _mod = mod;
};

static void AddAnalogInfo(JsonObject &obj, const char* inBuff, const char* typeName, float val) {
  char buff[30];
  sprintf(buff, "%s/%s", inBuff, typeName);
  obj[buff] = val;
};

static void AddAnalogModule(JsonObject &obj, const char *equipClass, const char *equipName, const char* instName, ioAnalog_typ &mod) {
  char buff[30];
  sprintf(buff, "%s/%s/%s", equipClass, equipName, instName);
  AddAnalogInfo(obj, buff, "raw", mod.raw);
  AddAnalogInfo(obj, buff, "smt", mod.smoothed);
  AddAnalogInfo(obj, buff, "scl", mod.scaled);
};
#endif

/*
  -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
  -3 : MQTT_CONNECTION_LOST - the network connection was broken
  -2 : MQTT_CONNECT_FAILED - the network connection failed
  -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
  0 : MQTT_CONNECTED - the client is connected
  1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
  2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
  3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
  4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
  5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect
*/
