#ifndef __KEYCARD_READER_MAIN_H
#define __KEYCARD_READER_MAIN_H

#define TRUE (1)
#define FALSE (0)

#include "state_machine.h"

StateMachine state_machine;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	// SPI protocol used by mfrc522 hardware
	SPI.begin();
	// initialize mfrc522 hardware
	mfrc522.PCD_Init();
	// LED configuration
	pinMode(LED_GRANTED_PIN, OUTPUT);
	pinMode(LED_DENIED_PIN, OUTPUT);
	pinMode(LED_PROG_PIN, OUTPUT);
	// SETUP LCD
	// Set display type as 16 char, 2 rows
	lcd.begin(LCD_COLS, LCD_ROWS);
	// Print on first row
	lcd.setCursor(0, 0);
	lcd.clear();
	// SETUP BackgroundController
	pinMode(MASTER_CONTROLLER_PIN, INPUT);
	// SETUP servo
	myservo.attach(SERVO_PIN);
}

void loop() {
	// put your main code here, to run repeatedly:
	int button_state = digitalRead(BACKGROUND_CONTROLLER_PIN);
	
	state_machine.UpdateResetModules(button_state);
	state_machine.Update();
}

#endif // __KEYCARD_READER_MAIN_H