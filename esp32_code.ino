#include <WiFi.h>

const char* ssid = "Pragvansh";
const char* password = "12345678";
const char* server = "http://your-server.com/api/data";

/* UART setup */
#define RX_PIN 16
#define TX_PIN 17
HardwareSerial mySerial(1);

/* UART buffer */
char uart_buffer[100];

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  /* Connect to Wi-Fi */
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Wi-Fi connected");
}

void loop() {
  if (mySerial.available()) {
    /* Read data from STM32 over UART */
    int len = mySerial.readBytesUntil('\n', uart_buffer, sizeof(uart_buffer) - 1);
    uart_buffer[len] = '\0'; // Null-terminate the string

    /* Print received data to Serial (for debugging) */
    Serial.print("Received data: ");
    Serial.println(uart_buffer);

    /* Send data to cloud server */
    send_data_to_server(uart_buffer);
  }
  delay(1000);
}

/* Send sensor data to cloud server */
void send_data_to_server(const char* data) {
  WiFiClient client;

  if (client.connect(server, 80)) {
    client.println("POST /api/data HTTP/1.1");
    client.println("Host: your-server.com");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(strlen(data));
    client.println();
    client.println(data);
    client.stop();
  }
}
