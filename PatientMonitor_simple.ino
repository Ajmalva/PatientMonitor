#include <WiFi.h>
#include <WebServer.h>
#include <HX711.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi Credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// DS18B20 setup (Assume DS18B20 sensors are connected to GPIOs 14, 27, 32)
#define ONE_WIRE_BUS1 14
#define ONE_WIRE_BUS2 27
#define ONE_WIRE_BUS3 32

OneWire oneWire1(ONE_WIRE_BUS1);
OneWire oneWire2(ONE_WIRE_BUS2);
OneWire oneWire3(ONE_WIRE_BUS3);

DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);

// Load Cell Setup (Assume load cells connected to HX711)
HX711 scale1;
HX711 scale2;
HX711 scale3;
const int LOADCELL_DOUT1 = 33;
const int LOADCELL_SCK1 = 25;
const int LOADCELL_DOUT2 = 26;
const int LOADCELL_SCK2 = 13;
const int LOADCELL_DOUT3 = 12;
const int LOADCELL_SCK3 = 15;

// Web Server on port 80
WebServer server(80);

// Variables to store sensor values
float temperature1, temperature2, temperature3;
float weight1, weight2, weight3;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Initialize DS18B20 sensors
  sensors1.begin();
  sensors2.begin();
  sensors3.begin();

  // Initialize Load Cells
  scale1.begin(LOADCELL_DOUT1, LOADCELL_SCK1);
  scale2.begin(LOADCELL_DOUT2, LOADCELL_SCK2);
  scale3.begin(LOADCELL_DOUT3, LOADCELL_SCK3);

  // Tare the scales (set current reading as zero)
  scale1.tare();
  scale2.tare();
  scale3.tare();

  // Define routes for web server
  server.on("/", handleRoot);
  server.on("/patient1", handlePatient1);
  server.on("/patient2", handlePatient2);
  server.on("/patient3", handlePatient3);
  
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // HTML code for home page with buttons
  String html = "<html><head><title>Patient Monitor</title></head><body>";
  html += "<h1>Patient Monitoring System</h1>";
  html += "<button onclick=\"window.location.href='/patient1'\">Patient 1</button><br>";
  html += "<button onclick=\"window.location.href='/patient2'\">Patient 2</button><br>";
  html += "<button onclick=\"window.location.href='/patient3'\">Patient 3</button><br>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handlePatient1() {
  // Read data from DS18B20 and load cell for patient 1
  sensors1.requestTemperatures();
  temperature1 = sensors1.getTempCByIndex(0);
  weight1 = scale1.get_units();

  // HTML code for patient 1 data
  String html = "<html><head><title>Patient 1</title></head><body>";
  html += "<h1>Patient 1 Data</h1>";
  html += "<p>Temperature: " + String(temperature1) + " &#8451;</p>";
  html += "<p>Weight: " + String(weight1) + " kg</p>";
  html += "<button onclick=\"window.location.href='/'\">Back</button>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handlePatient2() {
  // Read data from DS18B20 and load cell for patient 2
  sensors2.requestTemperatures();
  temperature2 = sensors2.getTempCByIndex(0);
  weight2 = scale2.get_units();

  // HTML code for patient 2 data
  String html = "<html><head><title>Patient 2</title></head><body>";
  html += "<h1>Patient 2 Data</h1>";
  html += "<p>Temperature: " + String(temperature2) + " &#8451;</p>";
  html += "<p>Weight: " + String(weight2) + " kg</p>";
  html += "<button onclick=\"window.location.href='/'\">Back</button>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handlePatient3() {
  // Read data from DS18B20 and load cell for patient 3
  sensors3.requestTemperatures();
  temperature3 = sensors3.getTempCByIndex(0);
  weight3 = scale3.get_units();

  // HTML code for patient 3 data
  String html = "<html><head><title>Patient 3</title></head><body>";
  html += "<h1>Patient 3 Data</h1>";
  html += "<p>Temperature: " + String(temperature3) + " &#8451;</p>";
  html += "<p>Weight: " + String(weight3) + " kg</p>";
  html += "<button onclick=\"window.location.href='/'\">Back</button>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}
