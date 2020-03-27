#define _DEBUG
//#define _DEBUG_VERBOSE

#ifdef _DEBUG
#define log(s) {Serial.println(F(s));}
#define _BASE_CYCLE_TIME 20
#else
#define log(s)
#define _BASE_CYCLE_TIME 10
#endif

//TaskScheduler Compiler Flags
#define _TASK_TIMECRITICAL          // Enable monitoring scheduling overruns
#define _TASK_SLEEP_ON_IDLE_RUN     // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST        // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
// #define _TASK_WDT_IDS            // Compile with support for wdt control points and task ids
#define _TASK_LTS_POINTER           // Compile with support for local task storage pointer
// #define _TASK_PRIORITY           // Support for layered scheduling priority
// #define _TASK_MICRO_RES          // Support for microsecond resolution
// #define _TASK_STD_FUNCTION       // Support for std::function (ESP8266 ONLY)
#define _TASK_DEBUG                 // Make all methods and variables public for debug purposes
// #define _TASK_INLINE             // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
#define _TASK_TIMEOUT               // Support for overall task timeout
// #define _TASK_OO_CALLBACKS       // Support for callbacks via inheritance
// #define _TASK_DEFINE_MILLIS      // Force forward declaration of millis() and micros() "C" style
#define _TASK_EXPOSE_CHAIN          // Methods to access tasks in the task chain

#include <TaskSchedulerSleepMethods.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskScheduler.h>
//#include <Time.h>
//#include <TimeLib.h>
#include <SD.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ArduinoMqttClient.h>
#include <RTCZero.h>
#include "GlobalDeclarations.h"
#include "P1AM.h"

//Global Function Declarations
void blink1CB();
void ioReadIn();
void ioWriteOut();
//void ATOcb();
//void Alarmscb();
void NTPcb();
void MQTTcb();
void RandomMQTTValueGenerator();
void logTaskTimer(Scheduler* pscheduler, String TaskName);
void logScheduler(Scheduler* pscheduler);
void PrintTime();

////Task Declarations
Scheduler ts;
Task ReadInputs(_BASE_CYCLE_TIME, TASK_FOREVER, &ioReadIn, &ts, true);
Task MyFirstTask(_BASE_CYCLE_TIME, TASK_FOREVER, &blink1CB, &ts, true);
////Task ATO(_BASE_CYCLE_TIME, TASK_FOREVER, &ATOcb, &ts, true);
////Task Alarms(_BASE_CYCLE_TIME, TASK_FOREVER, &Alarmscb, &ts, true);
Task NTP(_BASE_CYCLE_TIME, TASK_FOREVER, &NTPcb, &ts, true);
Task MQTT(_BASE_CYCLE_TIME, TASK_FOREVER, &MQTTcb, &ts, true);
//Task SendRandomMQTTValue(_BASE_CYCLE_TIME*4, TASK_FOREVER, &RandomMQTTValueGenerator, &ts, true);
Task WriteOutputs(_BASE_CYCLE_TIME, TASK_FOREVER, &ioWriteOut, &ts, true);

void setup() {
  Serial.begin(115200);         //initialize serial communication at 0.5M bits per second 
  while (!Serial) {}            //Wait for Serial Port to be opened
  log("Initialized Serial Communications");

  log("Initializing Hardware And Communications");
  while (!P1.init()) {
    log("Waiting For P1 Modules");
    delay(100);
  }
  //P1.printModules();                              //print out all currently signed-on modules to the console
  P1.getFwVersion();                              //print the Base Controller's firmware version to the console
  pinMode(LED_BUILTIN, OUTPUT);                   //Set our LED (Pin 32) to be an output
  pinMode(SWITCH_BUILTIN, INPUT);                 //Set our Switch (Pin 31) to be an input
  int baseErrors = P1.rollCall(baseArray, 3);     //Pass in list of modules and the number to check
  log(String("Base Errors: " + String(baseErrors)).c_str());
  //P1.configWD(10000, TOGGLE);
  
  log("Initialized P1AM-100 Unit.");

  log("Beginning Ethernet Configuration");
  Ethernet.init(5);   //CS pin for P1AM-ETH
  Ethernet.begin(mac);  // Get IP from DHCP
  //  log(String(Ethernet.localIP()).c_str());
  Serial.println(Ethernet.localIP());

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

  //dataFile = SD.open("datalog.txt", FILE_WRITE);

  mqttClient.setUsernamePassword(UserName, Password);  // Username and Password tokens for Shiftr.io namespace. These can be found in the namespace settings.
  log(String("Connecting to the MQTT broker: " + String(broker)).c_str());
  while (!mqttClient.connect(broker, port)) {
    log(String("MQTT connection failed! Error code = " + String(mqttClient.connectError())).c_str());
  }
  log("You're connected to the MQTT broker!");
  for(int ii_MQTT = 0; ii_MQTT < MQTT_CHANNEL_COUNT; ii_MQTT++){
    mqttClient.subscribe(ChannelName[ii_MQTT]); //Subscribe to "InputReading" topic
    log(String("Subscribed To Channel: " + String(ChannelName[ii_MQTT])).c_str());
   }

  Udp.begin(NTPPort);

  rtc.begin();
  

  MyFirstTask.enable();
  ReadInputs.enable();
  WriteOutputs.enable();
  MQTT.enable();

  if (rtc.getEpoch() > 1585302206){
    gTimeSet = true;
    NTP.disable();
    log("Time already set, not enabling NTP");
  }else{
    log("Time not set, enabling NTP");
    NTP.enable();
  }
  PrintTime();
  //P1.startWD();
}

