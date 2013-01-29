#include "workload.h"

//ProblemSpace* Workload::PS = 0;	//Allocate PS.

void Workload::init(const Workload& parentA, const Workload& parentB, int mutRate, int mutSev){
	for (int i = 0; i < 15; i++){//flag or int
		if (parentA.thresholds[i][0]){	//flags
			if (rand() < RAND_MAX / 2){
				thresholds[i][0] |= parentA.thresholds[i][0] & 0x2;
			}
			else{
				thresholds[i][0] |= parentB.thresholds[i][0] & 0x2;
			}
			if (rand() < RAND_MAX / mutRate) thresholds[i][0] ^= 0x2; //toggle
			if (rand() < RAND_MAX / 2){
				thresholds[i][0] |= parentA.thresholds[i][0] & 0x4;
			}
			else{
				thresholds[i][0] |= parentB.thresholds[i][0] & 0x4;
			}
			if (rand() < RAND_MAX / mutRate) thresholds[i][0] ^= 0x4; //toggle
			
			//select each bit.
			int shiftA1 = parentA.thresholds[i][1];
			int shiftA2 = parentA.thresholds[i][2];
			int shiftB1 = parentB.thresholds[i][1];
			int shiftB2 = parentB.thresholds[i][2];
			for (int j = 0; j < sizeof(int) * 8; i++){
				if (rand() < RAND_MAX / 2){
					thresholds[i][1] |= (shiftA1 & 0x1) << j;
				}
				else{
					thresholds[i][1] |= (shiftB1 & 0x1) << j;
				}
				if (rand() < RAND_MAX / mutRate) thresholds[i][1] ^= 0x1 << j;//toggle
				
				if (rand() < RAND_MAX / 2){
					thresholds[i][2] |= (shiftA2 & 0x1) << j;
				}
				else{
					thresholds[i][2] |= (shiftB2 & 0x1) << j;
				}
				if (rand() < RAND_MAX / mutRate) thresholds[i][2] ^= 0x1 << j;//toggle
				shiftA1 >>= 1;
				shiftA2 >>= 1;
				shiftB1 >>= 1;
				shiftB2 >>= 1;
			}
		}
		else{//int
			if (rand() < RAND_MAX / 2){
				thresholds[i][1] = parentA.thresholds[i][1];
			}
			else{
				thresholds[i][1] = parentB.thresholds[i][1];
			}
			if (rand() < RAND_MAX / mutRate){
				thresholds[i][1] += (rand() / (RAND_MAX / (mutSev * 2))) - mutSev;	//rand from +-mutSev
			}
			if (rand() < RAND_MAX / 2){
				thresholds[i][2] = parentA.thresholds[i][2];
			}
			else{
				thresholds[i][2] = parentB.thresholds[i][2];
			}
			if (rand() < RAND_MAX / mutRate){
				thresholds[i][2] += (rand() / (RAND_MAX / (mutSev * 2))) - mutSev;	//rand from +-mutSev
			}
		}
	}
	
	for (int i = 15; i < 21; i++){//flags
		int shiftA0 = parentA.thresholds[i][0];
		int shiftB0 = parentB.thresholds[i][0];
		for (int j = 0; j < sizeof(int) * 8; i++){
			if (rand() < RAND_MAX / 2){
				thresholds[i][0] |= (shiftA0 & 0x1) << j;
			}
			else{
				thresholds[i][0] |= (shiftB0 & 0x1) << j;
			}
			if (rand() < RAND_MAX / mutRate) thresholds[i][0] ^= 0x1 << j;//toggle
			shiftA0 >>= 1;
			shiftB0 >>= 1;
		}
		if (i >= 19){
			if (rand() < RAND_MAX / 2){
				thresholds[i][1] = parentA.thresholds[i][1];
			}
			else{
				thresholds[i][1] = parentB.thresholds[i][1];
			}
			if (rand() < RAND_MAX / mutRate){
				thresholds[i][1] += (rand() / (RAND_MAX / (mutSev * 2))) - mutSev;	//rand from +-mutSev
			}
			if (rand() < RAND_MAX / 2){
				thresholds[i][2] = parentA.thresholds[i][2];
			}
			else{
				thresholds[i][2] = parentB.thresholds[i][2];
			}
			if (rand() < RAND_MAX / mutRate){
				thresholds[i][2] += (rand() / (RAND_MAX / (mutSev * 2))) - mutSev;	//rand from +-mutSev
			}
		}
	}
}

