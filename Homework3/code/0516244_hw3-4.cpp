#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process{
    int id = 0;
    int waitTime = 0;
    int timeRemaining = 0;
    int turnAroundTime = 0;
    int arriveTime = 0;
    int initialArriveTime = 0;
    int burstTime = 0;
    int proceedTime = 0;
};

// sort by burstTime
bool compareBurstTime(const Process &a, const Process &b) {
    return a.burstTime < b.burstTime;
}

// sort by arriveTime
bool compareArriveTime(const Process &a, const Process &b) {
    return a.arriveTime <= b.arriveTime;
}

void roundRobin(Process & processInput, int & timeCount, int & processCount, int timeQuantum, int level) {
    processInput.waitTime += timeCount - processInput.arriveTime - 1;
    if (processInput.timeRemaining == 0)
        processInput.timeRemaining = timeQuantum;
    // special handle for waitTime
    processInput.arriveTime = timeCount;
    processInput.timeRemaining -= 1;
    processInput.burstTime -= 1;
    if(processInput.burstTime == 0) { // finish
        processInput.proceedTime = 2;
        processInput.turnAroundTime = timeCount - processInput.initialArriveTime + 1;
        processCount += 1;
        processInput.arriveTime = timeCount;
        processInput.proceedTime = 3;
    }
    else if (processInput.timeRemaining == 0) {
        processInput.proceedTime = level;
        processInput.arriveTime = timeCount;
    }
}

int main(int argc, char** argv) {
    ifstream inputFile(argv[1]);
    int numProcess;
    inputFile >> numProcess;
    vector <Process> processList(numProcess);
    for (int i = 0; i < numProcess; ++i) {
        processList[i].id = i;
        inputFile >> processList[i].arriveTime;
        processList[i].initialArriveTime = processList[i].arriveTime;
    }
    for (int i = 0; i < numProcess; ++i) {
        inputFile >> processList[i].burstTime;
    }
    int t1, t2;
    inputFile >> t1;
    inputFile >> t2;
    inputFile.close();
    sort(processList.begin(), processList.end(), compareArriveTime);
    int processCount = 0;
    int timeCount = 0;
    while (processCount != numProcess) {
        bool isSelect = false;
        // high-level queue
        for (int i = 0; i < numProcess; ++i) {
            if (processList[i].arriveTime <= timeCount && processList[i].proceedTime == 0) { // check at first queue
                roundRobin(processList[i], timeCount, processCount, t1, 1);
                isSelect = true;
                break;
            }
        }
        if(isSelect) {
            timeCount += 1;
            continue;
        }
        // medium-level queue
        for (int i = 0; i < numProcess; ++i) {
            if (processList[i].arriveTime <= timeCount && processList[i].proceedTime == 1) { // check if proceed
                roundRobin(processList[i], timeCount, processCount, t2, 2);
                isSelect = true;
                break;
            }
        }
        if(isSelect) {
            timeCount += 1;
            continue;
        }
        // low-level queue
        vector<Process> tmpQueue = processList;
        sort(tmpQueue.begin(), tmpQueue.end(), compareBurstTime);
        int selectedId = 0;
        for (int i = 0; i < numProcess; ++i) {
            if (tmpQueue[i].timeRemaining > 0 && tmpQueue[i].proceedTime == 2) {
                int tt_index = tmpQueue[i].id;
                for (int j = 0; j < numProcess; j++) {
                    if (processList[j].id == tt_index) {
                        selectedId = j;
                        break;
                    }
                }
                roundRobin(processList[selectedId], timeCount, processCount, tmpQueue[i].burstTime, 1);
                isSelect = true;
                break;
            }
        }
        if(isSelect) {
            timeCount += 1;
            continue;
        }
        for (int i = 0; i < numProcess; ++i) {
            if (tmpQueue[i].arriveTime <= timeCount && tmpQueue[i].proceedTime == 2) { // check if proceed
                int tt_index = tmpQueue[i].id;
                for (int j = 0; j < numProcess; j++) {
                    if (processList[j].id == tt_index) {
                        selectedId = j;
                        break;
                    }
                }
                roundRobin(processList[selectedId], timeCount, processCount, tmpQueue[i].burstTime, 1);
                break;
            }
        }
        timeCount += 1;
    }
    // print out waiting and turnaround time
    int waitTime_total = 0;
    int turnAroundTime_total = 0;
    ofstream outputFile("ans4.txt");
    for (int i = 0; i < numProcess; ++i) {
        for (int j = 0; j < numProcess; ++j) {
            if (i == processList[j].id) {
                processList[j].waitTime += 1;
                waitTime_total += processList[j].waitTime;
                turnAroundTime_total += processList[j].turnAroundTime;
                outputFile << processList[j].waitTime << " " << processList[j].turnAroundTime << endl;
                break;
            }
        }
    }
    outputFile << setprecision(7) << ((double)waitTime_total / numProcess) << endl;
    outputFile << setprecision(7) << ((double)turnAroundTime_total / numProcess) << endl;
    outputFile.close();
    return 0;
}
