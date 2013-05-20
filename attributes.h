#ifndef GEN_AGENTATTRIBUTES_H_
#define GEN_AGENTATTRIBUTES_H_

#include <vector>

#include "mateTarget.h"

class AgentAttributes{
public:

protected:
enum {	//Indecies of each attribute. Makes bulk attribute handling simpler inside functions.
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

int attributes[13];	//All attributes governing agent behavior. See paper for full description of each.

public:
void init(const AgentAttributes& parentA, const AgentAttributes& parentB, int mutRate, int mutSev);//birth
void init(int minThresh, int maxThresh);//initial population

int evaluateMoveBid(int energy, int income, int duration);//Whether to move and how much to bid.
int evaluateCloneBid(int energy, int income, int duration);//as above.
int evaluateMateBid(int energy);//as above again.
MateTarget* evaluateMateTarget(std::vector<MateTarget*> mates, Agent* self);

private:

};

#endif
