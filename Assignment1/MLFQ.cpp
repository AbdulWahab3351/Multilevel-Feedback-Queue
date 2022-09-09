#include "MLFQ.h"
#include <fstream>
#include <iostream>

MLFQ::MLFQ(string filename)
{
	ifstream fin{ filename };
	if (!fin)
	{
		cout << "File not found\n";
		return;
	}
	int id;
	int arrival;
	int burst;
	int priority;
	arrivalTimeOfLastProcess = -1;
	while (!fin.eof())
	{
		fin >> id >> arrival >> burst >> priority;
		if (arrival > arrivalTimeOfLastProcess)
			arrivalTimeOfLastProcess = arrival;
		if (priority <= 5)
			v1.push_back(Process(id, arrival, burst, priority));
		else if (priority <= 15)
			v2.push_back(Process(id, arrival, burst, priority));
		else
			v3.push_back(Process(id, arrival, burst, priority));
	}
}

void MLFQ:: simulate()
{
	Queue<Process> q1;
	Queue<Process> q2;
	priority_queue<Process, vector<Process>, CompareProcess> q3;
	int counter = 0;
	int quantumQ1 = 5;
	int quantumQ2 = 8;
	string timeLine;
	string str;
	bool isIdle = false;
	addProcessesInQueue(q1, q2, q3, counter);
	while (counter <= arrivalTimeOfLastProcess || !q1.isEmpty() || !q2.isEmpty() || !q3.empty())
	{
		if (q1.isEmpty() && q2.isEmpty() && q3.empty())
		{
			if (!isIdle)
			{
				isIdle = true;
				str = to_string(counter) + ": idle ";
				timeLine += str;
				cout << str << '\n';
				displayReadyQueues(q1, q2, q3);
			}
			counter++;
			addProcessesInQueue(q1, q2, q3, counter);
		}
		else if (!q1.isEmpty())
		{
			isIdle = false;
			while (!q1.isEmpty())
			{
				Process p = q1.deQueue();
				if (p.burstTime == p.remaingBurst)
					p.responseTime = counter - p.arrivalTime;
				str = to_string(counter) + ": P" + to_string(p.pId) + ' ';
				timeLine += str;
				cout << str << '\n';
				displayReadyQueues(q1, q2, q3);
				for (int i = 0; i < quantumQ1 && p.remaingBurst; i++)
				{
					p.remaingBurst--;
					incrementCounter(q2, q3);
					performAging(q1, q2, q3, counter);
					counter++;
					addProcessesInQueue(q1, q2, q3, counter);
				}
				if (p.remaingBurst)
				{
					p.timeOfLastExecution = counter;
					q2.enQueue(p);
				}
				else
				{
					p.finishTime = counter;
					updateArrays(p);
				}
			}

		}
		else if (!q2.isEmpty())
		{
			isIdle = false;
			Process p = q2.deQueue();
			if (p.burstTime == p.remaingBurst)
				p.responseTime = counter - p.arrivalTime;
			str = to_string(counter) + ": P" + to_string(p.pId) + ' ';
			timeLine += str;
			cout << str << '\n';
			displayReadyQueues(q1, q2, q3);
			for (int i = 0; i < quantumQ2 && p.remaingBurst; i++)
			{
				p.remaingBurst--;
				incrementCounter(q2, q3);
				performAging(q1, q2, q3, counter);
				counter++;
				addProcessesInQueue(q1, q2, q3, counter);
			}
			if (p.remaingBurst)
			{
				p.timeOfLastExecution = counter;
				q3.push(p);
			}
			else
			{
				p.finishTime = counter;
				updateArrays(p);
			}
		}
		else
		{
			isIdle = false;
			Process p = q3.top();
			q3.pop();
			if (p.burstTime == p.remaingBurst)
				p.responseTime = counter - p.arrivalTime;
			str = to_string(counter) + ": P" + to_string(p.pId) + ' ';
			timeLine += str;
			cout << str << '\n';
			displayReadyQueues(q1, q2, q3);
			while (p.remaingBurst)
			{
				p.remaingBurst--;
				incrementCounter(q2, q3);
				performAging(q1, q2, q3, counter);
				counter++;
				addProcessesInQueue(q1, q2, q3, counter);
			}
			p.finishTime = counter;
			updateArrays(p);
		}
	}
	cout << "\n---Gantt Chart---\n";
	cout << timeLine << '\n';
	showTimes();
}
void MLFQ:: addProcessesInQueue(Queue<Process>& q1, Queue<Process>& q2,
	priority_queue<Process, vector<Process>, CompareProcess>& q3, int counter)
{
	int s1 = v1.size();
	int s2 = v2.size();
	int s3 = v3.size();
	Process p;
	for (int i = 0; i < s1; i++)
	{
		p = v1[i];
		if (p.arrivalTime == counter)
			q1.enQueue(p);
	}
	for (int i = 0; i < s2; i++)
	{
		p = v2[i];
		if (p.arrivalTime == counter)
			q2.enQueue(p);
	}
	for (int i = 0; i < s3; i++)
	{
		p = v3[i];
		if (p.arrivalTime == counter)
			q3.push(p);
	}
}
void MLFQ::updateArrays(Process p)
{
	Process temp;
	int s1 = v1.size();
	for (int i = 0; i < s1; i++)
	{
		temp = v1[i];
		if (temp.pId == p.pId)
		{
			v1[i] = p;
			return;
		}
	}
	int s2 = v2.size();
	for (int i = 0; i < s2; i++)
	{
		temp = v2[i];
		if (temp.pId == p.pId)
		{
			v2[i] = p;
			return;
		}
	}
	int s3 = v3.size();
	for (int i = 0; i < s3; i++)
	{
		temp = v3[i];
		if (temp.pId == p.pId)
		{
			v3[i] = p;
			return;
		}
	}
}
void MLFQ:: performAging(Queue<Process>& q1, Queue<Process>& q2, priority_queue<Process,
	vector<Process>, CompareProcess>& q3, int counter)
{
	if (q2.isEmpty() && q3.empty())
		return;
	if (!q2.isEmpty())
	{
		while (!q2.isEmpty() && (counter - q2.getElementAtFront().timeOfLastExecution == 30))
			q1.enQueue(q2.deQueue());
	}
	if (!q3.empty())
	{
		priority_queue<Process, vector<Process>, CompareProcess> temp3;
		while (!q3.empty())
		{
			Process p = q3.top();
			q3.pop();
			if (counter - p.timeOfLastExecution == 20)
				q2.enQueue(p);
			else
				temp3.push(p);
		}
		while (!temp3.empty())
		{
			q3.push(temp3.top());
			temp3.pop();
		}
	}
}
void MLFQ::incrementCounter(Queue<Process> &q2, 
	priority_queue<Process,vector<Process>, CompareProcess>& q3)
{
	Queue<Process> temp2;
	priority_queue<Process, vector<Process>, CompareProcess> temp3;
	while (!q2.isEmpty())
	{
		Process p = q2.deQueue();
		p.timeOfLastExecution++;
		temp2.enQueue(p);
	}
	while (!q3.empty())
	{
		Process p = q3.top();
		q3.pop();
		p.timeOfLastExecution++;
		temp3.push(p);
	}
	while (!temp2.isEmpty())
		q2.enQueue(temp2.deQueue());
	while (!temp3.empty())
	{
		Process p = temp3.top();
		temp3.pop();
		q3.push(p);
	}
}

