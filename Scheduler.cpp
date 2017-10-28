#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

//////////////////////////////////////////////////////////////////////////////////////////
// Structures, Variables and Prototypes
//////////////////////////////////////////////////////////////////////////////////////////

typedef struct Process
{
	int id;
	int arrival;
	int burst;

	Process(int id, int arrival, int burst) :
		id(id),
		arrival(arrival),
		burst(burst)
	{}
} Process;

std::ifstream ifs_config;
bool RR = false;
int quantum = 0;

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
	char quantum_num;
	if (type.compare("RR") == 0 || type.compare("rr") == 0)
	{
		if (argc != 4)
		{
			std::cout << "Error, when using RR please enter a quantum as the last argument." << std::endl;
			exit(-1);
		}

		quantum_num = argv[3][0] - '0';
		if (quantum_num < 0 || quantum_num > 9) // Therefore the original input was not an integer
		{
			std::cout << "Error, please enter an integer value for the quantum" << std::endl;
			exit(-1);
		}

		quantum = quantum_num;
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
}