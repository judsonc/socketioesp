#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <ArduinoJson.h>


StaticJsonBuffer<200> jsonBuffer;


SocketIOClient client;
const char* ssid     = "Robotica-IMD";
const char* password = "roboticawifi";

char host[] = "saiot.ect.ufrn.br";
int port = 80;
char nsp[] = "dispositivo";

extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0;
long interval = 5000;
unsigned long lastreply = 0;
unsigned long lastsend = 0;
String JSON;
JsonObject& root = jsonBuffer.createObject();

void setup() {

  root["sensor"] = "gps";
  root["time"] = 1351824120;
  JsonArray& data = root.createNestedArray("data");
  data.add(double_with_n_digits(48.756080, 6));
  data.add(double_with_n_digits(2.302038, 6));
  root.printTo(JSON);
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!client.connect(host, port, nsp)) {
    Serial.println("connection failed");
    return;
  }
  if (client.connected())
  {
    //client.send("connection", "message", "Connected !!!!");
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    //client.heartbeat(0);

    //Serial.println(JSON);
    //client.send("i am client", "Informacoes vindas do ESP8266", "Time please?");
    //client.sendJSON("JSON", JSON);

    lastsend = millis();
  }
  if (client.monitor())
  {
    lastreply = millis();
    Serial.print("RID: ");
    Serial.println(RID);
    if (RID == "action") {
      Serial.print("200\n\n Rcontent: ");
      Serial.println(Rcontent);
      Serial.print("Rname: ");
      Serial.println(Rname);
    }
    if (RID == "atime" && Rname == "time")
    {
      Serial.print("Il est ");
      Serial.println(Rcontent);
    }
  }
}
