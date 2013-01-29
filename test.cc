#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

class B{
	public:
	int dataB;
	int array[8];
	B(){};
	
	void setB(int b){
		dataB = b;
		for (int i = 0; i < 8; i++){
			array[i] = i;
		}
	}
};

class A{
	public:
	int dataA;

	B myB;
	A(int a, int b){
		dataA = a;
		myB.setB(b);
	}
};

int main(int argc, char* argv[]){
	A* origA = new A(18,100);
	
	cout << sizeof(*origA) << '\n';
	cout << sizeof(origA->myB) << '\n';
	
	char* newAStr[sizeof(*origA)];
	memcpy(newAStr,origA,sizeof(*origA));
	A* newA = (A*)malloc(sizeof(*origA));
	memcpy(newA,newAStr,sizeof(*origA));
	
	//modify one to see if both change.
	for (int i = 0; i < 8; i++){
		newA->myB.array[i]++;
	}
	
	//print "origA"
	cout << origA->dataA << ", " << origA->myB.dataB;
	for (int i = 0; i < 8; i++){
		cout << ", " << origA->myB.array[i];
	}
	cout << '\n';
	
	//print oldA
	cout << newA->dataA << " " << newA->myB.dataB;
	for (int i = 0; i < 8; i++){
		cout << ", " << newA->myB.array[i];
	}
	cout << '\n';
}