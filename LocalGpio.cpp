/*
 * LocalGpio.cpp
 *
 *  Created on: Aug 3, 2017
 *      Author: pete
 */

#include "LocalGpio.h"

LocalGpio::LocalGpio(int pin)
{
	m_pin = pin;
	std::string ex_str = "/sys/class/gpio/export";
	std::ofstream ex_file(ex_str);
	if (ex_file) {
		ex_file << pin;
	}

	std::string gpio_str = "/sys/class/gpio/gpio" + m_pin + "/direction";
	std::ofstream gpio_file(gpio_str);
	if (gpio_file) {
		gpio_file << "out";
	}

	std::string value_str = "/sys/class/gpio/gpio" + m_pin + "/value";
	m_gpio.open(value_str.c_str());
}

LocalGpio::~LocalGpio()
{
	m_gpio.close();
}

void LocalGpio::high()
{
	if (m_gpio.is_open())
		m_gpio << "1";
}

void LocalGpio::low()
{
	if (m_gpio.is_open())
		m_gpio << "0";
}

void LocalGpio::set(int val)
{
	if (m_gpio.is_open())
		m_gpio << val;
}
