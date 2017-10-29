#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <queue>
#include <algorithm>
#include <cmath>

//////////////////////////////////////////////////////////////////////////////////////////
// Structures, Variables and Prototypes
//////////////////////////////////////////////////////////////////////////////////////////

typedef struct Process
{
	std::string str_id;
	int arrival;
	int burst;

	Process(int id, int arrival, int burst) :
		str_id("P"),
		arrival(arrival),
		burst(burst)
	{
		int i = 0;
		int power10;
		int value;

		// Getting the max power10
		do
		{
			i++;
			power10 = std::pow(10, i);
			value = id % power10;

		} while (value != id);
		i--;
		power10 = std::pow(10, i);

		while (i != -1)
		{
			value = id / power10;
			id -= power10 * value;

			char c = '0' + value;
			str_id.push_back(c);

			i--;
			power10 = std::pow(10, i);
		} 
	}

	bool operator<(Process const &rhs) { return burst < rhs.burst; }

} Process;

typedef struct IsInt
{
	bool isInt = true;
	int value = 0;

	IsInt(std::string& check)
	{
		if (check.empty())
			isInt = false;

		for (int i = check.length() - 1; i >= 0; i--)
		{
			char valueI = check.at(i);
			if (valueI < '0' || valueI > '9')
			{
				isInt = false;
				return;
			}

			value += std::pow(10, check.length() - i - 1) * (valueI - '0');
		}
	}

} IsInt;

std::ifstream ifs_config;
bool RR = false;
int quantum = 0;
std::deque<Process*> deque_ready; // We need to use a double sided queue so that std::sort is able to sort it. 

//////////////////////////////////////////////////////////////////////////////////////////
// General Functions
//////////////////////////////////////////////////////////////////////////////////////////

void setup(int argc, char ** argv)
{
	// If it is 3 then it must be SJF, if 4 must be RR with the quantum
	if (argc != 3 && argc != 4)
	{
		std::cout << "Uasge: scheduler <config.txt> <TYPE (RR or SJF)> <(Only if RR) quantum>" << std::endl;
		exit(-1);
	}
	
	// Try to open the file
	ifs_config.open(argv[1]);
	if (!ifs_config.is_open())
	{
		std::cout << "Error, could not open the file: " << argv[1] << std::endl;
		exit(-1);
	}

	// Get the sheduler type and check for the appropriate 3rd command. 
	std::string type(argv[2]);
	if (type.compare("RR") == 0 || type.compare("rr") == 0)
	{
		if (argc != 4)
		{
			std::cout << "Error, when using RR please enter a quantum as the last argument." << std::endl;
			exit(-1);
		}

		std::string temp(argv[3]);
		IsInt IsInt_input_quantum(temp);
		if (!IsInt_input_quantum.isInt) 
		{
			std::cout << "Error, please enter an integer value for the quantum" << std::endl;
			exit(-1);
		}

		quantum = IsInt_input_quantum.value;
		RR = true;
	}
	else if (type.compare("SJF") == 0 || type.compare("sjf") == 0)
	{
		if (argc == 4)
		{
			std::cout << "Error, when using SJF do not enter a quantum value (no last argument)." << std::endl;
			exit(-1);
		}
	}

	std::string typeName((RR ? "Round Robin" : "Shortest Job First"));
	std::string withQuant((argc == 4 ? ", at a quantum of: " : ""));
	if (!withQuant.empty()) withQuant.append(argv[3]);
	std::cout << "Reading: " << argv[1] << ", using: " << typeName << withQuant << std::endl;
}

int main(int argc, char ** argv)
{
	setup(argc, argv);

	std::string line;
	int id = 0;
	while (ifs_config.good() && std::getline(ifs_config, line))
	{
		std::string arrival;
		std::string burst;

		int i = 0;
		for (; line.at(i) != ' '; i++)
			arrival.push_back(line.at(i));
		i++;

		IsInt ii_arrival(arrival);
		if (!ii_arrival.isInt)
		{
			std::cout << "Error, a value in the input file is not an integer: " << arrival << std::endl;
			exit(-1);
		}

		for (; i < line.size() && line.at(i) != '\r'; i++)
			burst.push_back(line.at(i));

		IsInt ii_burst(burst);
		if (!ii_burst.isInt)
		{
			std::cout << "Error, a value in the input file is not an integer: " << burst << std::endl;
			exit(-1);
		}

		deque_ready.push_front(new Process(id, ii_arrival.value, ii_burst.value));
		id++;
	}
		

	// Note that RR is preemptive FCFS, so the queue is already in the proper order for that. We must sort for SJF though:
	// Sorting to ascending order via the structs less than operator, which is defined around burst time. 
	if (!RR)
		std::sort(deque_ready.begin(), deque_ready.end());
}
