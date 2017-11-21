#include <iostream>
#include <stdlib.h> 
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;

class ActivityMonitor {
  vector<int> processes;
public:
  ActivityMonitor(vector<int> processes);

  //Functions for finding maximum one unfinished process
  //I thought of two ways of implementing the solution to this problem and decided I would
  //implement both and compare the solutions.
  int findUnfinishedProcessSlow();
  //This solution finds the unifinished process by sorting the processes vector and checking
  //pairs of values to see if they are equal. If a number is not equal to the number next to it
  //then it is an unfinished process. This solution has a worse case complexity of O(nlogn), but it 
  //does not need any extra space.

  int findUnfinishedProcessFast();
  //This solution takes advantage of an unordered_set, removing the need to sort. At each value in the 
  //processes vector, the value is looked up in the unordered_set. If it already exists, then the value
  //is removed from the set. If not, then it is added to the set. The final remaining element in the set
  //is the processes that did not finish. This solution has a worse case complexity of O(n), but needs access
  //to extra space.

  //The fast solution might seem like the better one to use for a problem like this, however, I would argue 
  //the slow solution would be better in the context of the given problem. The number of processes running on
  //a computer is often small (possibly in the magnitude of only a few hundred). Thus, the time it will take for the slow
  //and fast implementations to complete will be similar (since n will be small). This means that it is probably
  //a better idea to go with the solution that uses less space, especially since space is a very valuable resource when
  //working on an operating system.


  //These two functions work for any number of unfinished processes
  vector<int> findUnfinishedProcessesSlow();
  vector<int> findUnfinishedProcessesFast();

  //Return number of processes that were started
  //Works for any number of processes left unfinished
  int numProcessesStarted();
};

ActivityMonitor::ActivityMonitor(vector<int> processes) {
  this->processes = processes;
}

int ActivityMonitor::findUnfinishedProcessSlow() {
  //Sort vector
  sort(processes.begin(), processes.end(), [](int i, int j) { return i < j; });

  //Loop through processes vector
  for (int i = 0; i < processes.size(); i += 2) {
    //Check if the process finished
    if (i + 1 >= processes.size() || processes[i] != processes[i + 1]) {
      return processes[i];
    }
  }

  //If there was no return in the for loop then all the processes finished
  return 0;
}

int ActivityMonitor::findUnfinishedProcessFast() {
  unordered_set<int> unfinishedProcesses;

  //Loop through processes vector
  for (int i = 0; i < processes.size(); i++) {
    //If the process already exists in unfinishedProcesses, then remove it from the set
    //else, add the process to the set
    if (unfinishedProcesses.find(processes[i]) != unfinishedProcesses.end()) {
      unfinishedProcesses.erase(unfinishedProcesses.find(processes[i]));
    } else {
      unfinishedProcesses.insert(processes[i]);
    }
  }

  //Check the size of unfinishedProcesses to determine the return value
  if (unfinishedProcesses.size() == 0) {
    return 0;
  } else {
    return *unfinishedProcesses.begin();
  }
}

vector<int> ActivityMonitor::findUnfinishedProcessesSlow() {
  vector<int> unfinishedProcesses;
  //Sort vector
  sort(processes.begin(), processes.end(), [](int i, int j) { return i < j; });
  
  int index = 0;
  //Loop through vector
  while (index < processes.size()) {
    //If the process is unfinished, then add it to the unfinishedProcesses vector and check the next index
    //else, skip the next index
    if (index + 1 >= processes.size() || processes[index] != processes[index + 1]) {
      unfinishedProcesses.push_back(processes[index]);
      index++;
    } else {
      index += 2;
    }
  } 
  
  return unfinishedProcesses;
}

vector<int> ActivityMonitor::findUnfinishedProcessesFast() {
  unordered_set<int> unfinishedProcesses;
  
  //Loop through processes vector
  for (int i = 0; i < processes.size(); i++) {
    //If the process already exists in unfinishedProcesses, then remove it from the set
    //else, add the process to the set
    if (unfinishedProcesses.find(processes[i]) != unfinishedProcesses.end()) {
      unfinishedProcesses.erase(unfinishedProcesses.find(processes[i]));
    } else {
      unfinishedProcesses.insert(processes[i]);
    }
  }

  //Copy unordered_set to vector
  vector<int> unfinished;
  copy(unfinishedProcesses.begin(), unfinishedProcesses.end(), back_inserter(unfinished));

  return unfinished;
}

int ActivityMonitor::numProcessesStarted() {
  //Sort vector
  sort(processes.begin(), processes.end(), [](int i, int j) { return i < j; });

  int numProcesses = 0;  
  int index = 0;
  //Loop through processes vector
  while (index  < processes.size()) {
    //If process was not finished then look at the next index, otherwise skip the next index
    if (index + 1 >= processes.size() || processes[index] != processes[index + 1]) {
      index++;
    } else {
      index += 2;
    }
    numProcesses++;    
  }

  return numProcesses;
}


//Read the inputs from stdin and make sure they are valid
//Store the inputs in the processes vector
void readInputs(vector<int> &processes) {
  string line = "";
  int processID = 0;
  while (cin >> line) {
    try {
      if (!line.empty())
      {
        processID = stoi(line);
        processes.push_back(processID);
      }
    } catch (const invalid_argument &e) {
      cout << "Invalid argument: " + line << endl;
      exit(0);
    }
  }
}



int main() {
  vector<int> processes;
  readInputs(processes);

  ActivityMonitor activityMonitor(processes);
  cout << activityMonitor.findUnfinishedProcessSlow() << endl;
}