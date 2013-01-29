#include "agency.h"

using boost::asio::ip::tcp;

const std::string Agency::PORT = "21337";

Agency::Agency(int holdingPopulation, int mutRate, int mutSev, int msDelay, int trueRatio){
	this->holdingPopulation = holdingPopulation;
	mutagenRate = mutRate;
	mutagenImpact = mutSev;
	ProblemSpace(msDelay, trueRatio);
	clientResolver = new tcp::resolver(clientService);
	readyTCPStreams();
	initStartPop();//initialize rand pop.
	beginListen();
	for(;;){
		logicLoop();
	}
}

bool Agency::bidCompare(const Agent& a, const Agent& b){
	return a.mateBid < b.mateBid;
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
		agents.push_back(new Agent(0,1000));
		temp = new MateTarget();
		temp->agent = agents.back();
		temp->energy = &(temp->agent->energy);
		temp->age = &(temp->agent->age);
		mates.push_back(temp);
	}
	population = holdingPopulation;
}

void Agency::logicLoop(){
	Agent* target;
	std::sort(agents.begin(), agents.end(), energySort);
	while(*(agents.end())->die){
		for (vector<MateTarget*>::iterator i = mates.begin(); i < mates.end(); i++){
			if (*i->agent == *agents.end()){
				delete *i;
				erase(i);
				break;
			}
		}
		delete *agents.end();
		agents.pop_back();
		population--;
	}
	if(population < holdingPopulation){
		migrateIn();
	}
	bool counts = timeSinceAgentScored < 10 && timeSinceTrueItem < 10;	//THIS LINE NEEDS TO BE VARRIED TO DETERMINE IDEAL NUMBERS.
	bool matingSeason = population < holdingPopulation;
	for (vector<Agent*>::iterator i = agents.begin(); i < agents.end; i++){
		boost::thread(*i,counts,&mates,matingSeason);
	}
}

bool Agency::energySort(Agent* a, Agent* b){
	return a->energy > b->energy;	//causes sort to put them in decending rank by energy.
}
