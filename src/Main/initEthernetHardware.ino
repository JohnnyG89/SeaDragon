void initEthernetHardware(void){
  if(_ENABLE_COMMS){
    log("Beginning Ethernet Configuration");
    Ethernet.init(5);   //CS pin for P1AM-ETH
    Ethernet.begin(mac);  // Get IP from DHCP
    //  log(String(Ethernet.localIP()).c_str());
    Serial.println(Ethernet.localIP());

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
  }

}
