#ifndef SOLENOID_SERVER_H
#define SOLENOID_SERVER_H
#ifdef ESP8266
#include <esp8266wifi.h>
#include <ESP8266WebServer.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
#endif

#include <DNSServer.h>
#include "config.h"

class SolenoidServer
{
public:
  /**
   * Listen - check for and serve clients
   */
  void Listen(Configuration config);
  bool output_state = false;
  void begin();
};

#endif