void loop() {
// put your main code here, to run repeatedly:
//  log("Running Task Scheduler...");
ts.execute();
logScheduler(&ts);
//P1.petWD();
}

void blink1CB(void){
  logTaskTimer(&ts, "Blink 1");
  switchState = digitalRead(SWITCH_BUILTIN);//Read the state of the switch
  digitalWrite(LED_BUILTIN, switchState); //Update the LED
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
  #endif;
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
  #endif;
  pscheduler->cpuLoadReset();
}

void ioReadIn(void) {
  String InputWord;
  logTaskTimer(&ts, "Read Inputs");
  inSlot1 = P1.readDiscrete(3);
  inDiscrete_1 = bitRead(inSlot1, 0);
  inWaterLevelSensor_1 = bitRead(inSlot1, 1);
  inWaterLevelSensor_2 = bitRead(inSlot1, 2);
  inWaterLevelSensor_3 = bitRead(inSlot1, 3);

  InputWord = String("Input Word Value: " + String(inSlot1));
}
void RandomMQTTValueGenerator(void){
  int RandomOutputIndex = random(MQTT_CHANNEL_COUNT);
  outMQTTArray[RandomOutputIndex] = rtc.getSeconds();
}

void ioWriteOut(void) {
  String OutputWord;
  //int RandomOutputIndex = random(MQTT_CHANNEL_COUNT);
  logTaskTimer(&ts, "Write Outputs");
  bitWrite(outSlot2, 0, switchState);
  P1.writeDiscrete(outSlot2, 1, 0);
  P1.writeDiscrete(outSlot2, 2, 0);

  OutputWord = String("Output Word: " + String(outSlot2));
}

void MQTTcb(void) {
  logTaskTimer(&ts, "MQTT");
  //if(switchState != _switchState){  // If the state doesn't match the last sent state, update the broker
    for(int ii_MQTT = 0; ii_MQTT < MQTT_CHANNEL_COUNT; ii_MQTT++){
      if(outMQTTArray[ii_MQTT] != _outMQTTArray[ii_MQTT]){
        mqttClient.beginMessage(ChannelName[ii_MQTT]);  //Topic name
        mqttClient.print(String(outMQTTArray[ii_MQTT])); //Value to send
        mqttClient.endMessage();
        log(String("Sent Message On Topic: " + String(ChannelName[ii_MQTT]) + " With Value: " + String(outMQTTArray[ii_MQTT])).c_str());
      }
      _outMQTTArray[ii_MQTT] = outMQTTArray[ii_MQTT];
    }
  //}
  
  _switchState = switchState; //Update our last sent reading
  int mqttValue = checkBroker();  //Check for new messages
  if(mqttValue != -1){  // -1 means we didn't get a new message
  }
}

int checkBroker(){
  String mqttMessage = "";
  int messageValue = 0;

  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    //log(String("Msg Rec'd On Channel: " + String(mqttClient.messageTopic())).c_str());
    for(int ii_MQTT = 0; ii_MQTT < MQTT_CHANNEL_COUNT; ii_MQTT++){
      if(mqttClient.messageTopic() == ChannelName[ii_MQTT]){
        while (mqttClient.available()){
          mqttMessage +=(char)mqttClient.read();
        }
        messageValue =  mqttMessage.toInt();
        log(String("Message From Topic: " + String(ChannelName[ii_MQTT]) + " Value: " + String(mqttMessage)).c_str());
      }
    }
  }else{
    messageValue =  -1;
  }
return messageValue;
}

void NTPcb(){
  logTaskTimer(&ts, "NTP");
  sendNTPpacket(TimeServer);
  
  if (Udp.parsePacket() && !gTimeSet) {
    NTP.disable();
    gTimeSet = true;
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    log(String("Seconds since Jan 1 1900 = " + String(secsSince1900)).c_str());
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears + TimeZone*3600;
    // print Unix time:
    log(String("Unix time = " + String(epoch)).c_str());
    rtc.setEpoch(epoch);
    PrintTime();
  }
}

void PrintTime(void){
  log(String("Time: " + String(rtc.getMonth()) + "/" + String(rtc.getDay()) + "/" + String(rtc.getYear()) + "-" + String(rtc.getHours()) + ":" + String(rtc.getMinutes()) + ":" + String(rtc.getSeconds())).c_str());
}

void sendNTPpacket(const char * address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
