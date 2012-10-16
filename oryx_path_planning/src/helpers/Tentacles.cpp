/**
 * @file Tentacle.cpp
 *
 * @date Oct 11, 2012
 * @author Adam Panzica
 */

//************************************************ INCLUDES ***************************************//
#include "Tentacles.h"

//************************************************ MACROS ***************************************//
#if oryx_path_planner_VERBOSITY
#define PRINTER ROS_INFO
#else
#define PRINTER ROS_DEBUG
#endif

#ifndef MIN_TENTACLE_LENGTH
#define MIN_TENTACLE_LENGTH 8.0
#else
#error MIN_TENTACLE_LENGTH is already defined!
#endif

#ifndef EXP_TENTACLE_LENGTH_BASE
#define EXP_TENTACLE_LENGTH_BASE 33.5
#else
#error EXP_TENTACLE_LENGTH_BASE is already defined!
#endif

#ifndef EXP_TENTACLE_LENGTH_FACTOR
#define EXP_TENTACLE_LENGTH_FACTOR 1.2
#else
#error EXP_TENTACLE_LENGTH_FACTOR is already defined!
#endif

#ifndef TENTACLE_SWEEP_ANGLE
#define TENTACLE_SWEEP_ANGLE 1.2
#else
#error TENTACLE_SWEEP_ANGLE is already defined!
#endif

