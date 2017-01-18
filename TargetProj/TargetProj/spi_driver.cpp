/// *************************************************
///
/// \file LocTransfer.cpp
/// Transfer the object detected in the image information into move parameter
///
/// \author     Chen Zhu
/// \version    v0.1.0a
/// \date       2016-11-12
/// \copyright  GPL v3.0+
///
/// **************************************************

#include "spi_driver.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

namespace robot_control{
	/// Constructor
	ControllerPort::ControllerPort()
	{
		/// Initialize the Logger
		openlog("RobotCom",LOG_PID|LOG_CONS,LOG_USER);
		LOG_PRINT(LOG_INFO, "Initializing SPI interface\n");
		/// Initialize the SPI interface
		spi_handle = wiringPiSPISetup(SPI_CHANNEL,SPI_CLOCK);
		/// The SPI cannot open
		if(spi_handle == -1)
		{
			
			LOG_PRINT_ERR(LOG_ERR,"Error in opening SPI\n");
			abort();
		}
		
		
	}
	
	/// Deconstructor
	ControllerPort::~ControllerPort()
	{
		
	}
	
	/// Move the robot forward
	/// \param speed The moving speed
	/// \param distance The moving distance
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::MoveForward(unsigned char speed,unsigned char distance)
	{
		LOG_PRINT(LOG_INFO,"Moving Forward using speed %u with distance %u\n",speed,distance);
		return Movement(MOVE_FORWARD,speed,distance);
	}
	
	/// Move the robot backward
	/// \param speed The moving speed
	/// \param distance The moving distance
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::MoveBackward(unsigned char speed,unsigned char distance)
	{
		LOG_PRINT(LOG_INFO,"Moving Back using speed %u with distance %u\n",speed,distance);
		return Movement(MOVE_BACK,speed,distance);
	}
	
	/// Move the robot 
	/// \param direction MOVE_FORWARD or MOVE_BACKWARD
	/// \param speed The moving speed
	/// \param distance The moving distance
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::Movement(unsigned char direction, unsigned char speed, unsigned char distance)
	{
		unsigned char btsMsg[7] = {0x01,0x01,0x00,0x00,0xff,0xcc,0x00};
		btsMsg[1] = direction;
		btsMsg[2] = speed;
		btsMsg[3] = distance;
		return _spi_send_message(btsMsg,7);
	}
	
	/// Adjust the azimuth of the robot
	/// \param azimuth the deflection information
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::Heading(Deflection azimuth)
	{
		unsigned char btsMsg[7] = {0x01,0x03,0x00,0x00,0xff,0xcc,0x00};
		/// Btyes are stored in little-endian on Linux
		short data = htole16(azimuth.data);
		btsMsg[2] = (unsigned char)((data>>8)&0xff);
		btsMsg[3] = (unsigned char)(data&0xff);
		LOG_PRINT(LOG_INFO,"Turning angle %x\n",azimuth.data);
		return _spi_send_message(btsMsg,7);
	}
	
	
	/// Using coeff as acceration to adjust speed
	/// \param coeff The acceration
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::SpeedSlope(char coeff)
	{
		unsigned char btsMsg[6] = {0x01,0x04,0x00,0xff,0xcc,0x00};
		btsMsg[2] = (unsigned char)(coeff&0xff);
		LOG_PRINT(LOG_INFO,"Speed Slope %d\n",coeff);
		return _spi_send_message(btsMsg,6);
	}
	
	/// Stop Moving the robot
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::Stop()
	{
		unsigned char btsMsg[5] = {0x01,0x80,0xff,0xcc,0x00};
		LOG_PRINT(LOG_INFO,"Emergency Stop\n");
		return _spi_send_message(btsMsg,5);
	}
	
	/// Set the left motor speed 
	/// \param speed The speed of motor
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::SetLMotorSpeed(short speed)
	{
		unsigned char btsMsg[7] = {0x01,0x10,0x00,0x00,0xff,0xcc,0x00};
		/// Btyes should be change into little-endian mode
		short sp = htole16(speed);
		btsMsg[2] = (unsigned char)((sp>>8)&0xff);
		btsMsg[3] = (unsigned char)(sp&0xff);
		LOG_PRINT(LOG_INFO,"Setting Left Motor Speed to %d\n",speed);
		return _spi_send_message(btsMsg,7);
	}
			
	/// Set the right motor speed 
	/// \param speed The speed of motor
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::SetRMotorSpeed(short speed)
	{
		unsigned char btsMsg[7] = {0x01,0x11,0x00,0x00,0xff,0xcc,0x00};
		/// Btyes should be change into little-endian mode
		short sp = htole16(speed);
		btsMsg[2] = (unsigned char)((sp>>8)&0xff);
		btsMsg[3] = (unsigned char)(sp&0xff);
		LOG_PRINT(LOG_INFO,"Setting Right Motor Speed to %d\n",speed);
		return _spi_send_message(btsMsg,7);
	}
	
