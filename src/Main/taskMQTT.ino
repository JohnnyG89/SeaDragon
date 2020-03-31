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