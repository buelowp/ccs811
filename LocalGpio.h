/*
 * LocalGpio.h
 *
 *  Created on: Aug 3, 2017
 *      Author: pete
 */

#ifndef LOCALGPIO_H_
#define LOCALGPIO_H_

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include
class LocalGpio {
public:
	LocalGpio(int);
	virtual ~LocalGpio();

	void low();
	void high();
	void set(int);

private:
	std::ofstream m_gpio;
	std::string m_gpioPath;
	int m_pin;
};

#endif /* LOCALGPIO_H_ */
