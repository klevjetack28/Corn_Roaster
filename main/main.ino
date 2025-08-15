#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Digital
#define RESET_BUTTON 4
#define PAUSE_BUTTON 3
#define START_BUTTON 2
// Some PWM pin for buzzer
#define BUZZER 5
#define RELAY 6

// Analog 
#define TEMP_POT A0
#define TIMER_POT A1
#define SPEED_POT A2

// LCD
#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define LCD_ADDRESS 0x27
#define MSG_BUF LCD_HEIGHT

// Constants
#define POTENTIOMETER_MAX 1024
#define MAX_TIMER 3600
//#define MAX_TEMP XXX maybe 200 250?
#define MAX_SPEED 10.0

enum class StateProgram {
  SETUP,
  PAUSE,
  DONE,
  COOKING
};

enum class Button {
  START,
  PAUSE,
  RESET,
  NONE
};

struct Context {
  StateProgram state;
  StateProgram previous;
  Button lastButton;
  double timer;
  double timeLeft;
  bool timerSet;
  double temp;
  double speed;
  LiquidCrystal_I2C lcd;
  unsigned long lastTick;
  unsigned long lcdFrames;

  Context()
  : state(StateProgram::SETUP),
    previous(StateProgram::SETUP),
    lastButton(Button::NONE),
    timer(0.0f),
    timeLeft(0.0f),
    timerSet(false),
    temp(0.0f),
    speed(0.0f),
    lcd(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT),
    lastTick(0),
    lcdFrames(0)
  {}
} context;

void resetContext() {
  context.state = StateProgram::SETUP;
  context.previous = StateProgram::SETUP;
  context.lastButton = Button::NONE;
  context.timer = 0.0;
  context.timeLeft = 0.0;
  context.timerSet = false;
  context.temp = 0.0;
  context.speed = 0.0;
  context.lastTick = 0;
  context.lastFrames = 0;
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
  context.timer = (potentiometerValue / POTENTIOMETER_MAX) * MAX_TIMER;
  // TODO: Chcek if new time is less than or equal to time left if it is change both otherwise just chhange context.timer
  // TODO: increment in increments of 5 seconds. 
}

void setTemp(double potentiometerValue) {
  // TODO: Used to translate the temp potentiometer value into some output for solenoid valve
  // TODO: Increment in increments of 1
  // TODO: Max temp unknown
}

void setSpeed(double potentiometerValue) {
  Serial.print("Speed: ");
  context.speed = (potentiometerValue / POTENTIOMETER_MAX) * MAX_SPEED;
  Serial.println(context.speed);
  // TODO: Increase in increments of 0.1
}

void countDownTimer() {
  unsigned long now = millis();
  if (now - context.lastTick >= 1000) {
    context.timeLeft -= 1;
    context.lastTick = now;
    Serial.println(context.timeLeft);
  }
}

void getTime(double time, int& minutes, int& seconds) {
  minutes = time / 60;
  seconds = time % 60;
}

void setTimeLeft() {
  if (!context.timerSet) {
    context.timerSet = true;
    context.timeLeft = context.timer;
  }
}

bool checkTimerDone() {
  return context.timeLeft <= 0;
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
  if (!digitalRead(START_BUTTON) && (context.lastButton != Button::START)) {
    context.previous = context.state;
    context.state = StateProgram::COOKING;
    context.lastButton = Button::START;
    setTimeLeft();
    relayOn();
  } else if (!digitalRead(PAUSE_BUTTON) && (context.lastButton != Button::PAUSE)) {
    context.previous = context.state;
    context.state = StateProgram::PAUSE;
    context.lastButton = Button::PAUSE;
    relayOff();
  } else if (!digitalRead(RESET_BUTTON) && (context.lastButton != Button::RESET)) {
    context.previous = context.state;
    context.state = StateProgram::SETUP;
    context.lastButton = Button::RESET;
    resetContext();
    relayOff();
  }
}

void initLCD() {
  context.lcd.init();
  context.lcd.backlight();
  context.lcd.setCursor(0, 0);
  context.lcd.print("Hello World!");
}

void mainLCD() {
  context.lcd.setCursor(0, 0);
  context.lcd.print("TIME   TEMP  SPD");
  context.lcd.setCursor(1, 0);
  char meta[LCD_WIDTH];
  int minutes, seconds;
  getTime(context.timer, &minutes, &seconds);
  sprintf(meta, "%d:%d  %d   %f", minutes, seconds, comtext.temp, context.speed);
  context.lcd.print(meta);
}

void cookingLCD() {
  context.lcd.setCursor(0, 0);
  context.lcd.print(banner("TIME LEFT       "));
  context.lcd.setCursor(1, 0);
  char time[LCD_WIDTH];
  int minutes, seconds;
  getTime(context.timeLeft, &minutes, &seconds);
  sprintf(time, "      %d:%d     ", minutes, seconds);
  context.lcd.print(time);
}

void doneLCD() {
  context.lcd.setCursor(0, 0);
  context.lcd.print(banner("DONE    DONE    "));
  context.lcd.setCursor(1, 0),
  context.lcd.print(banner("    DONE    DONE"));
}

char* banner(char* in) {
  char out[LCD_WIDTH];
  int j = 0;
  for (int i = context.lcdFrames; i < LCD_WIDTH; i++, j++) {
    out[j] = in[i];
  }
  for (int i = 0; i < context.lcdFrames; i++, j++) {
    out[j] = in[i];
  }
  return out;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(START_BUTTON, INPUT_PULLUP); // Start Button
  pinMode(PAUSE_BUTTON, INPUT_PULLUP); // Pause Button
  pinMode(RESET_BUTTON, INPUT_PULLUP); // Reset Button

  pinMode(TIMER_POT, INPUT);
  pinMode(TEMP_POT, INPUT);
  pinMode(SPEED_POT, INPUT);

  pinMode(BUZZER, OUTPUT);
  
  initLCD();
}

void loop() {
  checkButtons();
  // TODO: Temperature Sensor
  // Potentially need to run this 10fps or some other rate.
  switch(context.state) {
    case StateProgram::SETUP:
        mainLCD();
        setPotentiometers();
        buzzerOff();
      break;
    case StateProgram::PAUSE:
        mainLCD();
        setPotentiometers();
      break;
    case StateProgram::COOKING:
        cookingLCD();
        // TODO: Motor Controller Function
        // TODO: Solenoid Valve Controller Function
        countDownTimer();
        if (checkTimerDone()) {
          context.previous = context.state;
          context.state = StateProgram::DONE;
        }
      break;
    case StateProgram::DONE:
        doneLCD();
        Serial.println("BUZZZZZZZZ");
        relayOff();
        buzzerOn();
      break;
  }
}
