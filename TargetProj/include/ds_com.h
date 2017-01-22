/// *****************************************
/// 
/// \file ds_com.h
/// The structure to be used in communication and calculation
///
/// \author Chen Zhu
/// \date 2016-11-11
/// \version v0.1.0
/// \copyright GPL v3.0+
///
/// ******************************************
#pragma once

namespace robot_control {
	/// The structure of the rebot reflection
	/// Little-endian
	typedef union  {
		unsigned short data;   ///< There are 2 bytes altogether in this data
		struct {
			unsigned char angle2 : 8;
			unsigned char angle1 : 1; ///< angle range from 0-360,
			unsigned char : 6;
			unsigned char direction : 1; ///< 0 for Clockwise , 1 for counter clockwise
			
		} turn;
	} Deflection;
}
