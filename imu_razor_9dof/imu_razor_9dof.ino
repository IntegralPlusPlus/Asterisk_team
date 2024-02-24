/************************************************************
  MPU9250_DMP_Quaternion
  Quaternion example for MPU-9250 DMP Arduino Library
  Jim Lindblom @ SparkFun Electronics
  original creation date: November 23, 2016
  https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

  The MPU-9250's digital motion processor (DMP) can calculate
  four unit quaternions, which can be used to represent the
  rotation of an object.

  This exmaple demonstrates how to configure the DMP to
  calculate quaternions, and prints them out to the serial
  monitor. It also calculates pitch, roll, and yaw from those
  values.

  Development environment specifics:
  Arduino IDE 1.6.12
  SparkFun 9DoF Razor IMU M0

  Supported Platforms:
  - ATSAMD21 (Arduino Zero, SparkFun SAMD21 Breakouts)
*************************************************************/
#include <SparkFunMPU9250-DMP.h>

#define SerialPort SerialUSB

MPU9250_DMP imu;


int adduction(int a) {
  while (a > 180) a -= 360;
  while (a < -180) a += 360;

  return a;
}

void setup()
{
  SerialPort.begin(115200);
  Serial1.begin(115200);

  // Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      SerialPort.println("Unable to communicate with MPU-9250");
      SerialPort.println("Check connections, and try again.");
      SerialPort.println();
      delay(5000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_SEND_RAW_ACCEL | // Send accelerometer data
               DMP_FEATURE_GYRO_CAL       | // Calibrate the gyro data
               DMP_FEATURE_SEND_CAL_GYRO  | // Send calibrated gyro data
               DMP_FEATURE_6X_LP_QUAT     , // Calculate quat's with accel/gyro
               10);
}

void loop()
{
  // Check for new data in the FIFO
  if (imu.fifoAvailable())
  {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if (imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.computeEulerAngles();
      int mapYaw = map(imu.yaw, 0, 360, 0, 254);
      Serial1.write(mapYaw);
      //SerialPort.println(mapYaw);
    }
  }
}
