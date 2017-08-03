/*
 * CCS811Core.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: pete
 */

#include <onion-i2c.h>
#include "CCS811Core.h"

CCS811Core::CCS811Core(uint8_t inputArg)  : I2CAddress(inputArg)
{
}

CCS811Core::~CCS811Core()
{
}

status CCS811Core::beginCore()
{
	CCS811Core::status returnError = SENSOR_SUCCESS;

	Wire.begin();

	//Spin for a few ms
	volatile uint8_t temp = 0;
	for( uint16_t i = 0; i < 10000; i++ )
	{
		temp++;
	}

	while ( Wire.available() ) //Clear wire as a precaution
	{
		Wire.read();
	}

	//Check the ID register to determine if the operation was a success.
	uint8_t readCheck;
	readCheck = 0;
	returnError = readRegister(CSS811_HW_ID, &readCheck);

	if( returnError != SENSOR_SUCCESS ) return returnError;

	if( readCheck != 0x81 )
	{
		returnError = SENSOR_ID_ERROR;
	}

	return returnError;


}
