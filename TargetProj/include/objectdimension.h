/// *************************************
/// 
///  \file:objectdimension.h
/// 
///  Measure the Dimension of Object from the image
///  Use the moved distance and length of side or  contour area to measure the dimension
///
///  \author: Chen ZHU
///  \date:2016-11-23
///  \version: v0.1.0
///  \copyright: GPL v3
///
/// ****************************************

#ifndef OBJ_DIMENSION_H
#define OBJ_DIMENSION_H

#include "config.h"

namespace image_recognize {
	// Measure the object Dimension from the image
	class ObjectDimension
	{
	public:

		enum AREA_APPROACH {
			AREA_APPROACH_RECT = 1, //! Use a rectangle to approach the area
			AREA_APPROACH_CIRCLE = 2 //! Use Circle to approach the area
		};
		/// Constructor Init the camera scale constant s
		ObjectDimension();
		~ObjectDimension();

		/// Use the 1m ruler to initialize the scale
		/// \param ruler The pixels of the ruler
		/// \param distance The distance between ruler and camera (m)
		/// \param length The length of ruler (m)
		float InitScale(float ruler,float distance,float length = 1);

		/// Get the distance to the object by the line from the image
		/// Require the movement of the robot, the distance is calculated by formular below
		/// \f[ \frac{{l_2}\cdot d}{{l_1}+{l_2}} ]
		/// \param len the length of line from first image
		/// \param len2 the length of line from the second image
		/// \param move the distance of moving (m)
		float DistanceByLine(float len, float len2, float move);

		/// Calculate the distance by contour area
		/// Require the movement of robot
		/// \param area the area of the contour in first image
		/// \param area2 the area of the second image
		float DistanceByArea(float area, float area2, float move, int approach = AREA_APPROACH_RECT);

		/// Calculate the target dimension by moving
		/// The dimension is calculated by
		/// \f[ \frac{{l_1} \cdot {l_2}\cdot d}{a \cdot ({l_1}+{l_2})} ]
		/// \param len the length of line from first image
		/// \param len2 the length of line from the second image
		/// \param move the distance of moving (m)
		float DimensionByLine(float len, float len2, float move);
        
        /// Calcuate the distance by reference image
        /// The distance is calculated by
        /// \f[ \frac{h\cdot d_{ref}}{h_{ref}}]
        /// \param height the height of target object
        float DistanceByRef(float height,float href);

        /// Calculate the dimension by reference image
        /// \param len the pixels from image
        /// \param distance the distance of from image
        float DimensionByRef(float len,float distance);

		void LoadScale();
		void SaveScale();

	private:
		/// The pixels of per word uint (m)
		float scale = 0;
		float dref=1;
		char calibfile[256];
	};
}
#endif

