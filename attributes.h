#ifndef GEN_AGENTATTRIBUTES_H_
#define GEN_AGENTATTRIBUTES_H_

#include <vector>

#include "mateTarget.h"

class AgentAttributes{
public:

protected:
enum {
	mateBidFactor,
	mateMinBid,
	mateMaxBid,
	ageBias,
	energyBias,
	wanderlust,
	greed,
	moveBidFactor,
	moveMinBid,
	moveMaxBid,
	cloneBidFactor,
	cloneMinBid,
	cloneMaxBid
};

int attributes[13];

public:
	//AgentAttributes();	//simple allocator
void init(const AgentAttributes& parentA, const AgentAttributes& parentB, int mutRate, int mutSev);
void init(int minThresh, int maxThresh);

int evaluateMoveBid(int energy, int income, int duration);
int evaluateCloneBid(int energy, int income, int duration);
int evaluateMateBid(int energy);
MateTarget* evaluateMateTarget(std::vector<MateTarget*> mates, Agent* self);

private:

};

#endif