//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

#if _USE_PUBSUB

static Logging rpcLog;
static EthernetClient  	ethclient_rpc;
static const char* 		rpcClientID = "SeaDragon_RPC";
static const char* rpcUserID = "PutYourUserIDHere";
static const char* rpcPassword = "PutYourPasswordHere";
static const char* rpcBroker = "broker.shiftr.io";
static const char* rpcTopic = "sd_api";
static const int 	rpcPort = 1883;
static PubSubClient	rpcClient(rpcBroker, rpcPort, rpcCallback, ethclient_rpc);
static const int rpcPrintBuffer = 100;
static const int rpcDocSize = 200;

void initRPC(void) {
  if (!gEthernetConnectionActive) {
    rpcLog.warning("RPC:: No ethernet comm yet.  Returning to try again on next pass ::");
    return;
  }

  rpcLog.begin(LOG_LEVEL_NOTICE, &Serial);
  rpcLog.setPrefix(printTimestamp);
  rpcLog.setSuffix(printNewline);
  rpcLog.notice("RPC:: Initialized RPC Log ::");
  rpcLog.trace("RPC:: Beginning RPC Initialization");

  if (rpcClient.connect(rpcClientID, rpcUserID, rpcPassword)) {
    rpcLog.notice("RPC:: Connected to %s:%d :: State %d :: Username %s :: Password %s :: ClientID %s ::", rpcBroker, rpcPort, rpcClient.state(), rpcUserID, rpcPassword, rpcClientID);
  } else {
    rpcLog.error("RPC:: Not able to connect to %s :: State %d :: Trying again on next pass ::", rpcBroker, rpcClient.state());
    return;
  }

  bool ret = rpcClient.subscribe(rpcTopic);
//return;

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicRPC);
  rpcLog.trace("RPC:: Finished RPC Initialization");
}

void cyclicRPC(void) {
  logTaskTimer("RPC", &Scheduler::currentScheduler().currentTask());
//return;
  if (!rpcClient.connected()) {
    rpcLog.warning("RPC:: Disconnected :: Connecting to RPC Client ::");
    Scheduler::currentScheduler().currentTask().setCallback(&initRPC);
  }

  rpcClient.loop();
}

static void rpcCallback(char* topic, byte* payload, unsigned int length) {
  logTaskTimer("RPC", &Scheduler::currentScheduler().currentTask());
  char printbuff[rpcPrintBuffer];
  CStringBuilder sb_print(printbuff, sizeof(printbuff));
  StaticJsonDocument<rpcDocSize> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    rpcLog.notice("RPC:: JSON Parse Error :: Callback Error %s :: Payload %s :: Length %d :: Text :%s ::", topic, payload, length, error.c_str());
    return;
  }

  const JsonObject &data = doc.template as<JsonObject>();

  const char* methodName = data["msg"]["method"];
  const char* params = data["msg"]["params"];
  bool setState = strcmp(params, "true") == 0;
  int numBytes = serializeJson(doc, sb_print);
  //String sMethod("btn/pmp/skm/1/ctrl/_Remote");


  rpcLog.notice("RPC:: Callback %s :: Payload %s :: Length %d :: method %s :: params %s :: state %T :: Bytes %d :: %s ::", topic, payload, length, methodName, params, setState, numBytes, printbuff);

  if (strcmp(methodName, "sd/btn/pmp/skm/1/ctrl/_Remote") == 0) {
    ioSkimmer.ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/pmp/wvm/1/ctrl/_Remote") == 0) {
    ioWavemaker.ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/pmp/pwh/1/ctrl/_Remote") == 0) {
    ioPowerhead.ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/pmp/ret/1/ctrl/_Remote") == 0) {
    ioReturn.ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/dsp/afd/1/ctrl/_Remote") == 0) {
    ioAutofeeder[0].ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/dsp/afd/2/ctrl/_Remote") == 0) {
    ioAutofeeder[1].ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/dsp/dos/1/ctrl/_Remote") == 0) {
    ioDosePump[0].ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/dsp/dos/2/ctrl/_Remote") == 0) {
    ioDosePump[1].ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/dsp/ato/1/ctrl/_Remote") == 0) {
    ioATOPump.ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/lit/slt/1/ctrl/_Remote") == 0) {
    ioSaltwaterLight.ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/lit/fsh/1/_Remote/set") == 0) {
    rpcLog.notice("RPC:: Found it MOFO ::");
    ioFreshwaterLight.ctrl._Remote = setState;
  }

  if (strcmp(methodName, "sd/btn/lit/fug/1/ctrl/_Remote") == 0) {
    ioFugeLight.ctrl._Remote = setState;
  }


};
#endif
