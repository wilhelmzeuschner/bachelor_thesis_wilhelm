/*
This file is part of the LMM-Retrofit project.

Created on: 09. Jun 2022
Author: Hans Wilhelm Zeuschner

Bachelorarbeit:
	"Retrofit eines modernen
	Luftmassenmessers in einen Ottomotor
	mit Bosch L-Jetronic ECU unter
	Verwendung eines Mikrocontrollers"


Fachhochschule Emden/Leer

Studiengang:
Elektrotechnik im Praxisverbund
Sommersemester 2022
Bearbeitungszeitraum: 14.04.2022 bis 22.07.2022

Betreuer: Prof. Dr. Carsten Koch
Zweitprüfer: B. Eng. Ewald Zimmermann
*/

#ifndef LMM_BASIC_IO__
#define LMM_BASIC_IO__

#define LED_1_BIT 0x1
#define LED_2_BIT 0x2
#define LED_ERR_BIT 0x4

#define LED_BLINK_SPEED_SLOT_CHANGE 300

#define BTN_1_MASK	0x1
#define BTN_2_MASK	0x2

void initIOPins();
void initUART(uint8_t *activeLMMSlot);

void handleButtons(uint8_t * activeLMMSlot, uint8_t *ledState);

void setLEDs(uint8_t *ledState);
uint16_t measureVBATTmV();
long readVcc();

void buttonOneISR();
void buttonTwoISR();

uint8_t checkForLMMErrors(uint8_t * ledState);
void setRelay(uint16_t *airADCVal);

#endif