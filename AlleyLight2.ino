int ledPin = 11;
int switchPin = 12;

bool LEDIsOn = false;

int lightOnHold = 60000;

long lastSwitchLow = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode (ledPin, OUTPUT); // sensor pin INPUT
  pinMode (switchPin, INPUT_PULLUP); // sensor pin INPUT

  Serial.begin(115200); // open the serial port at 9600 bps:

  Serial.print("STARTING UP...\n");
}

void loop() {
  bool switchState = digitalRead(switchPin);

  bool LEDIsOnOldState = LEDIsOn;

  if (!switchState) {
    LEDIsOn = true;
    lastSwitchLow = millis();
    Serial.print("SWITCH PRESSED: " + String(lastSwitchLow) + ".\n");
  }

  if (LEDIsOn && millis() > lastSwitchLow + lightOnHold) {
    LEDIsOn = false;
    Serial.print("LEDS TIMEOUT.\n");
  }

  if (LEDIsOnOldState != LEDIsOn) {
    digitalWrite(ledPin, LEDIsOn);
    Serial.print("LED ON/OFF STATE CHANGE: " + String(LEDIsOn) + "\n");
  }

  delay(1);
}
