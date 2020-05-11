//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static        Logging       tbrpcLog;

static const  char*         TOKEN                     = "VqSbL0ENWMvPUA7GTLBd";
static const  char*         THINGSBOARD_SERVER        = "192.168.1.200";

static const  size_t        	callbacks_size            = 2;
static        RPC_Callback  	callbacks[callbacks_size] = {
  { "setBtn_FreshwaterLight",   		setBtn_FreshwaterLight},
  { "getBtn_FreshwaterLight",   		getBtn_FreshwaterLight}};
  /*
  { "setBtn_CabinetLight",     			setBtn_CabinetLight},
  { "getBtn_CabinetLight",     			getBtn_CabinetLight},
  { "setBtn_Skimmer",     				setBtn_Skimmer},
  { "getBtn_Skimmer",     				getBtn_Skimmer},
  { "setBtn_WaveMaker",     			setBtn_WaveMaker},
  { "getBtn_WaveMaker",     			getBtn_WaveMaker},
  { "setBtn_Powerhead",     			setBtn_Powerhead},
  { "getBtn_Powerhead",     			getBtn_Powerhead},
  { "setBtn_SaltwaterLightOutput",     	setBtn_SaltwaterLight},
  { "getBtn_SaltwaterLightOutput",     	getBtn_SaltwaterLight},
  { "setBtn_ReturnPump",     			setBtn_ReturnPump},
  { "getBtn_ReturnPump",     			getBtn_ReturnPump},
  { "setBtn_FugeLight",     			setBtn_FugeLight},
  { "getBtn_FugeLight",     			getBtn_FugeLight},
  { "setBtn_AutoFeeder_1",     			setBtn_AutoFeeder_1},
  { "getBtn_AutoFeeder_1",     			getBtn_AutoFeeder_1},
  { "setBtn_AutoFeeder_2",     			setBtn_AutoFeeder_2},
  { "getBtn_AutoFeeder_2",     			getBtn_AutoFeeder_2},
  { "setBtn_DosePump_1",     			setBtn_DosePump_1},
  { "getBtn_DosePump_1",     			getBtn_DosePump_1},
  { "setBtn_DosePump_2",     			setBtn_DosePump_2},
  { "getBtn_DosePump_2",     			getBtn_DosePump_2},
  { "setBtn_ATOPump",     				setBtn_ATOPump},
  { "getBtn_ATOPump",     				getBtn_ATOPump}
};
*/

void initThingsBoard_RPC(void) {

  tbrpcLog.begin(LOG_LEVEL_NOTICE, &Serial);
  tbrpcLog.setPrefix(printTimestamp);
  tbrpcLog.setSuffix(printNewline);
  tbrpcLog.notice("ThingsBoard_RPC:: Initialized ThingsBoard_RPC Log ::");
  tbrpcLog.notice("ThingsBoard_RPC:: Initialized ThingsBoard_RPC Log ::");
  tbrpcLog.trace("ThingsBoard_RPC:: Beginning ThingsBoard_RPC Initialization ::");

  if (!gEthernetConnectionActive) {
    tbrpcLog.warning("ThingsBoard_RPC:: Ethernet Connection Not Active-Returning to try again later ::");
    return;
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    tbrpcLog.notice("ThingsBoard_RPC:: Connecting to %s with token %s ::", THINGSBOARD_SERVER, TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      tbrpcLog.fatal("ThingsBoard_RPC:: Failed to connect ::");
      return;
    } else {
      tbrpcLog.notice("ThingsBoard_RPC:: Connected to broker at %s with token %s ::", THINGSBOARD_SERVER, TOKEN);
    }
  }
  tbrpcLog.notice("ThingsBoard_RPC:: Subscribing to RPC ::");
  if (!tb.RPC_Subscribe(callbacks, callbacks_size)) {
    tbrpcLog.fatal("ThingsBoard_RPC:: Failed to Subscribe to RPC ::");
    return;
  }
  tbrpcLog.notice("ThingsBoard_RPC:: Subscribed to RPC ::");

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicThingsBoard_RPC);
  tbrpcLog.trace("ThingsBoard_RPC:: Finished ThingsBoard_RPC Initialization ::");
}

void cyclicThingsBoard_RPC(void) {
  logTaskTimer("ThingsBoard_RPC", &Scheduler::currentScheduler().currentTask());

  if (!tb.connected()) {
    Scheduler::currentScheduler().currentTask().setCallback(&initThingsBoard_RPC);
    tbrpcLog.notice("ThingsBoard_RPC:: Disconnected from ThingsBoard_RPC Server :: Returning To Initialization ::");
    return;
  }

  tb.loop();
}

//FreshwaterLight
static RPC_Response setBtn_FreshwaterLight(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_FreshwaterLight Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_FreshwaterLight Callback :: Data %T ::", Incoming);
  }
  rmt_oFreshwaterLight = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_FreshwaterLight(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_FreshwaterLight Callback ::");
  
  return RPC_Response("value", rmt_oFreshwaterLight);
}

