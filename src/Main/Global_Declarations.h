//Global Function Declarations
void blink1CB();
void ioReadIn();
void ioWriteOut();
void ATOcb();
void Alarmscb();
void NTPcb();
void MQTTcb();
void RandomMQTTValueGenerator();
void CommCheckcb();
void logTaskTimer(Scheduler* pscheduler, String TaskName);
void logScheduler(Scheduler* pscheduler);
void PrintTime();
void LogEntry(String s);
void initEthernetHardware();
void initP1AMHardware();
void initStorageHardware();
void initRTC();
//log(String("" + String()).c_str());

//IO Declarations
uint32_t inSlot1;
uint32_t outSlot2;
bool inDiscrete_1, inWaterLevelSensor_1, inWaterLevelSensor_2, inWaterLevelSensor_3;
bool outATOPump, outSkimmer, outPowerhead_1, outPowerhead_2;
bool switchState, _switchState;
int outMQTTArray[MQTT_CHANNEL_COUNT];
int _outMQTTArray[MQTT_CHANNEL_COUNT];
int inMQTTArray[MQTT_CHANNEL_COUNT];

EthernetClient client;
MqttClient mqttClient(client);
EthernetUDP Udp;
RTCZero rtc;

bool gTimeSet = false;
byte mac[] = {0x60, 0x52, 0xD0, 0x06, 0x68, 0x2E};  // P1AM-ETH have unique MAC IDs on their product label
const char broker[]    = "broker.shiftr.io";  // MQTT Broker URL
//const char broker[]    = "bk0vn6.messaging.internetofthings.ibmcloud.com/";  // MQTT Broker URL
//const char broker[] = "broker.emqx.io";
//const char broker[] = "192.168.10.63";
int port = 1883;
unsigned int NTPPort = 8888;
const char TimeServer[] = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
uint8_t lastSentReading = 0; //Stores last Input Reading sent to the broker
const char UserName[] = "2cc80738";
const char Password[] = "4a2823143f869c33";
//const char UserName[] = "admin";
//const char Password[] = "admin";
//const char ChannelName[] = "testtopic/Topic2/Hello";
const char ChannelName[MQTT_CHANNEL_COUNT][50]= {
{"testtopic/Topic2/Ch0"}, 
{"testtopic/Topic2/Ch1"}, 
{"testtopic/Topic2/Ch2"}, 
{"testtopic/Topic2/Ch3"}, 
{"testtopic/Topic2/Ch4"},
{"testtopic/Topic2/Ch5"},
{"testtopic/Topic2/Ch6"},
{"testtopic/Topic2/Ch7"},
{"testtopic/Topic2/Ch8"},
{"testtopic/Topic2/Ch9"},
{"testtopic/Topic2/Ch10"}, 
{"testtopic/Topic2/Ch11"}, 
{"testtopic/Topic2/Ch12"}, 
{"testtopic/Topic2/Ch13"}, 
{"testtopic/Topic2/Ch14"},
{"testtopic/Topic2/Ch15"},
{"testtopic/Topic2/Ch16"},
{"testtopic/Topic2/Ch17"},
{"testtopic/Topic2/Ch18"},
{"testtopic/Topic2/Ch19"}
};

//Hardware-specific parameters:
char statusByteArray[12];								//Array for second implementation where all bytes are read
bool statusBit;											//Variable to temporarily store individual bits of our Status Bytes
const char* baseArray[] = { "P1-08TD2","P1-08TRS", "P1-08ND3" };	//Expected Modules

//SD Card Related Declarators
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = SDCARD_SS_PIN;
File dataFile;

//Other global parameters:
//time_t t;
