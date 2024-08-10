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

#ifndef LMM_CONFIG__
#define LMM_CONFIG__


//Battery (supply) voltage measurement
//Resistor values (the absolute value isn't important, the ratio matters!)
#define CONFIG_R1_VBATT						475
#define CONFIG_R2_VBATT						221

#define CONFIG_ADC_MAX						1023
#define CONFIG_VBATT_AVG_COUNT				2				//Sets how many measurements are made and averaged

#define CONFIG_TEMPERATURE_AVG_COUNT		1				//Sets how many measurements are made and averaged

#define CONFIG_AIR_SENS_AVG_COUNT			8				//Sets how many measurements are made and averaged


#define CONFIG_VBATT_NOMINAL_MILLIVOLT				12000	//Nominal battery voltage in mV
#define CONFIG_VBATT_ACCEPTABLE_DEVIATON_PERCENT	0.3		//How may percent is the battery voltage allowed to deviate from the nominal 12 V before it gets regarded as an issue


#define CONFIG_VBATT_NOMINAL_LOWER			(int)(CONFIG_VBATT_NOMINAL_MILLIVOLT * (1 - CONFIG_VBATT_ACCEPTABLE_DEVIATON_PERCENT))
#define CONFIG_VBATT_NOMINAL_UPPER			(int)(CONFIG_VBATT_NOMINAL_MILLIVOLT * (1 + CONFIG_VBATT_ACCEPTABLE_DEVIATON_PERCENT))

#define CONFIG_ERR_LED_BLINK_INTERVAL		500

#define CONFIG_EEPROM_SLOT_ADDR				0x0


#define CONFIG_RELAY_ADC_ON_VAL				280
#define CONFIG_RELAY_HYSTERESIS				50

#endif