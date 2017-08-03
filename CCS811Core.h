/*
 * CCS811Core.h
 *
 *  Created on: Jul 23, 2017
 *      Author: pete
 */

#ifndef CCS811CORE_H_
#define CCS811CORE_H_

#include <cstdint>

class CCS811Core {
public:

	// Return values
	typedef enum
	{
		SENSOR_SUCCESS,
		SENSOR_ID_ERROR,
		SENSOR_I2C_ERROR,
		SENSOR_INTERNAL_ERROR,
		SENSOR_GENERIC_ERROR
		//...
	} status;


	CCS811Core( uint8_t );
	~CCS811Core() = default;

	status beginCore( void );

	//***Reading functions***//

	//readRegister reads one 8-bit register
	status readRegister( uint8_t offset, uint8_t* outputPointer);
	//multiReadRegister takes a uint8 array address as input and performs
	//  a number of consecutive reads
	status multiReadRegister(uint8_t offset, uint8_t *outputPointer, uint8_t length);

	//***Writing functions***//

	//Writes an 8-bit byte;
	status writeRegister(uint8_t offset, uint8_t dataToWrite);
	//multiWriteRegister takes a uint8 array address as input and performs
	//  a number of consecutive writes
	status multiWriteRegister(uint8_t offset, uint8_t *inputPointer, uint8_t length);

protected:
	uint8_t I2CAddress;
};

#endif /* CCS811CORE_H_ */