void MLFQ:: displayReadyQueues(Queue<Process>& q1, Queue<Process>& q2,
	priority_queue<Process, vector<Process>, CompareProcess>& q3)
{
	Queue<Process> temp;
	priority_queue<Process, vector<Process>, CompareProcess> temp3;
	cout << "---Ready Queue 1---\n";
	while (!q1.isEmpty())
	{
		Process p = q1.deQueue();
		cout << 'p' << p.pId << ", ";
		temp.enQueue(p);
	}
	while (!temp.isEmpty())
		q1.enQueue(temp.deQueue());
	cout << '\n';
	cout << "---Ready Queue 2---\n";
	while (!q2.isEmpty())
	{
		Process p = q2.deQueue();
		cout << 'p' << p.pId << ", ";
		temp.enQueue(p);
	}
	cout << '\n';
	while (!temp.isEmpty())
		q2.enQueue(temp.deQueue());
	cout << "---Ready Queue ---\n";
	while (!q3.empty())
	{
		Process p = q3.top();
		q3.pop();
		cout << 'p' << p.pId << ", ";
		temp3.push(p);
	}
	while (!temp3.empty())
	{
		q3.push(temp3.top());
		temp3.pop();
	}
	cout << "\n\n";
}
void MLFQ:: showTimes()
{
	int s1 = v1.size();
	int s2 = v2.size();
	int s3 = v3.size();
	int totalWaitingTime = 0;
	int totalTurnAroundTime = 0;
	int toalResponseTime = 0;
	int responseTim;
	int waitingTim;
	int turnAroundTim;
	for (int i = 0; i < s1; i++)
	{
		turnAroundTim = v1[i].finishTime - v1[i].arrivalTime;
		waitingTim = turnAroundTim - v1[i].burstTime;
		responseTim = v1[i].responseTime;
		cout << "\nProcess P" << v1[i].pId << '\n';
		cout << "Waiting Time : " << waitingTim << '\n';
		cout << "Response Time : " << responseTim << '\n';
		cout << "Turn Around Time : " << turnAroundTim << '\n';
		totalTurnAroundTime += turnAroundTim;
		totalWaitingTime += waitingTim;
		toalResponseTime += responseTim;
	}
	for (int i = 0; i < s2; i++)
	{
		turnAroundTim = v2[i].finishTime - v2[i].arrivalTime;
		waitingTim = turnAroundTim - v2[i].burstTime;
		responseTim = v2[i].responseTime;
		cout << "\nProcess P" << v2[i].pId << '\n';
		cout << "Waiting Time : " << waitingTim << '\n';
		cout << "Response Time : " << responseTim << '\n';
		cout << "Turn Around Time : " << turnAroundTim << '\n';
		totalTurnAroundTime += turnAroundTim;
		totalWaitingTime += waitingTim;
		toalResponseTime += responseTim;
	}
	for (int i = 0; i < s3; i++)
	{
		turnAroundTim = v3[i].finishTime - v3[i].arrivalTime;
		waitingTim = turnAroundTim - v3[i].burstTime;
		responseTim = v3[i].responseTime;
		cout << "\nProcess P" << v3[i].pId << '\n';
		cout << "Waiting Time : " << waitingTim << '\n';
		cout << "Response Time : " << responseTim << '\n';
		cout << "Turn Around Time : " << turnAroundTim << '\n';
		totalTurnAroundTime += turnAroundTim;
		totalWaitingTime += waitingTim;
		toalResponseTime += responseTim;
	}
	cout << "\nAverage Waiting Time : " << (double)totalWaitingTime / (s1 + s2 + s3) << '\n';
	cout << "Average Turn Around Time : " << (double)totalTurnAroundTime / (s1 + s2 + s3) << '\n';
	cout << "Average Response Time : " << (double)toalResponseTime / (s1 + s2 + s3) << '\n';
}