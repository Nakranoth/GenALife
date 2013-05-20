#ifndef GEN_AGENT_H_
#define GEN_AGENT_H_

#include <boost/functional/hash.hpp>
#include <queue>

#include "workload.h"
#include "attributes.h"

class Agent{
public:
	int id;	//should be read-only
	int mateBid,
		moveBid,
		cloneBid,
		energy,	//accessed from agency when setting up mate-targets.
		age;	//as energy
	MateTarget* mateTarget;
	bool die,	//agents filp their own die flag. Agency then handles death.
		isThingamajig;	//results of evalutation.
	std::queue<unsigned short> incomes;	//used to calculate income.

private:
	int duration;
	float income;	//needs 1/N precision where N = incomes.size(). Thus, N should be a power of 2.
	
	bool counts,	//whether this round of execution causes loss of energy
		matingSeason;	//whether to set up mate bids 
	std::vector<MateTarget*> mates;

protected:
	Workload knowledge;	//attributes about PS.
	AgentAttributes stats; 	//constant attributes about life.

public:
	Agent(const Agent& parentA, const Agent& parentB, int mutRate, int mutSev);
	Agent(int minThresh, int maxThresh);	//used for initial populations.
	void ready(bool counts, const std::vector<MateTarget*>& mates, bool matingSeason);	
	void operator()();//thread called from agency. Do aging steps iff (counts).
	
	void clearIncomes();	//Used after a move to sanitize income. 
	
};

#endif //GEN_AGENT_H_
