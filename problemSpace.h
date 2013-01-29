#ifndef GEN_PROBLEMSPACE_H
#define GEN_PROBLEMSPACE_H

//For threading
#include <boost/interprocess/sync/named_upgradable_mutex.hpp>
#include <boost/thread.hpp>
//For Random
#include <ctime>

//*********************************************************************
// Must be threaded, with upgradable mutex around active payload.
// All attributes MUST be of non-pointer types.
//*********************************************************************

class ProblemSpace{
public://constructors
ProblemSpace(int tPerItem, int iPerTrue);

public:
int attributes[15]; 
  /*longEdge,
	midEdge,
	shortEdge,
	movingPartCount,
	composition,	//Set of flags, 32-bit.
	nonMovingPartCount,
	color,
	weight,
	orientation,	//set of flags, 6bit.
	operationalVelocity,
	roundness,
	transparency,
	temperature,
	resistance,
	elasticity;*/
	
bool isThingamajig;
unsigned short problemID;	//it's okay to cycle quickly. We just need to be sure that everything happened. 

boost::interprocess::named_upgradable_mutex* rwControl;	//lock() for write. All others lock_sharable() for read. Must be constructed in this constructor.

private://attributes
int tPerItem;	//Time per item. In milliseconds.
int iPerTrue;	//Items per correct.

public://functions
void operator()();	//Drives updating attributes. Must lock() mutex.
private:
void deVolumize(int volume, int* attributes);	//Used for factoring volumes. Converts 16 to 8 and always uses the most factored form, but that's okay.
int randInRange(int min, int max);	//gets mostly even distribution in range.
};

#endif