#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class node
{
	public:
		int jobId;
		int jobTime;

		// Constructor
		node(int, int);
};

node :: node(int id, int time)
{
	this->jobId = id;
	this->jobTime = time;
}


class qNode
{
	public:
		int jobId;
		int num_of_dpts;
		qNode* next;

		qNode();
		qNode(int, int);
};

// Dummy node
qNode :: qNode()
{
	this->jobId = 999;
	this->num_of_dpts = 9999;
	this->next = NULL;
}

qNode :: qNode(int id, int n)
{
	this->jobId = id;
	this->num_of_dpts = n;
	this->next = NULL;
}

class linkedList
{
	public:
		qNode* head;

		// Constructors
		linkedList();
		void insert2Open(qNode *);
		qNode * removal();
		void printList();
};

linkedList :: linkedList()
{
	this->head = new qNode();
}

void linkedList :: insert2Open(qNode *newNode)
{
	qNode* curStep = this->head;
	qNode* nextStep = curStep->next;
	
	while(nextStep != NULL)
	{
		// The newNode order is in between
		if(newNode->num_of_dpts < curStep->num_of_dpts &&
			newNode->num_of_dpts > nextStep->num_of_dpts)
		{
			newNode->next = nextStep;
			curStep->next = newNode;
			return;
		}
		else
		{
			curStep->next = nextStep;
			nextStep = curStep->next;
		}
	}

	// The queue is either empty or it reaches to the end of queue
	newNode->next = nextStep;
	curStep->next = newNode;
	return;
}

qNode* linkedList :: removal()
{
	qNode* temp = this->head->next;
	if(temp == NULL)
		return NULL;
	else
	{
		this->head = this->head->next;
	}
	return temp;
}

void linkedList :: printList()
{
	qNode* ptr = new qNode();
	ptr = this->head->next;
	cout << "OPEN -> ";
	while(ptr != NULL)
	{
		cout << ptr->jobId << " -> ";
		ptr = ptr->next;
	}
	cout << "NULL";
	return;
}

class Scheduling
{
	public:
		int numNodes;			// Total number of nodes(jobs)
		int totalJobTimes;		// Total time of all jobs
		int procGiven;			// Given number of processors
		int** adjacencyMatrix;	// 2-D matrix recording the edges
		int** scheduleTable;	// The schedule table what should be worked on
		int* jobTimeAry;		// Recording job time for each job
		linkedList* OPEN;		// OPEN Queue
		int* processJob;		// Recording the current job the i th processor working on
		int* processTime;		// Recording the current job's remaining time
		int* parentCount;		// The number of parent nodes with node i
		int* kidCount;			// The number of kid nodes with node i
		int* jobDone;			// Mark for done job, 0 == not done, 1 == done
		int* jobMarked;			// Mark for graph G, 0 == job is not marked, 1 == job is marked 

		// Constructor
		Scheduling();

		// Methods
		void loadMatrix(string);			// Loading data from file
											// and initializing the size of all dyn-ary
		void computeTotalJobTimes(string);	// Finding total job time of all job
											// and update job time array
		int getUnMarkOrphen();				// Find the next orphen, just 1 orphen
		void printList();					// Printing the OPEN on console
		void printTable(string);			// Printing the schedule table to file
		int findProcessor();				// Finding the next available processor, return -1 if there's no one
		void updateTable(int, int, int);	// Updating talbe with parameters (Proc id, job id, time slice)
		int checkCycle();					// Checking if the graph has cycle, 1 == yes, 0 == no
		int findDoneJob(int);				// Finding if a job is done by processr i, return job id if there is
		void deleteNode(int);				// Deleting the done job(id = i) node
		void deleteEdge(int);				// Deleting the edges pointed out from done job(id = i) node
};

Scheduling :: Scheduling()
{
	this->numNodes = 0;
	this->totalJobTimes = 0;
	this->procGiven = 0;
	this->OPEN = new linkedList();
}

