//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static Logging ntpLog;

static const char TimeServer[] =            "time.nist.gov";
static const int NTP_PACKET_SIZE =           48;
static const unsigned int                   NTPPort = 8888;

static byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

void initNTP(void) {
  ntpLog.begin(LOG_LEVEL_NOTICE, &Serial);
  ntpLog.setPrefix(printTimestamp);
  ntpLog.setSuffix(printNewline);
  ntpLog.notice("NTP:: Initialized NTP Log ::");
  ntpLog.trace("NTP:: Beginning RTC Initialization");

  if (!_ENABLE_NTP) {
    ntpLog.warning("NTP::NTP Disabled");
    Scheduler::currentScheduler().currentTask().disable();
    return;
  }

  if (_ENABLE_COMMS) {
    ntpLog.notice("NTP:: Comms enabled, beginning clock sync procedure");
    Udp.begin(NTPPort);

    Scheduler::currentScheduler().currentTask().setCallback(&cyclicNTP);
    ntpLog.trace("NTP:: Finished RTC/NTP Initialization");
  }
}

void cyclicNTP(void) {
  logTaskTimer("NTP", &Scheduler::currentScheduler().currentTask());
  if (!gTimeSet && _ENABLE_COMMS) { //Don't send the NTP Packet if we've already set the time (TODO: Find a better way to set the time!)
    ntpLog.warning("NTP:: Sending NTP Packet...");
    sendNTPpacket(TimeServer); //Send the NTP packet to NIST.
    ntpLog.notice("NTP:: Sent NTP Packet...");
    if (Udp.parsePacket()) {// We've received a packet, read the data from it
      ntpLog.warning("NTP:: Rec'd UDP Packet");
      gTimeSet = true; //Mark that we've already set the time!
      Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
      ntpLog.notice("NTP:: Read packet buffer :: Value %b ::", packetBuffer);
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);// the timestamp starts at byte 40 of the received packet and is four bytes,
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);// or two words, long. First, extract the two words:
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      ntpLog.notice("NTP:: Seconds since Jan 1 1900 = %d ::", secsSince1900);
      unsigned long epoch = secsSince1900 - seventyYears + TimeZone * 3600;
      ntpLog.notice("Unix time = " );
      //      RTC.set(epoch); //TODO: get system clock epoch set function
      setTime(epoch);
      PrintTime();
      Scheduler::currentScheduler().currentTask().disable();
    }
  }
}

static void sendNTPpacket(const char * address) {
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
  ntpLog.notice("NTP:: Wrote packet buffer :: Value %b ::", packetBuffer);
  Udp.endPacket();
}

static void PrintTime(void) { //TODO: Print Time!
  char buff[100];
  CStringBuilder sb(buff, sizeof(buff));
  Chronos::DateTime::now().printTo(sb);
  ntpLog.notice("NTP:: Current Time Is %s ::", buff);
}
