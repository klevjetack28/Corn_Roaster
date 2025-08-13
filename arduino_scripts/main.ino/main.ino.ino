#define RESET_BUTTON 3
#define PAUSE_BUTTON 2
#define START_BUTTON 1
// Some PWM pin for buzzer
#define BUZZER 4 
// LCD with driver
#define WIRE1 A0
#define WIRE2 A1
// ---------------
#define TEMP_POT A2
#define TIMER_POT A3
#define SPEED_POT A4

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
  // LCD
};
Context context;

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
        // TODO: setPotentiometers();
      break;
    case StateProgram::PAUSE:
        // TODO: pauseLCD();
        // TODO: setPotentiometers();
        // TODO: Motor Controller Function
        // TODO: Solenoid Valve Controller Function
      break;
    case StateProgram::COOKING:
        // TODO: cookingLCD();
        // TODO: Turn Off Relay
      break;
    case StateProgram::DONE:
        // TODO: doneLCD();
        // TODO: buzzer();
      break;
  }
}
