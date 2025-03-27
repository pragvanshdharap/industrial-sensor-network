#include <ArduinoIoTCloud.h>
#include <WiFiConnectionManager.h>

const char SSID[]     = "pragvansh";
const char PASS[]     = "12345678";

float temperature;
float humidity;

void initProperties() {
  ArduinoCloud.addProperty(temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(humidity, READ, ON_CHANGE, NULL);
}

WiFiConnectionManager ArduinoIoTPreferredConnection(SSID, PASS);