//************************************************ IMPLEMENTATION ***************************************//
namespace oryx_path_planning{

//***************************** TENTACLE *********************************//
/**
 * This constructor just makes an empty tentacle
 */
Tentacle::Tentacle(){
	this->radius = 0;
	this->velocity = 0;
}

Tentacle::~Tentacle(){};

Tentacle::Tentacle(double expFact, double seedRad, int index, int numTent, double resolution, double xDim, double yDim, double velocity){

	this->velocity= velocity;
	PRINTER("Generating Tentacle %d", index);
	int halfwayIndex	= numTent/2;
	//Calculate tentacle radius. Formula taken from 'von Hundelshausen et al.: Integral Structures for Sensing and Motion'
	if(index<halfwayIndex){
		//Tentacle is to the right of halfway
		this->radius = std::pow(expFact,index)*seedRad;
	}else if(index>halfwayIndex){
		//Tentacle is to the left of halfway
		this->radius = -std::pow(expFact,index-(halfwayIndex+1))*seedRad;
	}else{
		//Tentacle is exactly at halfway
		this->radius = std::numeric_limits<double>::infinity();
	}
	PRINTER("Calculated Tentacle Radius=%f", this->radius);

	//Check for special case of an effectively straight line
	if(this->radius > this->straightThreshold || this->radius < -this->straightThreshold){
		this->radius = std::numeric_limits<double>::infinity();
		for(double i = 0; i<roundToFrac(seedRad,resolution); i+=resolution){
			tf::Point coord;
			coord.setX(i);
			coord.setY(0);
			coord.setZ(0);
			this->points.push_back(coord);
		}
	}
	else{
		//Tracks the last coordinate so that we don't get duplicates
		tf::Point lastCoord;
		lastCoord.setZero();
		//The amount to increment theta by
		double thetaIncrement	= oryx_path_planning::PI/360;
		double sweepAngle		= TENTACLE_SWEEP_ANGLE;
		//Push the first coordinate on
		this->points.push_back(lastCoord);
		//Calculate the X and Y coord along the tentacle
		if(this->radius>0){
			for(double t=sweepAngle; t>0; t-=thetaIncrement){
				tf::Point newCoord;
				Tentacle::calcCoord(this->radius, t, this->radius,resolution, newCoord);
				if(!(newCoord==lastCoord)){
					if(newCoord.getX()<0||newCoord.getY()>yDim) break;
					this->points.push_back(newCoord);
					lastCoord = newCoord;
				}
			}
		}
		else{
			for(double t=0; t<sweepAngle; t+=thetaIncrement){
				tf::Point newCoord;
				Tentacle::calcCoord(-this->radius, t, -this->radius,resolution, newCoord);
				if(!(newCoord==lastCoord)){
					if(newCoord.getX()<0||newCoord.getY()>yDim) break;
					this->points.push_back(newCoord);
					lastCoord = newCoord;
				}
			}
		}
	}
	PRINTER("Calculated a Tentacle with Number of Points=%d",(int)this->points.size());
}

std::vector<tf::Point >& Tentacle::getPoints(){
	return this->points;
}

/**
 * Calculated via the following formula:
 * @f[ y = floor(\frac{radius \times \cosine(theta)}{scale}+rshift) @f]
 * @f[ x = floor(\frac{radius \times \sine(theta)}{scale}) @f]
 */
void Tentacle::calcCoord(double radius, double theta, double rshift, double scale, tf::Point& result){
	result.setY(roundToFrac(radius*std::cos(theta)+rshift, scale));
	result.setX(roundToFrac(radius*std::sin(theta), scale));
	result.setZ(0);
}

double Tentacle::roundToFrac(double raw, double frac){
	return std::floor(raw/frac)*frac;
}


//***************************** SPEED SET *********************************//
/**
 * This constructor just makes an empty set of tentacles
 */
SpeedSet::SpeedSet(){
}


SpeedSet::SpeedSet(double expFact, double seedRad, int numTent, double resolution, double xDim, double yDim, double velocity){
	PRINTER("Generating a Speed Set with the Parameters <SRad=%f, Vel=%f, NumTent=%d, expF=%f>", seedRad, velocity, numTent, expFact);
	for(int t=0; t<numTent; t++){
		this->tentacles.push_back(Tentacle(expFact, seedRad, t, numTent, resolution, xDim, yDim, velocity));
	}
}

SpeedSet::~SpeedSet(){};

unsigned int SpeedSet::getNumTentacle(){
	return this->tentacles.size();
}

Tentacle& SpeedSet::getTentacle(int index){
	return this->tentacles.at(index);
}


//***************************** TENTACLE GENERATOR *********************************//
TentacleGenerator::TentacleGenerator(double minSpeed, double maxSpeed, int numSpeedSet, int numTentacles, double expFact, double resolution, double xDim, double yDim){
	this->expFact 		= expFact;
	this->numTentacles	= numTentacles;
	this->numSpeedSet	= numSpeedSet;
	ROS_INFO("Generating Speed Sets...");

	double q = 0;
	//Generate the SpeedSets
	for(unsigned int v=0; v<numSpeedSet; v++){
		q = calcQ(v);
		this->speedSets.push_back(SpeedSet(expFact, calcSeedRad(v, q), numTentacles, resolution, xDim, yDim, calcSpeedSetVel(minSpeed, maxSpeed, q)));
	}
	ROS_INFO("Speed Sets Complete!");
}

TentacleGenerator::~TentacleGenerator(){};

Tentacle& TentacleGenerator::getTentacle(int speedSet, int index){
	return this->speedSets.at(speedSet).getTentacle(index);
}

SpeedSet& TentacleGenerator::getSpeedSet(int speedSet){
	return this->speedSets.at(speedSet);
}

/**
 * calculate seed radius for the speed set. Formula taken from 'von Hundelshausen et al.: Integral Structures for Sensing and Motion'
 * Equations are as follows:
 * @f[ delta phi = TSA \times \frac{pi}{2} @f]
 * @f[ l = l_min + E_b \times q^(E_f) @f]
 * @f[ R_j = \frac{l}{delta phi \times (1-q^(0.9))} @f]
 *
 * Where @f$ R_j = \text{Seed Radius for Speed Set j} @f$, @f$ delta phi =@f$ the arc swept by the smallest tentacle,
 * @f$ l=@f$ length of the longest tentacle in the set, and @f$ TSA, E_b, E_f@f$ are all tuning constants determined empirically.
 */
double TentacleGenerator::calcSeedRad(int speedSet, double q){
	double dphi = TENTACLE_SWEEP_ANGLE*PI/2.0;
	double l	= MIN_TENTACLE_LENGTH+EXP_TENTACLE_LENGTH_BASE*std::pow(q, EXP_TENTACLE_LENGTH_FACTOR);
	return l/(dphi*(1-std::pow(q,0.9)));
}

/**
 * calculate velocity for the speed set. Formula taken from 'von Hundelshausen et al.: Integral Structures for Sensing and Motion'
 * Equations are as follows:
 * @f[ V_j = V_s+q^(1.2) \times (V_e-V_s) @f]
 *
 * Where @f$ V_j = \text{Velocity for Speed Set j} @f$, @f$ delta phi =@f$ the arc swept by the smallest tentacle,
 * @f$ l=@f$ length of the longest tentacle in the set, and @f$ TSA, E_b, E_f@f$ are all tuning constants determined empirically.
 */
double TentacleGenerator::calcSpeedSetVel(double minSpeed, double maxSpeed, double q){
	return minSpeed+std::pow(q, 1.2)*(maxSpeed-minSpeed);
}

/**
 * Calculates 'magic constant' q. Formula taken from 'von Hundelshausen et al.: Integral Structures for Sensing and Motion'
 */
double TentacleGenerator::calcQ(int speedSet){
	return (double)speedSet/((double)this->numSpeedSet-1);
}

};

//************************************************ UNDEFS ***************************************//
#undef MIN_TENTACLE_LENGTH
#undef EXP_TENTACLE_LENGTH_BASE
#undef EXP_TENTACLE_LENGTH_FACTOR
#undef TENTACLE_SWEEP_ANGLE
