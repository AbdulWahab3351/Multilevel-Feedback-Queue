#include "Process.h"
#include <iostream>
using namespace std;

Process::Process(int id, int arrival, int burst, int _priority)
{
	finishTime = responseTime = -1;
	priority = _priority;
	arrivalTime = arrival;
	remaingBurst = burstTime = burst;
	pId = id;
	timeOfLastExecution = 0;
}