//Cabinet Light
static RPC_Response setBtn_CabinetLight(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_CabinetLight Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_CabinetLight Callback :: Data %T ::", Incoming);
  }
  rmt_oCabinetLight = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_CabinetLight(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_CabinetLight Callback ::");
  
  return RPC_Response("value", rmt_oCabinetLight);
}

//Skimmer
static RPC_Response setBtn_Skimmer(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_Skimmer Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_Skimmer Callback :: Data %T ::", Incoming);
  }
  rmt_oSkimmer = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_Skimmer(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_Skimmer Callback ::");
  
  return RPC_Response("value", rmt_oSkimmer);
}

//WaveMaker
static RPC_Response setBtn_WaveMaker(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_WaveMaker Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_WaveMaker Callback :: Data %T ::", Incoming);
  }
  rmt_oWaveMaker = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_WaveMaker(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_WaveMaker Callback ::");
  
  return RPC_Response("value", rmt_oWaveMaker);
}

//WaveMaker
static RPC_Response setBtn_Powerhead(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_Powerhead Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_Powerhead Callback :: Data %T ::", Incoming);
  }
  rmt_oPowerhead = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_Powerhead(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_Powerhead Callback ::");
  
  return RPC_Response("value", rmt_oPowerhead);
}

//SaltwaterLight
static RPC_Response setBtn_SaltwaterLight(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_SaltwaterLight Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_SaltwaterLight Callback :: Data %T ::", Incoming);
  }
  rmt_oSaltwaterLight = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_SaltwaterLight(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_SaltwaterLight Callback ::");
  
  return RPC_Response("value", rmt_oSaltwaterLight);
}

//ReturnPump
static RPC_Response setBtn_ReturnPump(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_ReturnPump Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_ReturnPump Callback :: Data %T ::", Incoming);
  }
  rmt_oReturnPump = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_ReturnPump(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_ReturnPump Callback ::");
  
  return RPC_Response("value", rmt_oReturnPump);
}

//FugeLight
static RPC_Response setBtn_FugeLight(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_FugeLight Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_FugeLight Callback :: Data %T ::", Incoming);
  }
  rmt_oFugeLight = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_FugeLight(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_FugeLight Callback ::");
  
  return RPC_Response("value", rmt_oFugeLight);
}

//AutoFeeder_1
static RPC_Response setBtn_AutoFeeder_1(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_AutoFeeder_1 Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_AutoFeeder_1 Callback :: Data %T ::", Incoming);
  }
  rmt_oAutoFeeder[0] = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_AutoFeeder_1(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_AutoFeeder_1 Callback ::");
  
  return RPC_Response("value", rmt_oAutoFeeder[0]);
}

//AutoFeeder_2
static RPC_Response setBtn_AutoFeeder_2(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_AutoFeeder_2 Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_AutoFeeder_2 Callback :: Data %T ::", Incoming);
  }
  rmt_oAutoFeeder[1] = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_AutoFeeder_2(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_AutoFeeder_2 Callback ::");
  
  return RPC_Response("value", rmt_oAutoFeeder[1]);
}

//DosePump_1
static RPC_Response setBtn_DosePump_1(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_DosePump_1 Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_DosePump_1 Callback :: Data %T ::", Incoming);
  }
  rmt_oDosePump[0] = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_DosePump_1(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_DosePump_1 Callback ::");
  
  return RPC_Response("value", rmt_oDosePump[0]);
}

//AutoFeeder_2
static RPC_Response setBtn_DosePump_2(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_DosePump_2 Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_DosePump_2 Callback :: Data %T ::", Incoming);
  }
  rmt_oDosePump[1] = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_DosePump_2(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_DosePump_2 Callback ::");
  
  return RPC_Response("value", rmt_oDosePump[1]);
}

//ATOPump
static RPC_Response setBtn_ATOPump(const RPC_Data &data) {
  bool Incoming = data;

  if(data.isNull()){
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_ATOPump Callback :: Null Packet ::");
  }else{
    tbrpcLog.notice("ThingsBoard_RPC:: Received setBtn_ATOPump Callback :: Data %T ::", Incoming);
  }
  rmt_oATOPump = Incoming;
  
  return RPC_Response("value", data);
}

static RPC_Response getBtn_ATOPump(const RPC_Data &data) {
  tbrpcLog.notice("ThingsBoard_RPC:: Received getBtn_ATOPump Callback ::");
  
  return RPC_Response("value", rmt_oATOPump);
}

static RPC_Response rpcCheckStatus(const RPC_Data &data) {
  almLog.notice("ThingsBoard_RPC:: rpcCheckStatus Recd Call :: Data=%s", "tmp1");

  return RPC_Response("checkStatus", iMachineModeSwitchWord);
}
