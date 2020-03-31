//Global pre-compiler flags and symbols
#include "Global_Defines.h"                

//Pre-built libraries
#include <TaskSchedulerSleepMethods.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskScheduler.h>
#include <SD.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ArduinoMqttClient.h>
#include <RTCZero.h>
#include <P1AM.h>

//Global variable, object, and prototype definitions
#include "Global_Declarations.h"
#include "Task_Declarations.h"

void setup() {
  Serial.begin(115200);     //Initialize serial communication at 0.5M bits per second 
  while (!Serial) {}        //Wait for Serial Port to be opened
  log("Initialized Serial Communications");

  initP1AMHardware();       //Initialize the P1AM IO modules.  

  initEthernetHardware();   //Initialize communications and protocol objects

  initStorageHardware();    //Initialize SD card, data logging functionality

  initRTC();                //Initialize the RTC and the scheduler
}

void loop() {
  ts.execute();
  logScheduler(&ts);
  //P1.petWD();
}

void blink1CB(void){
  logTaskTimer(&ts, "Blink 1");
  switchState = digitalRead(SWITCH_BUILTIN);//Read the state of the switch
  digitalWrite(LED_BUILTIN, switchState); //Update the LED
}

void ATOcb(void){
  logTaskTimer(&ts, "Read Inputs");  
}

void Alarmscb(void){
  logTaskTimer(&ts, "Read Inputs");
  
}

void CommCheckcb(void){
  logTaskTimer(&ts, "Read Inputs");
}

void ioReadIn(void) {
  logTaskTimer(&ts, "Read Inputs");
  inSlot1 = P1.readDiscrete(3);
  inDiscrete_1 = bitRead(inSlot1, 0);
  inWaterLevelSensor_1 = bitRead(inSlot1, 1);
  inWaterLevelSensor_2 = bitRead(inSlot1, 2);
  inWaterLevelSensor_3 = bitRead(inSlot1, 3);
}
void RandomMQTTValueGenerator(void){
  int RandomOutputIndex = random(MQTT_CHANNEL_COUNT);
  outMQTTArray[RandomOutputIndex] = rtc.getSeconds();
}

void ioWriteOut(void) {
  logTaskTimer(&ts, "Write Outputs");
  bitWrite(outSlot2, 0, switchState);
  bitWrite(outSlot2, 1, ReadInputs.isEnabled());
  bitWrite(outSlot2, 2, client.connected());
  bitWrite(outSlot2, 3, client.available());
  bitWrite(outSlot2, 4, mqttClient.connected());
  bitWrite(outSlot2, 5, false);
  bitWrite(outSlot2, 6, false);
  bitWrite(outSlot2, 7, false);

  
  P1.writeDiscrete(outSlot2, 1, 0);
  //P1.writeDiscrete(outSlot2, 2, 0);
}

void NTPcb(){
  logTaskTimer(&ts, "NTP");
  
  if(!gTimeSet && _ENABLE_COMMS){//Don't send the NTP Packet if we've already set the time (TODO: Find a better way to set the time!)
    sendNTPpacket(TimeServer); //Send the NTP packet to NIST.
    if (Udp.parsePacket()) {// We've received a packet, read the data from it
      NTP.disable();
      gTimeSet = true; //Mark that we've already set the time!
      Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);// the timestamp starts at byte 40 of the received packet and is four bytes,
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);// or two words, long. First, extract the two words:
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      log(String("Seconds since Jan 1 1900 = " + String(secsSince1900)).c_str());
      unsigned long epoch = secsSince1900 - seventyYears + TimeZone*3600;
      // print Unix time:
      log(String("Unix time = " + String(epoch)).c_str());
      rtc.setEpoch(epoch);
      PrintTime();
    }
  }
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
