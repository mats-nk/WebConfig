#include <WebServer.h>
#include <ESPmDNS.h>


#include <WebConfig.h>

String params = "["
  "{"
  "'name':'ssid',"
  "'label':'Name des WLAN',"
  "'type':"+String(INPUTTEXT)+","
  "'default':''"
  "},"
  "{"
  "'name':'pwd',"
  "'label':'WLAN Passwort',"
  "'type':"+String(INPUTPASSWORD)+","
  "'default':''"
  "},"
  "{"
  "'name':'amount',"
  "'label':'Quantity',"
  "'type':"+String(INPUTNUMBER)+","
  "'min':-10,'max':20,"
  "'default':'1'"
  "},"
  "{"
  "'name':'float',"
  "'label':'Floating point number',"
  "'type':"+String(INPUTTEXT)+","
  "'default':'1.00'"
  "},"
  "{"
  "'name':'area',"
  "'label':'More Text',"
  "'type':"+String(INPUTTEXTAREA)+","
  "'default':'',"
  "'min':40,'max':5"  //min = columns max = rows
  "},"
  "{"
  "'name':'duration',"
  "'label':'Duration',"
  "'type':"+String(INPUTRANGE)+","
  "'min':5,'max':30,"
  "'default':'10'"
  "},"
  "{"
  "'name':'date',"
  "'label':'Datum',"
  "'type':"+String(INPUTDATE)+","
  "'default':'2019-08-14'"
  "},"
  "{"
  "'name':'time',"
  "'label':'Time',"
  "'type':"+String(INPUTTIME)+","
  "'default':'18:30'"
  "},"
  "{"
  "'name':'col',"
  "'label':'Color',"
  "'type':"+String(INPUTCOLOR)+","
  "'default':'#ffffff'"
  "},"
  "{"
  "'name':'switch',"
  "'label':'Switch',"
  "'type':"+String(INPUTCHECKBOX)+","
  "'default':'1'"
  "},"
  "{"
  "'name':'gender',"
  "'label':'Gender',"
  "'type':"+String(INPUTRADIO)+","
  "'options':["
  "{'v':'m','l':'Male'},"
  "{'v':'w','l':'Female'},"
  "{'v':'x','l':'Other'}],"
  "'default':'w'"
  "},"
  "{"
  "'name':'continent',"
  "'label':'Continent',"
  "'type':"+String(INPUTSELECT)+","
  "'options':["
  "{'v':'EU','l':'Europe'},"
  "{'v':'AF','l':'Africa'},"
  "{'v':'AS','l':'Asia'},"
  "{'v':'AU','l':'Australia'},"
  "{'v':'AM','l':'America'}],"
  "'default':'AM'"
  "},"
  "{"
  "'name':'weekday',"
  "'label':'Weekday',"
  "'type':"+String(INPUTMULTICHECK)+","
  "'options':["
  "{'v':'0','l':'Sunday'},"
  "{'v':'1','l':'Monday'},"
  "{'v':'2','l':'Tuesday'},"
  "{'v':'3','l':'Wednesday'},"
  "{'v':'4','l':'Thursday'},"
  "{'v':'5','l':'Friday'},"
  "{'v':'6','l':'Saturday'}],"
  "'default':''"
  "}"
  "]";

WebServer server;
WebConfig conf;

boolean initWiFi() {
    boolean connected = false;
    WiFi.mode(WIFI_STA);
    Serial.print("Connection to ");
    Serial.print(conf.values[0]);
    Serial.println(" to restore");
    if (conf.values[0] != "") {
      WiFi.begin(conf.values[0].c_str(),conf.values[1].c_str());
      uint8_t cnt = 0;
      while ((WiFi.status() != WL_CONNECTED) && (cnt<20)){
        delay(500);
        Serial.print(".");
        cnt++;
      }
      Serial.println();
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("IP-Adress = ");
        Serial.println(WiFi.localIP());
        connected = true;
      }
    }
    if (!connected) {
          WiFi.mode(WIFI_AP);
          WiFi.softAP(conf.getApName(),"",1);  
    }
    return connected;
}

void handleRoot() {
  conf.handleFormRequest(&server);
  if (server.hasArg("SAVE")) {
    uint8_t cnt = conf.getCount();
    Serial.println("*********** Configuration ************");
    for (uint8_t i = 0; i<cnt; i++) {
      Serial.print(conf.getName(i));
      Serial.print(" = ");
      Serial.println(conf.values[i]);
    }
    if (conf.getBool("switch")) Serial.printf("%s %s %i %5.2f \n",
                                conf.getValue("ssid"),
                                conf.getString("continent").c_str(), 
                                conf.getInt("amount"), 
                                conf.getFloat("float"));
  }
}

void setup() {
  Serial.begin(74880);
  Serial.println(params);
  conf.setDescription(params);
  conf.readConfig();
  initWiFi();
  char dns[30];
  sprintf(dns,"%s.local",conf.getApName());
  if (MDNS.begin(dns)) {
    Serial.println("MDNS responder gestartet");
  }
  server.on("/",handleRoot);
  server.begin(80);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
//  MDNS.update();
}
