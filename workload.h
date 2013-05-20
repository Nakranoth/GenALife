#ifndef _GENE_WORKLOAD_H_
#define _GENE_WORKLOAD_H_

#include "problemSpace.h"

class Workload{
	//****************************************************************
	//* 15 PS attributes, 2 BG multipliers, 2 BG adder, 1 FG
	//* multiplier, 1 FG adder. Note: BG maths look only at raw
	//* attributes. FG maths look at BG and FG maths only.
	//*
	//* Collumns are:	type, min,   max	for int type data
	//*					type, ~flag, flag	for flag type data
	//*				Note: if type == 0 type always = 0.
	//*					otherwise it may be between 1-7(odd) evolutionarily.
	//*					type & 2 ? (flags & c1 == c1) : (flags & c1)
	//*					type & 4 ? (flags & c2 == c2) : (flags & c2)
	//*					
	//* For BG maths: flags (30-bit), results
	//* For FG maths: flags (12-bit), min, max
	//****************************************************************
	protected:
		int thresholds[21][3];
		static ProblemSpace PS;
	
	public:
	Workload();	//explicit default constructor for agent declarations.
	
	void init(const Workload& parentA, const Workload& parentB, int mutRate, int mutSev);
	void init(int minThresh, int maxThresh);

	bool evaluate();
};
ProblemSpace Workload::PS(20,20);//TODO:Make proper init function in main.

#endif
