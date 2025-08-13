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
  //IDLE,
  SETUP,
  PAUSE,
  DONE,
  COOKING
};

struct Context {
  StateProgram state;
  StateProgram previous;
  double timer;
  double temp;
  double speed;
  LiquidCrystal_I2C lcd;
};
Context context;

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

void checkTimer() {
  // TODO: If I change the time during pause I want to check if new time is less than time left if it is set it to the new time otherwise keep it there. 
}

double getPotentiometer(int pinNumber) {
  return analogRead(pinNumber);
}

void setPotentiometers() {
  getPotentiometer(TIMER_POT);
  getPotentiometer(TEMP_POT);
  getPotentiometer(SPEED_POT);
}

void checkButtons() {
  if (digitalRead(START_BUTTON)) {
    context.previous = context.state;
    context.state = StateProgram::COOKING;
  } else if (digitalRead(PAUSE_BUTTON)) {
    context.previous = context.state;
    context.state = StateProgram::PAUSE;
  } else if (digitalRead(RESET_BUTTON)) {
    context.previous = context.state;
    context.state = StateProgram::SETUP;
  }
}

void initContext() {
  context.state = StateProgram::SETUP;
  context.previous = NULL;
  context.timer = 0.0;
  context.temp = 0.0;
  context.speed = 0.0;
  context.lcd(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
}

void initLCD() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
}

void setup() {
  pinMode(ACTUATORS, INPUT_PULLUP); // Actuator Button
  pinMode(START_BUTTON, INPUT_PULLUP); // Start Button
  pinMode(PAUSE_BUTTON, INPUT_PULLUP); // Pause Button
  pinMode(RESET_BUTTON, INPUT_PULLUP); // Reset Button

  pinMode(TIMER_POT, INPUT);
  pinmode(TEMP_POT, INPUT);
  pinMode(SPEED_POT, INPUT);
  
  initContext();
  initLCD();
}

void loop() {
  checkButtons();
  // TODO: Temperature Sensor
  switch(stateProgram) {
    case StateProgram::SETUP:
        // TODO: setupLCD();
        setPotentiometers();
        buzzerOff();
      break;
    case StateProgram::PAUSE:
        // TODO: pauseLCD();
        setPotentiometers();
        checkTimer();
        relayOff();
      break;
    case StateProgram::COOKING:
        // TODO: cookingLCD();
        // TODO: Motor Controller Function
        // TODO: Solenoid Valve Controller Function
        relayOn();
      break;
    case StateProgram::DONE:
        // TODO: doneLCD();
        relayOff();
        buzzerOn();
      break;
  }
}
