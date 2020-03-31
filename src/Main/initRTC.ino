void initRTC(void){
  rtc.begin();
  #ifdef _ENABLE_COMMS
  if (rtc.getEpoch() > 1585302206){
    gTimeSet = true;
    NTP.disable();
    log("Time already set, not enabling NTP");
  }else{
    log("Time not set, enabling NTP");
    NTP.enable();
  }
  #endif
  PrintTime();
  //P1.startWD();
}

void PrintTime(void){
  log(String("Time: " + String(rtc.getMonth()) + "/" + String(rtc.getDay()) + "/" + String(rtc.getYear()) + "-" + String(rtc.getHours()) + ":" + String(rtc.getMinutes()) + ":" + String(rtc.getSeconds())).c_str());
}
