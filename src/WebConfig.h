/*
File WebConfig.h
Version 1.4
Author Gerald Lechner
contakt lechge@gmail.com

Description
This library builds a web page with a smart phone friendly form to edit
a free definable number of configuration parameters.
The submitted data will bestored in the SPIFFS
The library works with ESP8266 and ESP32

Dependencies:
  ESP8266WebServer.h
  ArduinoJson.h
*/

#ifndef WebConfig_h
#define WebConfig_h

#include <Arduino.h>
#if defined(ESP32)
  #include <WebServer.h>
#else
  #include <ESP8266WebServer.h>
#endif

#define MAXVALUES 20              // Maximum number of parameters
#define MAXOPTIONS 15             // Maximum number of options per parameters
#define NAMELENGTH 20             // Character limits
#define LABELLENGTH 40
#define CONFFILE "/WebConf.conf"  // Name for the config file

#define INPUTTEXT 0
#define INPUTPASSWORD 1
#define INPUTNUMBER 2
#define INPUTDATE 3
#define INPUTTIME 4
#define INPUTRANGE 5
#define INPUTCHECKBOX 6
#define INPUTRADIO 7
#define INPUTSELECT 8
#define INPUTCOLOR 9
#define INPUTFLOAT 10
#define INPUTTEXTAREA 11
#define INPUTMULTICHECK 12
#define INPUTTYPES 13             // Number of types

#define BTN_CONFIG 0
#define BTN_DONE 1
#define BTN_CANCEL 2
#define BTN_DELETE 4

typedef                           // Data structure to hold the parameter Description
struct  {
  char name[NAMELENGTH];
  char label[LABELLENGTH];
  uint8_t type;
  int min;
  int max;
  uint8_t optionCnt;
  String options[MAXOPTIONS];
  String labels[MAXOPTIONS];
} DESCRIPTION;

class WebConfig {
public:
  WebConfig();
  void setDescription(String parameter);    // Load form descriptions
  void addDescription(String parameter);    // Add extra descriptions

// Function to respond a HTTP request for the form use the filename to save.
#if defined(ESP32)
  void handleFormRequest(WebServer * server, const char * filename);
  // Function to respond a HTTP request for the form use the default file to save and restart ESP after saving the new config
  void handleFormRequest(WebServer * server);
  // Get the index for a value by parameter name
#else
  void handleFormRequest(ESP8266WebServer * server, const char * filename);
  // Function to respond a HTTP request for the form use the default file to save and restart ESP after saving the new config
  void handleFormRequest(ESP8266WebServer * server);
  // Get the index for a value by parameter name
#endif
  int16_t getIndex(const char * name);                     // Read configuration from file
  boolean readConfig(const char *  filename);              // Read configuration from default file
  boolean readConfig();                                    // Write configuration to file
  boolean writeConfig(const char *  filename);             // Write configuration to default file
  boolean writeConfig();                                   // Delete configuration file
  boolean deleteConfig(const char *  filename);            // Delete default configutation file
  boolean deleteConfig();                                  // Get a parameter value by its name
  const String getString(const char * name);
  const char * getValue(const char * name);
  int getInt(const char * name);
  float getFloat(const char * name);
  boolean getBool(const char * name);                      // Get the accesspoint name
  const char * getApName();                                // Get the number of parameters
  uint8_t getCount();                                      // Get the name of a parameter
  String getName(uint8_t index);                           // Get results as a JSON string
  String getResults();                                     // Set values from a JSON string
  void setValues(String json);                             // Set the value for a parameter
  void setValue(const char*name,String value);             // Set the label for a parameter
  void setLabel(const char * name, const char* label);     // Remove all options
  void clearOptions(uint8_t index);
  void clearOptions(const char * name);                    // Add a new option
  void addOption(uint8_t index, String option);
  void addOption(uint8_t index, String option, String label);                       // Modify an option
  void setOption(uint8_t index, uint8_t option_index, String option, String label);
  void setOption(char * name, uint8_t option_index, String option, String label);   // Get the options count
  uint8_t getOptionCount(uint8_t index);
  uint8_t getOptionCount(char * name);                     // Set form type to doen cancel
  void setButtons(uint8_t buttons);                        // Register onSave callback
  void registerOnSave(void (*callback)(String results));   // Register onSave callback
  void registerOnDone(void (*callback)(String results));   // Register onSave callback
  void registerOnCancel(void (*callback)());               // Register onSave callback
  void registerOnDelete(void (*callback)(String name));
  String values[MAXVALUES];                                // Values for the parameter
private:
  char _buf[1200];
  uint8_t _count;
  String _apName;
  uint8_t _buttons = BTN_CONFIG;
  DESCRIPTION _description[MAXVALUES];
  void (*_onSave)(String results) = NULL;
  void (*_onDone)(String results) = NULL;
  void (*_onCancel)() = NULL;
  void (*_onDelete)(String name) = NULL;
};

#endif
