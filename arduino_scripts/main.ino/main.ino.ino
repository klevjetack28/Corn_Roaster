#define RESET_BUTTON 3
#define PAUSE_BUTTON 2
#define START_BUTTON 1
// LCD with driver
#define WIRE1 A0
#define WIRE2 A1
// ---------------
#define TEMP_POTENTIOMETER A2
#define TIMER_POTENTIOMETER A3
#define SPEED_POTENTIOMETER A4

enum StateProgram {
  //IDLE,
  SETUP,
  PAUSE,
  DONE,
  COOKING
};

StateProgram stateProgram = StateProgram::SETUP;

void checkButtons() {
  if (digitalRead(START_BUTTON)) {
    stateProgram = StateProgram::COOKING;
  } else if (digitalRead(PAUSE_BUTTON)) {
    stateProgram = StateProgram::PAUSE;
  } else if (digitalRead(RESET_BUTTON)) {
    statePogram = StateProgram::SETUP;
  }
}

void setup() {
  pinMode(ACTUATORS, INPUT_PULLUP); // Actuator Button
  pinMode(START_BUTTON, INPUT_PULLUP); // Start Button
  pinMode(PAUSE_BUTTON, INPUT_PULLUP); // Pause Button
  pinMode(RESET_BUTTON, INPUT_PULLUP); // Reset Button
  
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
