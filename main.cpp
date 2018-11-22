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
		void insertSort(qNode *);
		qNode * removal();
};

linkedList :: linkedList()
{
	this->head = new qNode();
}

void linkedList :: insertSort(qNode *newNode)
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


class Scheduling
{
	public:
		int numNodes;			// Total number of nodes(jobs)
		int totalJobTimes;		// Total time of all jobs
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
		int computeTotalJobTimes(string);	// Finding total job time of all job
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
	this->numNodes = numOfJobs;

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

	



	inFile.close();
	return;
}









int main(int argc, char ** argv)
{

	return 0;
}