void Scheduling :: loadMatrix(string input_file_name)
{
	ifstream inFile;
	inFile.open(input_file_name);
	
	int numOfJobs;
	int thisJob;
	int nextJob;
	string word;

	inFile >> word;
	numOfJobs = atoi(word.c_str());	// Get the number of nodes
	this->numNodes = numOfJobs;		// Seg FAULT!!!!!!

	// Initializing every dynamic array in this class except schedule table(need totalJobTimes)
	this->adjacencyMatrix = new int*[numOfJobs + 1];
	for(int i = 0; i <= numOfJobs; i++)
		this->adjacencyMatrix[i] = new int[numOfJobs + 1];
	for(int i = 0; i <= numOfJobs; i++)
		for(int j = 0; j <= numOfJobs; j++)
			this->adjacencyMatrix[i][j] = 0;

	this->scheduleTable = new int*[numOfJobs + 1];	// Column need to be defined

	this->jobTimeAry = new int[numOfJobs + 1];
	for(int i = 0; i <= numOfJobs; i++)
		this->jobTimeAry[i] = 0;

	this->parentCount = new int[numOfJobs + 1];
	for(int i = 0; i <= numOfJobs; i++)
		this->parentCount[i] = 0;

	this->kidCount = new int[numOfJobs + 1];
	for(int i = 0; i <= numOfJobs; i++)
		this->kidCount[i] = 0;

	this->jobDone = new int[numOfJobs + 1];
	for(int i = 0; i <= numOfJobs; i++)
		this->jobDone[i] = 0;

	this->jobMarked = new int[numOfJobs + 1];
	for(int i = 0; i <= numOfJobs; i++)
		this->jobMarked[i] = 0;

	// Inputing the matrix
	int r;
	int c;

	while(inFile >> word)
	{
		r = atoi(word.c_str());
		inFile >> word;
		c = atoi(word.c_str());
		this->adjacencyMatrix[r][c] = 1;
	}

	// Testing code
	for(int i = 1; i <= numOfJobs; i++)
	{
		for(int j = 1; j <= numOfJobs; j++)
			cout << std::to_string(this->adjacencyMatrix[i][j]) << " ";
		cout << endl;
	}

	// Finding parentCount and kidCount
	int parent_id;
	int kid_id;
	for(parent_id = 1; parent_id <= numOfJobs; parent_id++)
	{
		for(int i = 1; i <= numOfJobs; i++)
			if(this->adjacencyMatrix[parent_id][i] == 1)
				this->kidCount[parent_id]++;
	}
	for(kid_id = 1; kid_id <= numOfJobs; kid_id++)
	{
		for(int i = 1; i <= numOfJobs; i++)
			if(this->adjacencyMatrix[i][kid_id] == 1)
				this->parentCount[kid_id]++;
	}

	// Testing code
	for(int i = 1; i <= numOfJobs; i++)
		cout << i << " has " << this->kidCount[i] << " kids." << endl;

	for(int i = 1; i <= numOfJobs; i++)
		cout << i << " has " << this->parentCount[i] << " parents." << endl;

	inFile.close();
	return;
}

void Scheduling :: computeTotalJobTimes(string input_file_name)
{
	ifstream inFile;
	inFile.open(input_file_name);

	int total = 0;
	string word;
	inFile >> word;
	int numOfJobs = atoi(word.c_str());
	if(numOfJobs != this->numNodes)
	{
		cout << "Number of Nodes does not match in two files. ERROR!" << endl;
		exit(0);
	}
	int j_id;
	int j_time;
	while(inFile >> word)
	{
		j_id = atoi(word.c_str());
		inFile >> word;
		j_time = atoi(word.c_str());
		this->jobTimeAry[j_id] = j_time;
		total += j_time;
	}

	this->totalJobTimes = total;

	for(int i = 0; i <= numOfJobs; i++)
		this->scheduleTable[i] = new int[total + 1];

	// Testing code
	for(int i = 1; i <= numOfJobs; i++)
		cout << i << " __ job time: " << this->jobTimeAry[i] << endl;

	inFile.close();
	return;
}

int Scheduling :: getUnMarkOrphen()
{
	for(int i = 1; i <= this->numNodes; i++)
	{
		if(this->jobMarked[i] == 0 && this->parentCount[i] == 0)
		{
			this->jobMarked[i] = 1;
			return i;
		}
	}
	return -1;
}

int Scheduling :: findProcessor()
{
	for(int i = 1; i <= this->procGiven; i++)
		if(this->processTime[i] == 0)
			return i;
	return -1;
}

void Scheduling :: updateTable(int availProc, int newJob, int currentTime)
{
	int j_time = this->jobTimeAry[newJob];
	int end = currentTime + j_time;
	for(int i = currentTime; i <= end; i)
		this->scheduleTable[availProc][i] = newJob;

	return;
}

int Scheduling :: checkCycle()
{
	bool OPEN_is_empty = false;
	bool Graph_is_not_empty = false;
	bool Proc_all_finished = true;

	if(this->OPEN->next == NULL)
		OPEN_is_empty = true;
	
	for(int i = 1; i <= this->numNodes; i++)
		if(this->jobMarked[i] == 0)
			Graph_is_not_empty = true;

	for(int i = 1; i <= this->procGiven; i++)
		if(this->processJob[i] != 0)
			Proc_all_finished = false;

	
}




int main(int argc, char ** argv)
{
	Scheduling* s = new Scheduling();
	s->loadMatrix(argv[1]);
	s->computeTotalJobTimes(argv[2]);
	s->OPEN->printList();

	return 0;
}
