#include "HCPCA9685.h"
#define I2CAdd 0x40

#define blueLED 2
#define greenLED 3
#define redLED 4

#define xServo 1
#define yServo 0

double kpX = 0.15;
double kiX = 0;//0.0000001;
double kdX = 22500;

double kpY = 0.15;
double kiY = 0;//0.0000001;
double kdY = 22500;

double currentTime = millis();
double prevTime = 0;
double totalErrorX = 0;
double totalErrorY = 0;
double prevErrorX = 0;
double prevErrorY = 0;

HCPCA9685 HCPCA9685(I2CAdd);



double* getBallPos() {
  
  // Computer returns string over serial of the form "X0.5429Y-0.6143"

  String positions;

  do {

    positions = "";

    if (Serial.available()) {
      positions = Serial.readStringUntil('\n');

      if (!positions.charAt(0) == 'X' || positions.indexOf("Y") < 0) {
        positions = "";
      }
      
    }
  } while (positions.compareTo("") == 0);
  

  int xIndicatorPos = positions.indexOf("X");
  int yIndicatorPos = positions.indexOf("Y");
  int newlineIndicatorPos = positions.indexOf("\n");
   
  String xString = positions.substring(xIndicatorPos + 1, yIndicatorPos);
  String yString = positions.substring(yIndicatorPos + 1, newlineIndicatorPos);
  
  static double ret[2];
  ret[0] = yString.toDouble();
  ret[1] = xString.toDouble();

  return ret;
}

double *pid(double ballX, double ballY, double targetX, double targetY) {

  // Define (0, 0) to be the center of the platform
  //Top left is (-1, -1)

  double currentTime = millis();
  double elapsedTime = currentTime - prevTime;

  double errX = targetX - ballX;
  double errY = targetY - ballY;
//
//  if(targetX - ballX < 0) errX *= -1;
//  if(targetY - ballY < 0) errY *= -1;

  totalErrorX += errX * elapsedTime;
  totalErrorY += errY * elapsedTime;

  double rateErrorX = (errX - prevErrorX) / elapsedTime;
  double rateErrorY = (errY - prevErrorY) / elapsedTime;

  prevErrorX = errX;
  prevErrorY = errY;

  double xMotor = kpX * errX + kiX * totalErrorX + kdX * rateErrorX;
  double yMotor = kpY * errY + kiY * totalErrorY + kdY * rateErrorY;

  static double ret[2];
  ret[0] = xMotor;
  ret[1] = yMotor;

  return ret;
}

//x and y inputs range [-1, 1]
void moveServos(double x, double y) {

  //Imperfections in robot
  x = x + 0.07;

  if(x > 1) x = 1;
  if(y > 1) y = 1;
  if (x < -1) x = -1;
  if (y < -1) y = -1;

  //The servos can realistically move from 0-300
  //This scales the range the range to allow inputs (-1, 1)

  HCPCA9685.Servo(xServo, (x + 1) * 150);
  HCPCA9685.Servo(yServo, (y + 1) * 150);
}

void setup() {
  Serial.begin(9600);

  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);

  moveServos(0, 0);

  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  double* ballPos = getBallPos();

  double* motorPos = pid(ballPos[0], ballPos[1], 0, 0);

  Serial.println(ballPos[0]);

  moveServos(motorPos[0], motorPos[1]);

}