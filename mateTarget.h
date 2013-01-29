#ifndef GEN_MATETARGET_H_
#define GEN_MATETARGET_H_

//*********************************************************************
//* Wrapper for agent pointer and required data in attributes to bypass
//* circular dependancy. Populated from agency.
//*********************************************************************


class Agent;

struct MateTarget{
	Agent* agent;
	int* energy;
	int* age;
};

#endif