//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

const unsigned int NTPPort = 8888;
const char TimeServer[] = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

void initNTP(void) {

  if(!_ENABLE_NTP){
    prglog("NTP Disabled");
    tskNTP.disable();
    return;
  }
  
  prglog("Beginning RTC Initialization");

//  rtc.begin();

  if (_ENABLE_COMMS) {
    prglog("Comms enabled, beginning clock sync procedure");
    if (rtc.getEpoch() > 1585302206) {
      prglog("Time already set, not enabling NTP");
    } else {
      prglog("Time not set, enabling NTP");
    } 
    PrintTime();
  } else {
    prglog("Comms disabled, Not syncing RTC");
  }
  Udp.begin(NTPPort);
  tskNTP.setCallback(&cyclicNTP);

  prglog("Finished RTC Initialization");
}

void cyclicNTP(void) {
  logTaskTimer("NTP");
//  prglog("In the NTP Task?");
//  return;
  if (!gTimeSet && _ENABLE_COMMS) { //Don't send the NTP Packet if we've already set the time (TODO: Find a better way to set the time!)
    prglog("Sending NTP Packet...");
    sendNTPpacket(TimeServer); //Send the NTP packet to NIST.
    prglog("Sent NTP Packet...");
    if (Udp.parsePacket()) {// We've received a packet, read the data from it
      prglog("Rec'd UDP Packet");
      gTimeSet = true; //Mark that we've already set the time!
      Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);// the timestamp starts at byte 40 of the received packet and is four bytes,
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);// or two words, long. First, extract the two words:
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      prglog(String("Seconds since Jan 1 1900 = " + String(secsSince1900)).c_str());
      unsigned long epoch = secsSince1900 - seventyYears + TimeZone * 3600;
      // print Unix time:
      prglog(String("Unix time = " + String(epoch)).c_str());
      rtc.setEpoch(epoch); //TODO: get system clock epoch set function
      PrintTime();
      tskNTP.disable();
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

void PrintTime(void) { //TODO: Print Time!
  prglog(String("Time: " + String(rtc.getMonth()) + "/" + String(rtc.getDay()) + "/" + String(rtc.getYear()) + "-" + String(rtc.getHours()) + ":" + String(rtc.getMinutes()) + ":" + String(rtc.getSeconds())).c_str());
}
