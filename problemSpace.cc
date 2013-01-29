//*********************************************************************
// Must be threaded, with upgradable mutex around active payload.
// All attributes MUST be of non-pointer types.
//*********************************************************************


#include "problemSpace.h"

#include <iostream> 
using namespace std;

ProblemSpace::ProblemSpace(int tPerItem, int iPerTrue){
	rwControl = new boost::interprocess::named_upgradable_mutex(boost::interprocess::open_or_create, "rwControl");
	this->tPerItem = tPerItem;
	this->iPerTrue = iPerTrue;
	problemID = 0;
	srand(time(0));
}

void ProblemSpace::operator()() {
	int attributes[16];	//populated by random funcs, then copied to real attributes while locked. Reduces lock time. 
	for (int i = 0;;i++){
		if (i % iPerTrue == 0){
			//Construct real bits
			deVolumize(randInRange(4,20), attributes);
			attributes[3] = randInRange(5,50);
			attributes[4] = randInRange(0,0xFFFF);
			attributes[4] &= ~0x4;	//no glue.
			attributes[4] |= 0x11;	//has metal and silicon
			attributes[15] = 1;
		}
		else{
			//rand everything. force 1 "wrong" category.
			attributes[0] = randInRange(1,20);
			attributes[1] = randInRange(1,20);
			attributes[2] = randInRange(1,20);
			if (attributes[0] < attributes[1]){//ensure 0 <= 1
				attributes[0] ^= attributes[1];
				attributes[1] ^= attributes[0];
				attributes[0] ^= attributes[1];
			}
			if (attributes[0] < attributes[2]){//ensure 0 <= 2
				attributes[0] ^= attributes[2];
				attributes[2] ^= attributes[0];
				attributes[0] ^= attributes[2];
			}
			if (attributes[1] < attributes[2]){//ensure 1 <= 2
				attributes[1] ^= attributes[2];
				attributes[2] ^= attributes[1];
				attributes[1] ^= attributes[2];
			}
			attributes[3] = randInRange(0,50);
			attributes[4] = randInRange(0,0xFFFF);
			switch (randInRange(0,4)){
			case 0:
				if (attributes[0] * attributes[1] * attributes[2] < 20){
					if (attributes[0] < 3){
						attributes[0] = 3;
						attributes[1] = 3;
						attributes[2] = 3;
					}
					else if (attributes[1] < 3){
						attributes[1] = 3;
						attributes[2] = 3;
					}
					else if (attributes[2] < 3){
						attributes[2] = 3;
					}
				}
			break;	//forces up each to at least 3... 3^3 = 27.
			case 1:
				if (attributes[3] >= 5) attributes[3] %= 5;	//too few parts
			break;
			case 2:
				attributes[4] &= ~0x1;	//missing metal
			break;
			case 3:
				attributes[4] &= ~0x10;	//missing silicon
			break;
			case 4:
				attributes[4] |= 0x4;	//has glue
			break;
			}
			attributes[15] = 0;
		}
		
		//always random categories.
		attributes[5] = randInRange(0,50);
		attributes[6] = randInRange(0,0xFFFFFF);
		attributes[7] = randInRange(5,50);
		attributes[8] = randInRange(0,6);
		attributes[9] = randInRange(0,0xFFFF);
		attributes[10] = randInRange(0,100);
		attributes[11] = randInRange(0,100);
		attributes[12] = randInRange(0,100);
		attributes[13] = randInRange(0,100);
		attributes[14] = randInRange(0,100);		
		
		//lock the mutex!
		//wait here, non-blocking until ready. 
		while (!rwControl->try_lock()) boost::this_thread::sleep(boost::posix_time::milliseconds(15));
		for (int i = 0; i <= 14; i++){
			this->attributes[i] = attributes[i];
		}
		
		problemID++;	//used to tell that it's a new problem.
		isThingamajig 		= attributes[15] != 0;	//LOL wonky typecasting
		//Don't forget to unlock.
		rwControl->unlock();
		//cout << longEdge << ", " << midEdge << ", " << shortEdge << ", " << movingPartCount << ", " << composition << ", " << nonMovingPartCount << ", " << color << ", " << weight << ", " << orientation << ", " << operationalVelocity << ", " << roundness << ", " << transparency << ", " << temperature << ", " << resistance << ", " << elasticity << ": " << isThingamajig << '\n';
		boost::this_thread::sleep(boost::posix_time::milliseconds(tPerItem));
	}
}

void ProblemSpace::deVolumize(int volume, int* attributes){
	//factor volume... Works for all but 16, which becomes 8. This is acceptable.
	int i, j = 1, k = 1;
	for (i = 2; i < volume; i++){
		while (volume % i == 0 && volume > 1)
		{
			k = j;
			j = i;
			volume /= i;
		}
	}
	i = volume;	//so i, j, k contain factors.
	if (i >= k){
		attributes[0] = i;
		attributes[1] = j;
		attributes[2] = k;
	}
	else{
		attributes[2] = i;
		attributes[1] = j;
		attributes[0] = k;
	}
}

int ProblemSpace::randInRange(int min, int max){
	int random = rand();
	random /= (RAND_MAX / (max - min));
	return random + min;
}

int main(void){
	ProblemSpace PS(500,2);
	new boost::thread(PS);
	char lies[30];
	cin >> lies;

}