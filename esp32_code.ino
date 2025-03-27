#include "thingProperties.h"
#include <HardwareSerial.h>

// UART settings for STM32 communication
HardwareSerial sensorSerial(1);  // UART1 for STM32
#define RX_PIN 16  // UART RX pin (STM32 TX)
#define TX_PIN 17  // UART TX pin (STM32 RX)

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  sensorSerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  // Initialize the IoT Cloud connection
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Sync the device variables with the cloud
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  // Update Arduino IoT Cloud connection
  ArduinoCloud.update();

  // Check if data is available from STM32 over UART
  if (sensorSerial.available()) {
    String sensorData = sensorSerial.readStringUntil('\n');
    Serial.print("Received data: ");
    Serial.println(sensorData);

    // Assuming sensorData is in format: "Temperature: 23.5, Humidity: 55.0"
    // Parse the received data for temperature and humidity
    int tempIndex = sensorData.indexOf("Temperature: ");
    int humIndex = sensorData.indexOf("Humidity: ");
    
    if (tempIndex != -1 && humIndex != -1) {
      String tempStr = sensorData.substring(tempIndex + 12, sensorData.indexOf(",", tempIndex));
      String humStr = sensorData.substring(humIndex + 10);
      
      // Convert string to float
      temperature = tempStr.toFloat();
      humidity = humStr.toFloat();
      
      // Print parsed data to serial monitor
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Humidity: ");
      Serial.println(humidity);
    }
  }

  delay(2000);  // Adjust the data update frequency
}
