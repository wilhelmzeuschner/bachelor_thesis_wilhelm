/*
This file is part of the LMM-Retrofit project.

Created on: 18. Jun 2022
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

#include <Arduino.h>
#include <Wire.h>

#include "lmm_pins.h"
#include "lmm_basic_io.h"
#include "lmm_uart_config.h"
#include "lmm_dac_sig.h"
#include "lmm_digi_pot.h"
#include "lmm_config.h"
#include "lmm_electronic_sensor.h"

/*********************************************************************/
/**
 \brief Reads the temperature of the electronic sensor (HFM 5)
		Deprecated: no longer used since the NTC thermistor is connected directly

 \param	-

 \return  temperature in milli °C

 **********************************************************************/
uint16_t readEletrSensTemperatureMilliDegreeCelsius()
{
	uint16_t temperature = 0;
	uint16_t temperatureADCVal = 0;

	for (uint8_t i = 0; i < CONFIG_TEMPERATURE_AVG_COUNT; i++)
	{
		temperatureADCVal += analogRead(TEMP_SENSE_PIN);
	}
	temperatureADCVal = temperatureADCVal / CONFIG_TEMPERATURE_AVG_COUNT;

	if (temperatureADCVal == 0)
	{
		Serial.println(F("Thermistor doesn't seem to be connected."));
	}
	else
	{
		Serial.print("temperatureADCVal: ");
		Serial.println(temperatureADCVal);

		// Convert ADC value into temperature
		temperature = EL_SENSOR_TEMPERATURE_COEFF_X * temperatureADCVal + EL_SENSOR_TEMPERATURE_COEFF_OFFSET;

		Serial.print("temperature: ");
		Serial.println(temperature);
	}

	return temperature;
}

/*********************************************************************/
/**
 \brief Measures the analog output of the electronic sensor (HFM 5) (air mass passing through)

 \param		airADCVal ADC value (call by reference)

 \return  	-

 **********************************************************************/
void readElectrSensAir(uint16_t *airADCVal)
{
	*airADCVal = 0;
	for (uint16_t i = 0; i < CONFIG_AIR_SENS_AVG_COUNT; i++)
	{
		*airADCVal += analogRead(SIG_SENSE_PIN);
	}
	*airADCVal = *airADCVal / CONFIG_AIR_SENS_AVG_COUNT;
}