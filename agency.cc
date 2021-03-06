#include "agency.h"

/***
* Main logic driver of entire simulator.
* Each node on a network should have exactly one.
***/

using boost::asio::ip::tcp;

const std::string Agency::PORT = "21337";

/***
* holdingPopulation: Minimum number of agents in a node. If there are fewer, agents may migrate in and/or reproduce.
* mutRate: Each birth, there is a 1/mutRate chance of a mutation.
* mutSev: The maximum severity of a mutation.
* msDelay: The wait in ms between each item being evaluated in the problem space.
* trueRatio: 1/trueRatio = ratio of thingamajigs to non-thingamajigs.
***/
Agency::Agency(int holdingPopulation, int mutRate, int mutSev, int msDelay, int trueRatio){
	this->holdingPopulation = holdingPopulation;
	mutagenRate = mutRate;
	mutagenImpact = mutSev;
	ProblemSpace(msDelay, trueRatio);
	//clientResolver = new tcp::resolver(serverService);
	readyTCPStreams();
	initStartPop();//initialize rand pop.
	beginListen();
	for(;;){
		logicLoop();
	}
}

void Agency::readyTCPStreams(){
	std::ifstream infile;
	std::string line;
	try{
		infile.open("iplist", std::ifstream::in);
		for(;;){
			std::getline(infile,line);
			destinations.push_back(new tcp::resolver::query(line, PORT));	//load an IP as a possible query.
		}
	}
	catch(...){
		if (infile.bad()){
			std::cerr << "File read exception(iplist). Exiting.\n";
			std::exit(1);
		}
		infile.close();
	}
}

void Agency::initStartPop(){
	MateTarget* temp;
	for (int i = 0; i < holdingPopulation; i++){
		agents.push_back(Agent(0,1000));
		temp = new MateTarget();
		temp->agent = &(agents.back());
		temp->energy = &(temp->agent->energy);
		temp->age = &(temp->agent->age);
		mates.push_back(temp);
	}
	population = holdingPopulation;
}

/**
* All agent handling happens here. 
*/
void Agency::logicLoop(){
	Agent* target;
	agents.sort(energySortatron());
	while(agents.end()->die){
		for (std::vector<MateTarget*>::iterator i = mates.begin(); i < mates.end(); i++){	//delete from mates.
			if ((*i)->agent == &(*agents.end())){
				delete *i;
				mates.erase(i);
				break;
			}
		}
		agents.pop_back();
		population--;
	}
	if(population < holdingPopulation){
		migrateIn();
	}
	bool counts = timeSinceAgentScored < 10 && timeSinceTrueItem < 10;	//THIS LINE NEEDS TO BE VARRIED TO DETERMINE IDEAL NUMBERS.
	bool matingSeason = population < holdingPopulation;
	boost::threadpool::pool threadPool(4);	//Number of agents to compute at once.
	for (std::list<Agent>::iterator i = agents.begin(); i != agents.end(); i++){
		i->ready(counts,mates,matingSeason);
		threadPool.schedule(*i);
	}
	threadPool.wait();
}

/**
* Starts the listening service(s). May require callback funtion to connection functions.
* Callback function should load the results directly into a memory space for a new agent in income queue.
*/
void Agency::beginListen(){
	
}

/**
* Simply pulls agents from the income queue into the general population.
* Remember to add associated mateTargets.
*/
void Agency::migrateIn(){
	
}
