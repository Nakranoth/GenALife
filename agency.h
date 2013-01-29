#ifndef GEN_AGENCY_H_
#define GEN_AGENCY_H_

#include "mateTarget.h"
#include "agent.h"
#include "problemSpace.h"

#include <vector>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

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

std::vector<Agent*> agents;
std::vector<MateTarget*> mates;

std::ifstream ipAddrs;

boost::asio::io_service serverService;	//client services will be made ephemerally as needed
boost::asio::ip::tcp::resolver* clientResolver;
std::vector<boost::asio::ip::tcp::resolver::query*> destinations;	//pre-loaded queries for the resolver to handle
	
public:
Agency(int holdingPopulation, int mutRate, int mutSev, int msDelay, int trueRatio);

private:
bool bidCompare(const Agent& a, const Agent& b);	//allows sort function.
void readyTCPStreams();
void initStartPop();
void logicLoop();
};

#endif