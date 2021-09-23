int irPowerPin = 10;
int irSensor1Pin = A4;
//int irSensor2Pin = A1;
int lightVoltagePin = A2;
int ledPin = 11;

bool IRIsOn = false;
bool trustIr = false;
bool LEDIsOn = false;

int lightThreshold = 600;

long lastTrustedDetection = 0;
int lightOnHold = 15000;

long lastCheck = 0;
int checkInterval = 500;

void setup()
{
  pinMode (irPowerPin, OUTPUT); // sensor pin INPUT
  pinMode (irSensor1Pin, INPUT); // sensor pin INPUT
  //pinMode (irSensor2Pin, INPUT); // sensor pin INPUT
  pinMode (lightVoltagePin, INPUT); // sensor pin INPUT
  pinMode (ledPin, OUTPUT); // sensor pin INPUT
  Serial.begin(115200); // open the serial port at 9600 bps:

  Serial.print("STARTING UP...\n");
}

bool IRDesiredOn() {
  int lightCount = analogRead(lightVoltagePin);
  Serial.print("Light/1024: " + String(lightCount) + ".\n");
  return (lightCount >= lightThreshold);
}

bool LEDDesiredOn() {
  return TrustedIRDetected();
}

bool IRDetected() {
  int ir1 = analogRead(irSensor1Pin);
  //int ir2 = analogRead(irSensor2Pin);

  Serial.print("IR DETECTED: " + String(ir1) + ".\n");
  //Serial.print("IR DETECTED: " + String(ir1) + "/" + String(ir2) + ".\n");

  bool IR1Detected = analogRead(irSensor1Pin) > 512;
  //bool IR2Detected = analogRead(irSensor2Pin) > 512;

  if (IRIsOn && !IR1Detected && !trustIr) {
    //if (IRIsOn && !IR1Detected && !IR2Detected && !trustIr) {
    TrustIR(true);
  }

  Serial.print("IR DETECTED: " + String(IR1Detected) + ". TRUST: " + trustIr + ".\n");
  //Serial.print("IR DETECTED: " + String(IR1Detected) + "/" + String(IR2Detected) + ". TRUST: " + trustIr + ".\n");

  //return IR1Detected || IR2Detected;
  return IR1Detected;
}

bool TrustedIRDetected() {
  bool trustedIRDetected = 0;

  if (IRDetected()) {
    Serial.print("TRUSTED IR DETECTED: " + String(trustIr) + ".\n");
    if (trustIr) {
      trustedIRDetected = true;
      lastTrustedDetection = millis();
    } else {
      //Not trusted.
    }
  } else {
    //Ok.
  }

  return trustedIRDetected;
}

void TrustIR(bool trust) {
  trustIr = trust;
  Serial.print("TRUST IR: " + String(trust) + ".\n");
}

void IROn(bool on) {
  digitalWrite(irPowerPin, on); //Turn the IR sensor ON/OFF.
  IRIsOn = on;
  Serial.print("IR SENSOR: " + String(on) + ".\n");

  if (!IRIsOn && LEDIsOn) {
    LEDsOn(LOW);
  } else {
    //Ok.
  }

  TrustIR(false);
}

void LEDsOn(bool on) {
  Serial.print("LEDS: " + String(on) + ".\n");
  LEDIsOn = on;
  digitalWrite(ledPin, on); //Turn LEDS ON/OFF.
}

void loop()
{
  if (millis() > lastCheck + checkInterval) {
    if (millis() > lastTrustedDetection + lightOnHold) {
      if (IRDesiredOn()) {
        if (IRIsOn) {
          //Ok.
          if (LEDDesiredOn()) {
            if (LEDIsOn) {
              //Ok.
            } else {
              LEDsOn(HIGH);
            }
          } else {
            if (LEDIsOn) {
              LEDsOn(LOW);
            } else {
              //Ok.
            }
          }
        } else {
          IROn(HIGH);
        }
      } else {
        if (IRIsOn) {
          IROn(LOW);
        } else {
          //Ok.
        }
      }
    } else {
      Serial.print("SUSPEND CHECKS.\n");
    }

    lastCheck = millis();
  }
}
