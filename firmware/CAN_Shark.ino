#include "src/Sparkfun_Shield_Custom/Canbus.h" // don't forget to include these
#include "src/Sparkfun_Shield_Custom/defaults.h"
#include "src/Sparkfun_Shield_Custom/global.h"
#include "src/Sparkfun_Shield_Custom/mcp2515.h"
#include "src/Sparkfun_Shield_Custom/mcp2515_defs.h"
#include <ArduinoJson.h>

char UserInput;
int data;
String message;

void setup()
{
  Serial.begin(115200);

  if (Canbus.init(CANSPEED_500)) /* Initialise MCP2515 CAN controller at the specified speed */
  {
    Serial.println("{\"status\": \"[ OK ]\"}");
  }
  else
  {
    Serial.println("{\"status\": \"[ ERROR ] Unable to initialize MCP2515 CAN controller\"}");
  }

  delay(1000);
}

void loop()
{
  while (Serial.available()) // only send data back if data has been received
  {
    char data = Serial.read(); // read incoming bytes
    message += String(data);   // concat incoming bytes to message string
    if (String(data) == "}")
    {
      StaticJsonDocument<456> doc;                                // will hold json data
      DeserializationError error = deserializeJson(doc, message); // deserialize json data
      //int value = doc["pids"][0];
      delay(30); // rest

      int i;
      String response = "[";
      //Serial.println(message);
      for (i = 0; i < sizeof(doc["pids"]); i++)
      {
        String out = Canbus.ecu_req(doc["pids"][i]); // read message from can bus
        if (out == NULL || out == "" || out == " ")
        {
          response += "\"NULL\",";
        }
        else
        {
          response += out;
        }
        delay(30); // rest
      }
      response.remove(response.length() - 1); // remove trailing commas for parser
      response += "]";
      Serial.println(response); // send message
      delay(30);                // allow buffer to fill
      message = "";             // clear message
    }
    Serial.flush(); // move on when serial is clear

    // String output = ""; // output for serial
    //StaticJsonDocument<200> doc; // will hold json data
    //DeserializationError error = deserializeJson(doc, data); // deserialize json data

    //String out1 = Canbus.ecu_req(doc["pids"][0]);
    //String out2 = Canbus.ecu_req(doc["pids"][1]);

    // if (out1.length() < 1)
    // {
    //   output += "{ \"Engine Coolant Temperature\": \"0\", ";
    // }
    // else
    // {
    //   output += "{ \"Engine Coolant Temperature\": " + out1 + ", ";
    // }
    //memset(buffer, 0, sizeof(buffer));
    //memset(&buffer[0], 0, sizeof(buffer));

    // if (out2.length() < 1)
    // {
    //   output += "{ \"Engine RPM\": \"0\" }";
    // }
    // else
    // {
    //   output += "{ \"Engine RPM\":" + out2 + " }";
    // }
  }
}