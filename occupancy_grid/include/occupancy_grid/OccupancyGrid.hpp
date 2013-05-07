/**
 * @file   OccupancyGrid.hpp
 *
 * @date   May 7, 2013
 * @author Adam Panzica
 * @brief  Class definitions for OccupancyGrid
 */

/*
 * Copyright (c) 2013, RIVeR Lab
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef OCCUPANCYGRID_HPP_
#define OCCUPANCYGRID_HPP_

//*********** SYSTEM DEPENDANCIES ****************//
#include <iostream>
#include <nav_msgs/OccupancyGrid.h>
//************ LOCAL DEPENDANCIES ****************//

//***********    NAMESPACES     ****************//

namespace ogu = occupancy_grid::utilities;
namespace nm  = nav_msgs;
namespace occupancy_grid
{

/**
 * Utilities for working with occupancy grids
 */
namespace utilities
{
/**
 * @author Adam Panzica
 * @brief enum representing the various types the occupancy grid cells can contain
 */
enum PointTrait
{
	UNKOWN = -1,        //!< UNKOWN Cell's value is unkown
	FREE_LOW_COST = 10, //!< FREE_LOW_COST Cell contains open terrain
	FREE_HIGH_COST= 20, //!< FREE_HIGH_COST Cell contains difficult terrain
	OBSTACLE      = 30, //!< OBSTACLE Cell contains an obstacle
	TRAVERSED     = 40, //!< TRAVERSED Cell contains previously traversed terrain
	GOAL          = 100,//!< GOAL Cell contains a goal point
};

/**
 * @author Adam Panzica
 * @brief Inserts a PointTrait into an ostream in string form
 * @param [in] out the input ostream
 * @param [in] in  the PointTrait to convert
 * @return The resultant ostream
 */
inline friend std::ostream& operator<<(std::ostream& out, const PointTrait& in)
{
	switch(in)
	{
	case UNKOWN:
		out<<"UNKOWN";
		break;
	case FREE_LOW_COST:
		out<<"FREE_LOW_COST";
		break;
	case FREE_HIGH_COST:
		out<<"FREE_HIGH_COST";
		break;
	case OBSTACLE:
		out<<"OBSTACLE";
		break;
	case TRAVERSED:
		out<<"TRAVERSED";
		break;
	case GOAL:
		out<<"GOAL";
		break;
	default:
		out<<"INVALID_"<<in;
		break;
	}
	return out;
}

/**
 * @author Adam Panzica
 * @brief  Override of the = operator to allow assignment via an int with defined behavior for what happens if the int isn't in the enum
 * @param [out] lhs The PointTrait to assign to
 * @param [in[  rhs The int to assign it with
 * @return Reference to lhs
 *
 * Note that if rhs is not contained in the enum, it will assign UNKOWN as the PointTrait
 */
PointTrait& operator=(PointTrait& lhs, const int& rhs)
{
	switch(rhs)
	{
	case UNKOWN:
		lhs = UNKOWN;
		break;
	case FREE_LOW_COST:
		lhs = FREE_LOW_COST;
		break;
	case FREE_HIGH_COST:
		lhs = FREE_LOW_COST;
		break;
	case OBSTACLE:
		lhs = OBSTACLE;
		break;
	case TRAVERSED:
		lhs = TRAVERSED;
		break;
	case GOAL:
		lhs = GOAL;
		break;
	default:
		lhs = UNKOWN;
		break;
	}
	return lhs;
}

/**
 * @author Adam Panzica
 * @brief  Calculates the index into a row-major linear array representing a grid for a given x,y pair
 * @param [in] x The x position in the grid (width)
 * @param [in] y The y position in the grid (height)
 * @param [in] width The width of the grid
 * @return Index into row-major linear array for the given x,y pair
 */
inline int calcIndexRowMajor2D(int x, int y, int width)
{
	return y*width+x;
}

/**
 * @author Adam Panzica
 * @brief normalizes a set of confidence values that range from 0-100
 * @param [in]  values An array of values to normalize
 * @param [in]  size   The size of the input and output arrays
 * @param [out] result An array to write out the normalized values. May safely be the same array as input, though will be destructive in that case
 */
inline void normalizeConfidance(const uint8_t values[], int size, uint8_t result[])
{
	int sum = 0;
	for(int i=0; i<size; i++)
	{
		sum+=values[i];
	}
	for(int i=0; i<size; i++)
	{
		result[i] = values[i]/sum;
	}
}


}; /* END UTILITIES */

class MultiDimentionalOccupancyGrid
{
private:
	std::vector<nm::OccupancyGrid> grid_;		///The backing map data
	nm::MapMetaData                map_data_;

};

}; /* END OCCUPANCY_GRID */


#endif /* OCCUPANCYGRID_HPP_ */
