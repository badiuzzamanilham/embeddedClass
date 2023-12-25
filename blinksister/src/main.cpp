#define BLYNK_TEMPLATE_ID "TMPL6Nzow09vL"
#define BLYNK_TEMPLATE_NAME "IoTubes"
#define BLYNK_AUTH_TOKEN "3H7pQA3k0nM5mcZINYwRnmNjasGlnpCW"

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONEWIRE_BUS 4
#define TURBIDITY_PIN 2

char auth[] = "3H7pQA3k0nM5mcZINYwRnmNjasGlnpCW";  // Ganti dengan token otentikasi Blynk Anda
char ssid[] = "nona";   // Ganti dengan nama WiFi Anda
char pass[] = "12345678";

OneWire oneWire(ONEWIRE_BUS);
DallasTemperature sensors(&oneWire);
BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  // Tambahkan inisialisasi koneksi WiFi di sini
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Inisialisasi Blynk
  Blynk.begin(auth, ssid, pass);

  sensors.begin();
}

void loop() {
  Blynk.run();

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");

  int turbidityValue = analogRead(TURBIDITY_PIN);
  int turbidity = map(turbidityValue, 0, 2000, 100, 0);
  Serial.print("Turbidity Asli: ");
  Serial.println(turbidityValue);
  Serial.print("Turbidity Mapping: ");
  Serial.println(turbidity);

  const int ph_pin = 33;
  float Po = 0;
  float PH_step;
  int nilai_analog_PH;
  double teganganPH;

  float PH4 = 3.1;
  float PH7 = 2.5;
  nilai_analog_PH = analogRead(ph_pin);
  Serial.print("Nilai ADC PH : ");
  Serial.println(nilai_analog_PH);
  teganganPH = 3.3 / 4095 * nilai_analog_PH;
  Serial.print("TeganganPH : ");
  Serial.println(teganganPH, 3);

  PH_step = (PH4 - PH7) / 3;
  Po = 7.00 + ((PH7 - teganganPH) / PH_step);
  Serial.print("Nilai PH Cairan : ");
  Serial.println(Po, 2);
 
  delay(3000);

  Blynk.virtualWrite(V0, temperatureC);
  Blynk.virtualWrite(V1, turbidity);
  Blynk.virtualWrite(V2, Po);
}
