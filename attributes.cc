#include <cstdlib>

#include "attributes.h"
//Brief descriptions in header.
void AgentAttributes::init(const AgentAttributes& parentA, const AgentAttributes& parentB, int mutRate, int mutSev){
	for (int i = 0; i < 13; i++){
		if (rand() < RAND_MAX / 2){
			attributes[i] = parentA.attributes[i];
		}
		else{
			attributes[i] = parentB.attributes[i];
		}
		
		if (rand() < RAND_MAX / mutRate){
			attributes[i] += (rand() / (RAND_MAX / (mutSev * 2))) - mutSev;	//rand from +-mutSev
		}
	}
	if (!attributes[moveBidFactor]) attributes[moveBidFactor]++;	//prevent divide by zero
	if (!attributes[mateBidFactor]) attributes[mateBidFactor]++;
	if (!attributes[cloneBidFactor]) attributes[cloneBidFactor]++;
}

void AgentAttributes::init(int minThresh, int maxThresh){
	for (int i = 0; i < 13; i++){
		attributes[i] = rand() / (RAND_MAX / (maxThresh - minThresh + 1)) + minThresh;
	}
}

int AgentAttributes::evaluateMoveBid(int energy, int income, int duration){
	if (duration < attributes[wanderlust] || income > attributes[greed]) return 0;
	if (energy / attributes[moveBidFactor] < attributes[moveMinBid]) {
		if (energy > attributes[moveMinBid] + attributes[moveBidFactor]) return attributes[moveMinBid];
		return 0;	//moving would kill me. Hope for the best here.
	}
	if (energy / attributes[moveBidFactor] > attributes[moveMaxBid]) return attributes[moveMaxBid];
	return energy / attributes[moveBidFactor];
}

int AgentAttributes::evaluateCloneBid(int energy, int income, int duration){
	if (duration < attributes[wanderlust] || income < attributes[greed]) return 0;
	if (energy / attributes[cloneBidFactor] < attributes[cloneMinBid]) return 0;
	if (energy / attributes[cloneBidFactor] > attributes[cloneMaxBid]) return attributes[cloneMaxBid];
	return energy / attributes[cloneBidFactor];
}

int AgentAttributes::evaluateMateBid(int energy){
	if (energy / attributes[mateBidFactor] < attributes[mateMinBid]) return 0;
	if (energy / attributes[mateBidFactor] > attributes[mateMaxBid]) return attributes[mateMaxBid];
	return energy / attributes[mateBidFactor];
}

//****************************************************************
//*Note: In the event of a draw, it presently goes to the "first."
//*	Eventually this may benefit from randomization.
//****************************************************************
MateTarget* AgentAttributes::evaluateMateTarget(std::vector<MateTarget*> mates, Agent* self){
	MateTarget* mostAttractive = NULL;
	long attractive = 0;
	for (std::vector<MateTarget*>::iterator i=mates.begin(); i < mates.end(); i++){
		MateTarget* iBase = *i;
		if (iBase->agent != self){	//we don't want to support mating with yourself.
			if (((long)*(iBase->energy) * (long)attributes[energyBias] + (long)*(iBase->age) * (long)attributes[ageBias]) > attractive){
				attractive = (long)*(iBase->energy) * (long)attributes[energyBias] + (long)*(iBase->age) * (long)attributes[ageBias];
				mostAttractive = iBase;
			}
		}
	}
	return mostAttractive;
}
