//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

const char broker[]      = "192.168.10.49";
const int port = 1883;
const char UserName[] = "Q6DHx3GAflxJnxPzN3FX";
const char Password[] = "LocalUbuntuUser_9483";
//const char ChannelName[] = "testtopic/Topic2/Hello";
const char ChannelName[MQTT_CHANNEL_COUNT][50] = {
  {"JohnnyG89/feeds/Feed1"},
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

int outMQTTArray[MQTT_CHANNEL_COUNT], _outMQTTArray[MQTT_CHANNEL_COUNT], inMQTTArray[MQTT_CHANNEL_COUNT];
bool MessageTxRxTracker[MQTT_CHANNEL_COUNT];
bool switchState, _switchState;

void initMQTT(void) {
  prglog("Beginning MQTT Configuration");

//  Ethernet.init(5);   //CS pin for P1AM-ETH
//  Ethernet.begin(mac);  // Get IP from DHCP
//  Serial.println(Ethernet.localIP());

  if(!gEthernetConnectionActive){
    prglog("MQTT Task: Ethernet Connection Not Active-Returning to try again later");
    return;
  }else{

  mqttClient.setUsernamePassword(UserName, Password);  // Username and Password tokens for Shiftr.io namespace. These can be found in the namespace settings.

  prglog(String("Connecting to the MQTT broker: " + String(broker)).c_str());
  if (!mqttClient.connect(broker, port)) {
    prglog(String("MQTT connection failed! Error code = " + String(mqttClient.connectError())).c_str());
  } else {
    prglog("You're connected to the MQTT broker!");
//    gEthernetConnectionActive = true;
    for (int ii_MQTT = 0; ii_MQTT < MQTT_CHANNEL_COUNT; ii_MQTT++) {
      mqttClient.subscribe(ChannelName[ii_MQTT]); //Subscribe to "InputReading" topic
      prglog(String("Subscribed To Channel: " + String(ChannelName[ii_MQTT])).c_str());
    }
  }

  tskMQTT.setCallback(&cyclicMQTT);
  }
  prglog("Initialized MQTT Task");
}

void cyclicMQTT(void) {
  logTaskTimer(&ts_low, "MQTT", "Low");
  return;
  
  if (gEthernetConnectionActive) {
    RandomMQTTValueGenerator();

    for (int ii_MQTT = 0; ii_MQTT < MQTT_CHANNEL_COUNT; ii_MQTT++) {
      if (outMQTTArray[ii_MQTT] != _outMQTTArray[ii_MQTT]) {

        if (MessageTxRxTracker[ii_MQTT]) {
          prglog(String("Channel: " + String(ii_MQTT) + " sent message before last one was acknowledged").c_str());
        } else {
          MessageTxRxTracker[ii_MQTT]=true;
        }

        mqttClient.beginMessage(ChannelName[ii_MQTT]);  //Topic name
        mqttClient.print(String(outMQTTArray[ii_MQTT])); //Value to send
        mqttClient.endMessage();
        if (_LOG_MQTT) {
          prglog(String("Sent Message On Topic: " + String(ChannelName[ii_MQTT]) + " With Value: " + String(outMQTTArray[ii_MQTT])).c_str());
        }
      }
      _outMQTTArray[ii_MQTT] = outMQTTArray[ii_MQTT];
    }
    _switchState = switchState; //Update our last sent reading
    int mqttValue = checkBroker();  //Check for new messages
    if (mqttValue != -1) { // -1 means we didn't get a new message
    }
  }

}

void RandomMQTTValueGenerator(void) {
  int RandomOutputIndex = random(MQTT_CHANNEL_COUNT);
  //outMQTTArray[RandomOutputIndex] = rtc.getSeconds();
  outMQTTArray[RandomOutputIndex]++;
}

int checkBroker() {
  String mqttMessage = "";
  int messageValue = 0;

  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    //prglog(String("Msg Rec'd On Channel: " + String(mqttClient.messageTopic())).c_str());
    for (int ii_MQTT = 0; ii_MQTT < MQTT_CHANNEL_COUNT; ii_MQTT++) {
      if (mqttClient.messageTopic() == ChannelName[ii_MQTT]) {
        MessageTxRxTracker[ii_MQTT] = false;
        while (mqttClient.available()) {
          mqttMessage += (char)mqttClient.read();
        }
        messageValue =  mqttMessage.toInt();
        if (_LOG_MQTT) {
          prglog(String("Message From Topic: " + String(ChannelName[ii_MQTT]) + " Value: " + String(mqttMessage)).c_str());
        }
      }
    }
  } else {
    messageValue =  -1;
  }
  return messageValue;
}
