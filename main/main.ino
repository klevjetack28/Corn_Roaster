// Digital
#define RESET_BUTTON 3
#define PAUSE_BUTTON 2
#define START_BUTTON 1
// Some PWM pin for buzzer
#define BUZZER 4 
#define RELAY 5

// Analog 
#define TEMP_POT A2
#define TIMER_POT A3
#define SPEED_POT A4

// LCD
#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define LCD_ADDRESS 0x27
#define MSG_BUF LCD_HEIGHT

enum StateProgram {
  SETUP,
  PAUSE,
  DONE,
  COOKING
};

struct Context {
  StateProgram state;
  StateProgram previous;
  double timer;
  double timeLeft;
  double temp;
  double speed;
  LiquidCrystal_I2C lcd;
  unsigned long deltaTime;
};
Context context;

void initContext() {
  context.state = StateProgram::SETUP;
  context.previous = NULL;
  context.timer = 0.0;
  context.timeLeft = 0.0;
  context.temp = 0.0;
  context.speed = 0.0;
  context.lcd(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
  context.deltaTime = 0.0;
}

void relayOn() {
  digitalWrite(RELAY, HIGH);
}

void relayOff() {
  digitalWrite(RELAY, LOW);
}

void buzzerOn() {
  digitalWrite(BUZZER, HIGH);
}

void buzzerOff() {
  digitalWrite(BUZZER, LOW);
}

void setTimer(double potentiometerValue) {
  // TODO: Used to translate the timer potentiometer value into seconds
  // TODO: Chcek if new time is less than or equal to time left if it is change both otherwise just chhange context.timer
}

void setTemp(double potentiometerValue) {
  // TODO: Used to translate the temp potentiometer value into some output for solenoid valve
}

void setSpeed(double potentiometerValue) {
  // TODO: Used to translate the speed potentiometer value into speed value for motors
}

void countDownTimer() {
  unsigned long now = millis();
  if (now - context.deltaTime >= 1000) {
    context.timeLeft -= 1
  }
}

bool checkTimerDone() {
  return context.timeLeft == 0;
}

double getPotentiometer(int pinNumber) {
  return analogRead(pinNumber);
}

void setPotentiometers() {
  setTimer(getPotentiometer(TIMER_POT));
  setTemp(getPotentiometer(TEMP_POT));
  setSpeed(getPotentiometer(SPEED_POT));
}

void checkButtons() {
  if (digitalRead(START_BUTTON)) {
    context.previous = context.state;
    context.state = StateProgram::COOKING;
    relayOn();
  } else if (digitalRead(PAUSE_BUTTON)) {
    context.previous = context.state;
    context.state = StateProgram::PAUSE;
    relayOff();
  } else if (digitalRead(RESET_BUTTON)) {
    context.previous = context.state;
    context.state = StateProgram::SETUP;
    initContext();
    relayOff();
  }
}

void initLCD() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
}

void setupLCD() {
  lcd.setCursor(0, 0);
  
}

void pauseLCD() {

}

void cookingLCD() {

}

void doneLCD() {

}

void setup() {
  pinMode(ACTUATORS, INPUT_PULLUP); // Actuator Button
  pinMode(START_BUTTON, INPUT_PULLUP); // Start Button
  pinMode(PAUSE_BUTTON, INPUT_PULLUP); // Pause Button
  pinMode(RESET_BUTTON, INPUT_PULLUP); // Reset Button

  pinMode(TIMER_POT, INPUT);
  pinMode(TEMP_POT, INPUT);
  pinMode(SPEED_POT, INPUT);
  
  initContext();
  initLCD();
}

void loop() {
  checkButtons();
  // TODO: Temperature Sensor

  // Potentially need to run this 10fps or some other rate.
  
  switch(stateProgram) {
    case StateProgram::SETUP:
        // TODO: setupLCD();
        setPotentiometers();
        buzzerOff();
      break;
    case StateProgram::PAUSE:
        // TODO: pauseLCD();
        setPotentiometers();
        checkTimerValue();
      break;
    case StateProgram::COOKING:
        // TODO: cookingLCD();
        // TODO: Motor Controller Function
        // TODO: Solenoid Valve Controller Function
        countDownTimer();
        if (checkTimerDone()) {
          context.previous = context.state;
          context.state = StateProgram::DONE;
        }
      break;
    case StateProgram::DONE:
        // TODO: doneLCD();
        relayOff();
        buzzerOn();
      break;
  }
}
