/*
 * CCS811.h
 *
 *  Created on: Jul 23, 2017
 *      Author: pete
 */

#ifndef CCS811_H_
#define CCS811_H_

#include <cstdint>
#include "CCS811Core.h"

class CCS811 : public CCS811Core {
public:
	CCS811( uint8_t );

	//Call to check for errors, start app, and set default mode 1
	status begin( void );

	status readAlgorithmResults( void );
	bool checkForStatusError( void );
	bool dataAvailable( void );
	bool appValid( void );
	uint8_t getErrorRegister( void );
	uint16_t getBaseline( void );
	status setBaseline( uint16_t );
	status enableInterrupts( void );
	status disableInterrupts( void );
	status setDriveMode( uint8_t mode );
	status setEnvironmentalData( float relativeHumidity, float temperature );
	void setRefResistance( float );
	status readNTC( void );
	uint16_t getTVOC( void );
	uint16_t getCO2( void );
	float getResistance( void );
	float getTemperature( void );

private:
	//These are the air quality values obtained from the sensor
	float refResistance;
	float resistance;
	uint16_t tVOC;
	uint16_t CO2;
	uint16_t vrefCounts = 0;
	uint16_t ntcCounts = 0;
	float temperature;
};

#endif /* CCS811_H_ */
