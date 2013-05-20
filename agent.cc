#include "agent.h"

/********
* Typical way an agent is born; with two parents.
* mutRate and mutSev relate to mutation rates and severities. 
********/
Agent::Agent(const Agent& parentA, const Agent& parentB, int mutRate, int mutSev){
	knowledge.init(parentA.knowledge, parentB.knowledge, mutRate, mutSev);
	stats.init(parentA.stats, parentB.stats, mutRate, mutSev);
	boost::hash<Agent*> selfHash;
	id = selfHash(this);
}

/********
* Way agents are born when there are not enough parents to reach acceptable populations.
* min\max Thresh: Minimum\Maximum value preferences are allowed to have.
********/
Agent::Agent(int minThresh, int maxThresh){
	knowledge.init(minThresh, maxThresh);
	stats.init(minThresh, maxThresh);
	boost::hash<Agent*> selfHash;
	id = selfHash(this);
}

/********
* Prepares an agent internally for running. Sets all relevant internals to correct values. 
* Must always execute before threading.
********/
void Agent::ready(bool counts, const std::vector<MateTarget*>& mates, bool matingSeason){
	this->counts = counts;
	this->mates = mates;
	this->matingSeason = matingSeason;
}

/********
* Executed portion of agent's code. This is run from threads in the agency's threadpool. 
********/
void Agent::operator()(){
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
