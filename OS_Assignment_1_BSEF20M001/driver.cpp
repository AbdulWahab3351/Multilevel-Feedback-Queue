// program in c++ to use priority_queue with structure
#include <iostream>
#include"MLFQ.h"
using namespace std;

int main()
{
	MLFQ q{ "Process_Data.txt" };
	q.simulate();
	return 0;
}
