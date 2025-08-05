#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

const float VREF = 3.3;        // MCP4725 supply voltage (VCC)
const float MAX_OUTPUT_MV = 50.0; // Max output voltage you want (50 mV)
const uint16_t MAX_DAC_VALUE = 4095; // 12-bit DAC

void setup() {
  Serial.begin(115200);
  Wire.begin();  // SDA: GPIO21, SCL: GPIO22 by default on ESP32-S3

  if (!dac.begin(0x60)) {  // Default I2C address
    Serial.println("Failed to find MCP4725. Check wiring.");
    while (1);
  }

  Serial.println("MCP4725 DAC ready.");
  Serial.println("Enter output voltage in mV (0–50):");
}

void loop() {
  if (Serial.available()) {
    float input_mV = Serial.parseFloat();

    if (input_mV >= 0.0 && input_mV <= MAX_OUTPUT_MV) {
      // Convert mV to DAC value
      uint16_t dac_value = (input_mV / VREF) * MAX_DAC_VALUE / 1000.0;

      dac.setVoltage(dac_value, false);  // false = no EEPROM write

      Serial.print("Output set to: ");
      Serial.print(input_mV);
      Serial.println(" mV");
    } else {
      Serial.println("Invalid input. Enter a value between 0 and 50 mV.");
    }

    // Flush remaining input
    while (Serial.available()) Serial.read();
  }
}
