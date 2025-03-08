#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address
float GyroX, GyroY, GyroZ;
const int windowSize = 10; // Number of samples for the moving average
float gyroXBuffer[windowSize] = {0};
float gyroYBuffer[windowSize] = {0};
float gyroZBuffer[windowSize] = {0};
int bufferIndex = 0;

void setup() {
  Wire.begin();
  Serial.begin(19200);

  // Initialize MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // Talk to the power management register
  Wire.write(0x00);  // Wake the MPU6050
  Wire.endTransmission(true);

void loop() {
  byte error, address;
  int nDevices = 0;

  // Scan for I2C devices
  Serial.println("Scanning I2C devices...");
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
  }
  // Read gyroscope values from MPU6050 and apply moving averages
  readGyroscopeWithMovingAverage();

  delay(1000); // Wait for 1 second
}

// Function to read gyroscope values and apply moving averages
void readGyroscopeWithMovingAverage() {
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro X-axis high byte address
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Request 6 bytes for gyro X, Y, Z

  if (Wire.available() == 6) {
    // Read raw gyroscope data
    float rawGyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // Convert to degrees/sec
    float rawGyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
    float rawGyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;

    // Update the buffers
    gyroXBuffer[bufferIndex] = rawGyroX;
    gyroYBuffer[bufferIndex] = rawGyroY;
    gyroZBuffer[bufferIndex] = rawGyroZ;

    // Compute moving averages
    GyroX = calculateMovingAverage(gyroXBuffer);
    GyroY = calculateMovingAverage(gyroYBuffer);
    GyroZ = calculateMovingAverage(gyroZBuffer);

    // Increment buffer index and wrap around
    bufferIndex = (bufferIndex + 1) % windowSize;

    // Print filtered values
    Serial.print("Gyro X (Filtered): ");
    Serial.print(GyroX);
    Serial.print(" | Gyro Y (Filtered): ");
    Serial.print(GyroY);
    Serial.print(" | Gyro Z (Filtered): ");
    Serial.println(GyroZ);
  } else {
    Serial.println("Error reading gyroscope data.");
  }
}

// Function to calculate moving average of a buffer
float calculateMovingAverage(float buffer[]) {
  float sum = 0.0;
  for (int i = 0; i < windowSize; i++) {
    sum += buffer[i];
  }
  return sum / windowSize;
}
