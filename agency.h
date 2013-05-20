#ifndef GEN_AGENCY_H_
#define GEN_AGENCY_H_

#include "mateTarget.h"
#include "agent.h"
#include "problemSpace.h"

#include "boost/threadpool.hpp"

#include <list>
#include <vector>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>

#define NUM_THREADS 4

class Agency{
public:
static const std::string PORT;	//only available here.
private:
int holdingPopulation,
	population,
	timeSinceTrueItem,
	timeSinceAgentScored,
	avgAge,
	mutagenRate,
	mutagenImpact;

std::list<Agent> agents;
std::vector<MateTarget*> mates;

std::vector<Agent> income;	//Agents arriving from foreign locations.

std::ifstream ipAddrs;
boost::asio::io_service serverService;	//client services will be made ephemerally as needed
boost::asio::ip::tcp::resolver* clientResolver;
std::vector<boost::asio::ip::tcp::resolver::query*> destinations;	//pre-loaded queries for the resolver to handle
	
public:
Agency(int holdingPopulation, int mutRate, int mutSev, int msDelay, int trueRatio);

private:
void readyTCPStreams();
void initStartPop();
void logicLoop();
void beginListen();
void migrateIn();	//TODO: Write this function. Just pulls from incoming queue to population.
struct bidSortatron{inline bool operator()(const Agent& a, const Agent& b){return a.mateBid < b.mateBid;}};
struct energySortatron{inline bool operator()(const Agent& a, const Agent& b){return a.energy > b.energy;}};
};

#endif
