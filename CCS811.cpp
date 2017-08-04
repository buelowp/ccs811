#include "CCS811.h"

CCS811::CCS811(int pin, uint8_t addr)
{
	m_gpio = new LocalGpio(pin);
	m_addr = addr;
}


bool CCS811::begin()
{
	uint8_t hwid = 0;
	uint8_t status = 0;

	// from datasheet - up to 70ms on the first Reset after new application download; up to 20ms delay after power on
	// For Linux, this isn't strictly necessary and may be removed in the future
	localSleep(0, 70000);

	if ((hwid = readHW_ID() != 0x81) {
		fprintf(stderr, "%s:%d: Error starting CCS811, wrong HW ID (%x) returned\n", __PRETTY_FUNCTION__, __LINE__, hwid);
		return false;
	}

	status = readStatus();
	uint8_t bit = (status & (1 << 5-1)) != 0; // black magic to read APP_VALID bit from STATUS register
	if(bit != 1) {}
    	fprintf(stderr, "%s:%d: Error: No application firmware loaded\n", __PRETTY_FUNCTION__, __LINE__);
    	readErrorID(status);
    	return false;
	}

	m_gpio->low();
	localSleep(0, 50000);

	i2c_writeBytes(0, m_addr, APP_START, 1, 1);
	m_gpio->high();

	status = readStatus();
	bit = (status & (1<<8-1)) !=0; // black magic to read FW_MODE bit from STATUS register
	if (bit != 1) {
    	fprintf(stderr, "%s:%d: Error: Firmware still in boot mode\n", __PRETTY_FUNCTION__, __LINE__);
		readErrorID(status);
		return false;
	}

	m_gpio->low();
	localSleep(0, 50000);
	i2c_writeBytes(0, m_addr, MEAS_MODE, 1, 1);

	m_gpio->high();

	return true;
}


uint8_t CCS811::readStatus(void)
{
	uint8_t status;

	m_gpio->low();
	localSleep(0, 50000);
	i2c_writeBytes(0, m_addr, STATUS, 1, 1);

	i2c_readBytes(0, m_addr, STATUS, &status);
	m_gpio->high();
	return status;
}


uint8_t CCS811::readHW_ID(void)
{
	uint8_t hwid = 0;

	// Need to wake it up
	m_gpio->low();
	localSleep(0, 50000);	// recommended 50us delay after asserting WAKE pin
	i2c_writeBytes(0, m_addr, HW_ID, 1, 1);
	i2c_readBytes(0, m_addr, HW_ID, &hwid);
	m_gpio->high();

	return hwid;
}
/*
	// Put it back to sleep
  Wire.beginTransmission(m_addr);
  Wire.write(HW_ID);
  Wire.endTransmission();

  Wire.requestFrom(m_addr, (uint8_t)1);
  byte hw_id = Wire.read();

  digitalWrite(_WAKE_PIN, HIGH);
  return hw_id;
}
*/

uint8_t CCS811::readErrorID(uint8_t _status)
{
	uint8_t error_id = 0;

	m_gpio->low();
	localSleep(0, 50000);
	i2c_writeBytes(0, m_addr, ERROR_ID, 1, 1);
	i2c_readBytes(0, m_addr, ERROR_ID, &error_id);
	uint8_t bit = (_status & (1 << 1 - 1)) != 0;
	if (bit == 1) {
		std::cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": Error ID: " << error_id;
	}
	return error_id;

	return error_id;
}


void CCS811::sleep()
{
	m_gpio->low();
	localSleep(0, 50000);
	i2c_writeBytes(0, m_addr, MEAS_MODE, 0, 2);
	m_gpio->high();
}


void CCS811::getData(void)
{
	uint8_t buffer[8];

	//CCS811::compensate(t, rh);
	m_gpio->low();
	localSleep(0, 50000);
	i2c_writeBytes(0, m_addr, ALG_RESULT_DATA, 0, 0);
	i2c_readBytes(0, m_addr, ALG_RESULT_DATA, &buffer, 8);
	CO2 = ((uint16_t)buffer[0] << 8) | buffer[1];
	TVOC = ((uint16_t)buffer[2] << 8) | buffer[3];
	m_gpio->high();
}


int CCS811::readTVOC(void)
{
  return TVOC;
}


int CCS811::readCO2(void)
{
  return CO2;
}


void CCS811::compensate(float t, float r)    // compensate for temperature and relative humidity
{
	int temp, rh;
	uint8_t envdata[4];

	m_gpio->low();
	localSleep(0, 50000);

	if (t > 0) {
		temp = (int)t + 0.5;
	}
	else if (t < 0) {
		temp = (int)t - 0.5;
	}
	temp = temp + 25;
	rh = (int)r + 0.5;

	envdata[0] = rh << 1;
	envdata[1] = 0;
	envdata[2] = temp << 1;
	envdata[3] = 0;

	i2c_writeBuffer(0, m_addr, ENV_DATA, envdata, 4);
	m_gpio->high();
}

void CCS811::_digitalWrite(int val)    // asserts WAKE pin with a small delay to ensure reliable communication - thanks to djdehaan for this fix
{
	m_gpio->set(val);
	localSleep(0, 60000);
}

void CCS811::localSleep(int secs, int msecs)
{
	struct timespec ts;

	ts.tv_sec = secs;
	ts.tv_nsec = msecs;
	nanosleep(&ts, NULL);
}