	/// Query the value of sensor
	/// \param id the id of sensor. See the sensor table.
	/// \return the value of sensor.
	int ControllerPort::QuerySensor(unsigned short id)
	{
		unsigned char btsMsg[7] = {0x01,0x20,0x00,0x00,0xff,0xcc,0x00};
		unsigned char btsRecv[12];
		union {
			unsigned int u32;
			unsigned char arr[4];
		}value;
		unsigned int len;
		
		
		memset(btsRecv,0,9);
		/// Btyes should be change into little-endian mode
		short id2 = htole16(id);
		btsMsg[2] = (unsigned char)((id2>>8)&0xff);
		btsMsg[3] = (unsigned char)(id2&0xff);
		_spi_send_message(btsMsg,7);
		
		/// Give 10ms for Querying
		usleep(10000);
		
		/// Get the Value from response
		_spi_send_message(btsRecv,11);
		
		len = btsRecv[2];
		printf("Received length: %d",len);
		/// the length cannot over 4
		if(len>6)
		{
			#ifdef _DEBUG
			printf("Length of Reply overflow! The length of message received is %d!\n",len);
			#endif
			LOG_PRINT(LOG_WARNING,"Length of Reply overflow! The length of message received is %d!\n",len);
			return 0;
		}
		
		memcpy(value.arr,btsRecv+3,len);
		
		LOG_PRINT(LOG_INFO,"Query sensor id %d, value %d\n",id,value);
		return be32toh(value.u32);
	}
	
	/// Turn of all the light
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::LightOff()
	{
		unsigned char btsMsg[5] = {0x02,0x00,0xff,0xcc,0x00};
		LOG_PRINT(LOG_INFO,"Light Off\n");
		return _spi_send_message(btsMsg,5);
	}
	
	/// Set the brightness of IR light using DC
	/// \param dc the duty cycle of the pwm
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::SetIRLight(unsigned char dc)
	{
		unsigned char btsMsg[6] = {0x02,0x01,0x00,0xff,0xcc,0x00};
		
		btsMsg[2] = dc;
		
		LOG_PRINT(LOG_INFO,"Setting IR light to dc %d\n",dc);
		return _spi_send_message(btsMsg,6);
	}
			
	/// Set the brightness of White light using DC
	/// \param dc the duty cycle of the pwm
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::SetWLight(unsigned char dc)
	{
		unsigned char btsMsg[6] = {0x02,0x01,0x00,0xff,0xcc,0x00};
		
		btsMsg[2] = dc;
		
		LOG_PRINT(LOG_INFO,"Setting White light to dc %d\n",dc);
		return _spi_send_message(btsMsg,6);
	}
	/// Keep the camera stable
	int ControllerPort::CamStable()
	{
		unsigned char btsMsg[5] = {0x03,0x00,0xff,0xcc,0x00};
		LOG_PRINT(LOG_INFO,"Camera Stable\n");
		return _spi_send_message(btsMsg,5);
	}
	
	/// Set whether to pause the movement
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::CamPause(unsigned char value)
	{
		unsigned char btsMsg[6] = {0x03,0x10,0x00,0xff,0xcc,0x00};
		
		btsMsg[2] = (value>0) ? 1: 0;
		
		LOG_PRINT(LOG_INFO,"Cam pause was set to  %d\n",btsMsg[2]);
		return _spi_send_message(btsMsg,6);
	}
			
	/// Set the Height of the camera
	/// \param height The height of the camera
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::CamHeight(unsigned short height)
	{
		unsigned char btsMsg[7] = {0x03,0x01,0x00,0x00,0xff,0xcc,0x00};
		/// Btyes are stored in little-endian on Linux
		unsigned short h = htole16(height);
		btsMsg[2] = (unsigned char)((h>>8)&0xff);
		btsMsg[3] = (unsigned char)(h&0xff);
		LOG_PRINT(LOG_INFO,"Setting Cam Height to %d\n",height);
		return _spi_send_message(btsMsg,7);
	}
			
	/// Set the angle of the camera
	/// \param angle the azimuth of the camera
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::CamAngle(unsigned short angle)
	{
		unsigned char btsMsg[7] = {0x03,0x02,0x00,0x00,0xff,0xcc,0x00};
		/// Btyes are stored in little-endian on Linux
		unsigned short agl = htole16(angle);
		btsMsg[2] = (unsigned char)((agl>>8)&0xff);
		btsMsg[3] = (unsigned char)(agl&0xff);
		LOG_PRINT(LOG_INFO,"Setting Cam Angle to %d\n",angle);
		return _spi_send_message(btsMsg,7);
	}
			
	/// Set the IR filter enable
	/// \param enable 1:the filter is enable, 0:the filter is disabled
	/// \return if ack enabled, 0 for no response; or the value of SPI
	int ControllerPort::CamFilter(unsigned char enable)
	{
		unsigned char btsMsg[6] = {0x03,0x03,0x00,0xff,0xcc,0x00};
		
		btsMsg[2] = (enable>0) ? 1: 0;
		
		LOG_PRINT(LOG_INFO,"Camera IR filter is set to %d\n",btsMsg[2]);
		return _spi_send_message(btsMsg,6);
	}

	int ControllerPort::TargetTool(unsigned char id,unsigned short status)
	{
		unsigned char btsMsg[7] = { 0x04,0x01,0x00,0x00,0xff,0xcc,0x00 };
		/// Btyes are stored in little-endian on Linux
		unsigned short agl = htole16(status);
		btsMsg[1] = id;
		btsMsg[2] = (unsigned char)((agl >> 8) & 0xff);
		btsMsg[3] = (unsigned char)(agl & 0xff);
		LOG_PRINT(LOG_INFO, "Setting Robot Tool No.%hh to %d\n",id, status);
		return _spi_send_message(btsMsg, 7);
	}
	
	/// Send Message through SPI
	/// \param message The pointer to message
	/// \param len the length of message
	int ControllerPort::_spi_send_message(unsigned char* message, int len)
	{
		if(NEED_ACK){
			message[len-1] = 0x80;
			wiringPiSPIDataRW(SPI_CHANNEL,message,len);
			if(message[len-1] != 0xff)
				return 0;
			else
				return 1;
		}else{
			return wiringPiSPIDataRW(SPI_CHANNEL,message,len);
		}
		
	}
}
