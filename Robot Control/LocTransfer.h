/// *********************************************
///
/// \file LocTransfer.h
/// Transfer the object detected in the image information into move parameter
///
/// \author     Chen Zhu
/// \version    v0.1.0
/// \date       2016-11-11
/// \copyright  GPL v3.0+
///
/// **************************************************
#pragma once
#include <opencv2/core.hpp>
#include "ds_com.h"

using namespace cv;
namespace robot_control {
	/// Get the location from point info
	/// This class DONOT calibrate Camera
	class LocTransfer
	{
	public:
		/// Constructor
		LocTransfer();

		/// Deconstructor
		~LocTransfer();

		/// Get the object direction from the image;
		/// \param x the center point of the object
		/// \param sz the size of the image
		/// \param distance the distance of object from distance function
		Deflection direction(Point x, Size sz, double distance);


		/// Calculate the distance of Object
		/// \param length the length in the image
		double distance(int length);


	private:
		int dist_unit_length =0;


	};

}