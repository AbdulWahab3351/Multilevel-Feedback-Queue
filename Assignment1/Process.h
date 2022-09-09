#ifndef PROCESS_H
#define PROCESS_H

struct Process
{
	int pId;
	int arrivalTime;
	int burstTime;
	int priority;
	int remaingBurst;
	int finishTime;
	int responseTime;
	int timeOfLastExecution;
	Process(int id = 0, int arrival = 0, int burst = 0, int _priority = 0);	
};
struct CompareProcess{
	bool operator()(Process const& p1, Process const& p2)
	{
		return p1.burstTime >= p2.burstTime;
	}
};


#endif // !PROCESS_H


