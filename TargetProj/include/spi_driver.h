/// ****************************************
/// \file spi_driver.h
/// Realize the communication protocol
///
/// \author		Chen Zhu
/// \date		2016-11-12
/// \version	v0.1.0a
/// \copyright 	GPL v3.0+
///
/// *****************************************

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#define MOVE_FORWARD 1
#define MOVE_BACK 2

#include "ds_com.h"


namespace robot_control{

	/// Send Command to the Arduino Controller
	class ControllerPort{
		public:
			/// Constructor, Initialize the SPI interface
			ControllerPort();
			
			/// Deconstructor
			~ControllerPort();
			
			/// Move the robot forward
			/// \param speed The moving speed
			/// \param distance The moving distance
			int MoveForward(unsigned char speed,unsigned char distance = 0);
			
			/// Move the robot forward
			/// \param speed The moving speed
			/// \param distance The moving distance
			int MoveBackward(unsigned char speed,unsigned char distance = 0);
			
			/// Movement Command
			/// \param direction MOVE_FORWARD or MOVE_BACKWARD
			/// \param speed The moving speed
			/// \param distance The moving distance
			int Movement(unsigned char direction, unsigned char speed, unsigned char distance);
			
			/// Adjust the azimuth of the robot
			/// \param azimuth the deflection information
			int Heading(Deflection azimuth);
			
			/// Using coeff as acceration to adjust speed
			/// \param coeff The acceration
			int SpeedSlope(char coeff);
			
			/// Stop Moving the robot
			int Stop();
			
			/// Set the left motor speed 
			/// \param speed The speed of motor
			int SetLMotorSpeed(short speed);
			
			/// Set the right motor speed 
			/// \param speed The speed of motor
			int SetRMotorSpeed(short speed);
			
			/// Query the value of sensor
			/// \param id the id of sensor. See the sensor table.
			/// \return the value of sensor
			int QuerySensor(unsigned short id);
			
			/// Turn of all the light
			int LightOff();
			
			/// Set the brightness of IR light using DC
			/// \param dc the duty cycle of the pwm
			int SetIRLight(unsigned char dc);
			
			/// Set the brightness of White light using DC
			/// \param dc the duty cycle of the pwm
			int SetWLight(unsigned char dc);
			
			/// Keep the camera stable
			int CamStable();
			
			/// Set whether to pause the movement
			/// \param value 1: pause the movement, 0: continue movement
			int CamPause(unsigned char value);
			
			/// Set the Height of the camera
			/// \param height The height of the camera
			int CamHeight(unsigned short height);
			
			/// Set the angle of the camera
			/// \param angle the azimuth of the camera
			int CamAngle(unsigned short angle);
			
			/// Set the IR filter enable
			/// \param enable 1:the filter is enable, 0:the filter is disabled
			int CamFilter(unsigned char enable);

			/// Set the state of target tools
			/// \param state The 
			int TargetTool(unsigned char id, unsigned short status);

		
		private:
			/// The spi driver describer
			int spi_handle;
			
			/// Send Message through SPI
			/// \param message The pointer to message
			/// \param len the length of message
			int _spi_send_message(unsigned char* message, int len);
		
	};
}


#endif
