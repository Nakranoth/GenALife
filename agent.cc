#include "agent.h"

Agent::Agent(const Agent& parentA, const Agent& parentB, int mutRate, int mutSev){
	knowledge.init(parentA.knowledge, parentB.knowledge, mutRate, mutSev);
	stats.init(parentA.stats, parentB.stats, mutRate, mutSev);
	boost::hash<Agent*> selfHash;
	id = selfHash(this);
}

Agent::Agent(int minThresh, int maxThresh){
	knowledge.init(minThresh, maxThresh);
	stats.init(minThresh, maxThresh);
	boost::hash<Agent*> selfHash;
	id = selfHash(this);
}

void Agent::operator()(bool counts, const std::vector<MateTarget*>& mates, bool matingSeason){
	if (counts){	//have semi-recently had a thingamajig
		age++;
		energy--;
		duration++;
		if (incomes.size() >= 32){
			if (incomes.front()) income -= incomes.front() / 32;
			incomes.pop();
		}
		if (incomes.back()){
			income += incomes.back() / 32;
		}
	}

	if (energy <= 0){
		die = true;
		return;
	}
	
	if (matingSeason){
		mateBid = stats.evaluateMateBid(energy);
		mateTarget = stats.evaluateMateTarget(mates, this);
	}	
	isThingamajig = knowledge.evaluate();
	moveBid = stats.evaluateMoveBid(energy, income, duration);
	cloneBid = stats.evaluateCloneBid(energy, income, duration);
}

void Agent::clearIncomes(){
	income = 0;
	while (incomes.front()){
		incomes.pop();
	}
}