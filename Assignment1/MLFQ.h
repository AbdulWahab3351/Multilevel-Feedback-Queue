#ifndef MLFQ_H
#define MLFQ_H
#include "Process.h"
#include <string>
using namespace std;
#include <vector>
#include <queue>
#include "Queue.h"

class MLFQ
{
	vector<Process> v1;
	vector<Process> v2;
	vector<Process> v3;
	int arrivalTimeOfLastProcess;

	void addProcessesInQueue(Queue<Process>& q1, Queue<Process>& q2,
		priority_queue<Process, vector<Process>, CompareProcess>& q3, int counter);
	void updateArrays(Process p);
	void performAging(Queue<Process>&q1,Queue<Process>& q2, priority_queue<Process,
		vector<Process>, CompareProcess>& q3, int counter);
	void incrementCounter(Queue<Process>& q2, priority_queue<Process,
		vector<Process>, CompareProcess>& q3);
	void displayReadyQueues(Queue<Process>& q1, Queue<Process>& q2,
		priority_queue<Process, vector<Process>, CompareProcess>& q3);
	void showTimes();
public:
	MLFQ(string filename);
	void simulate();	
};


#endif // !MLFQ_H