void Workload::init(int minThresh, int maxThresh){
	for (int i = 0; i < 15; i++){
		thresholds[i][1] = rand() / (RAND_MAX / (maxThresh - minThresh + 1)) + minThresh;
		thresholds[i][2] = rand() / (RAND_MAX / (maxThresh - minThresh + 1)) + minThresh;
		if (i == 4 || i == 8){//flags
			thresholds[i][0] = rand();
			thresholds[i][0] &= 0x7;//reduce to valid flags.
			thresholds[i][0] |= 0x1;//set flags flag.
		}
		else{
			thresholds[i][0] = 0;
				//ensure max is larger than min. Should make survival much more likely.
			if (thresholds[i][1] > thresholds[i][2]){
				thresholds[i][1] ^= thresholds[i][2];
				thresholds[i][2] ^= thresholds[i][1];
				thresholds[i][1] ^= thresholds[i][2];
			}
		}
	}
}

void Workload::init(const ProblemSpace& PSin){
	PS = &PSin;
}

bool Workload::evaluate(){
	PS->rwControl->lock_sharable();
	bool ret = true;
	
	for (int i = 0; i < 15; i++){
		if (thresholds[i][0] == 0){	//int
			if (PS->attributes[i] < thresholds[i][1] || PS->attributes[i] > thresholds[i][2]){
				ret = false;
				break;
			}
		}
		else{	//flags
			if (thresholds[i][0] & 0x2){
				if (thresholds[i][1] & PS->attributes[i] != thresholds[i][1]){
					ret = false;
					break;
				}
			}
			else{
				if (!(thresholds[i][1] & PS->attributes[i])){
					ret = false;
					break;
				}
			}
			if (thresholds[i][0] & 0x4){
				if (thresholds[i][2] & ~PS->attributes[i] != thresholds[i][2]){
					ret = false;
					break;
				}
			}
			else{
				if (!(thresholds[i][2] & ~PS->attributes[i])){
					ret = false;
					break;
				}
			}
		}
	}
	
	//bg mults
	float m1res = 1.0, m2res = 1.0;
	int m1shift = thresholds[15][0], m2shift = thresholds[16][0];
	for (int i = 0; (m1shift || m2shift) && i < 15; i++){
		if (m1shift & 0x1){
			if (m1shift & 0x2){
				m1res /= PS->attributes[i];
			}
			else{
				m1res *= PS->attributes[i];
			}
		}
		m1shift >>= 2;
		if (m2shift & 0x1){
			if (m2shift & 0x2){
				m2res /= PS->attributes[i];
			}
			else{
				m2res *= PS->attributes[i];
			}
		}
		m2shift >>= 2;
	}
	thresholds[15][1] = (int)m1res;
	thresholds[16][1] = (int)m2res;
	
	int a1res = 0, a2res = 0;
	m1shift = thresholds[17][0], m2shift = thresholds[18][0];
	for (int i = 0; (m1shift || m2shift) && i < 15; i++){
		if (m1shift & 0x1){
			if (m1shift & 0x2){
				m1res -= PS->attributes[i];
			}
			else{
				m1res += PS->attributes[i];
			}
		}
		m1shift >>= 2;
		if (m2shift & 0x1){
			if (m2shift & 0x2){
				m2res -= PS->attributes[i];
			}
			else{
				m2res += PS->attributes[i];
			}
		}
		m2shift >>= 2;
	}
	thresholds[17][1] = a1res;
	thresholds[18][1] = a2res;
	
	m1res = 1.0, a2res = 1;
	m1shift = thresholds[19][0], m2shift = thresholds[20][0];
	for (int i = 0; (m1shift || m2shift) && i < 4; i++){
		if (m1shift & 0x1){
			if (m1shift & 0x2){
				m1res /= thresholds[i+15][1];
			}
			else{
				m1res *= thresholds[i+15][1];
			}
		}
		m1shift >>= 2;
		if (m2shift & 0x1){
			if (m2shift & 0x2){
				a2res -= thresholds[i+15][1];
			}
			else{
				a2res += thresholds[i+15][1];
			}
		}
		m2shift >>= 2;
	}
	
	if (m1res < thresholds[19][1] || m1res > thresholds[19][2]){
		ret = false;
	}
	if (a2res < thresholds[20][1] || m1res > thresholds[20][2]){
		ret = false;
	}
	
	PS->rwControl->unlock_sharable();
	return ret;
}