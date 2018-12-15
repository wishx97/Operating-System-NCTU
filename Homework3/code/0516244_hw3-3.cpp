#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process{
    int id = 0;
    int waitTime = 0;
    int turnAroundTime = 0;
    int arriveTime = 0;
    int initialArriveTime = 0;
    int burstTime = 0;
    int proceedTime = 0;
    bool is_finish = false;
};

// sort by arriveTime
bool compareArriveTime(const Process &a, const Process &b) {
    return (a.arriveTime < b.arriveTime) || ((a.arriveTime == b.arriveTime) && (a.proceedTime < b.proceedTime));
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
    int timeQuantum;
    inputFile >> timeQuantum;
    inputFile.close();
    sort(processList.begin(), processList.end(), compareArriveTime);
    // count waiting and turnaround time
    int processCount = 0;
    int timeCount = 0;
    int tmpIndex = 0;
    vector <Process> tmpList = processList;
    while (processCount != numProcess) {
        sort(tmpList.begin(), tmpList.end(), compareArriveTime);
        int tt_index;
        int selectedId = -1;
        for (int i = 0; i < numProcess; ++i) {
            if (!tmpList[i].is_finish) {
                tt_index = tmpList[i].id;
                for (int j = 0; j < numProcess; j++) {
                    if (processList[j].id == tt_index) {
                        selectedId = j;
                        break;
                    }
                }
                tmpIndex = i;
                break;
            }
        }
        if (selectedId == -1) {
            timeCount = tmpList[0].arriveTime;
            selectedId = tmpList[0].id;
            tmpIndex = 0;
        }
        tmpList[tmpIndex].proceedTime += 1;
        processList[selectedId].waitTime += timeCount - tmpList[tmpIndex].arriveTime;
        if (processList[selectedId].burstTime <= timeQuantum) {
            timeCount += processList[selectedId].burstTime;
            processList[selectedId].turnAroundTime = timeCount - processList[selectedId].initialArriveTime;
            tmpList[tmpIndex].is_finish = true;
            processCount += 1;
        }
        else {
            timeCount += timeQuantum;
            processList[selectedId].burstTime -= timeQuantum;
            tmpList[tmpIndex].arriveTime = timeCount;
        }
    }
    // print out waiting and turnaround time
    int waitTime_total = 0;
    int turnAroundTime_total = 0;
    ofstream outputFile("ans3.txt");
    for (int i = 0; i < numProcess; ++i) {
        for (int j = 0; j < numProcess; ++j) {
            if (i == processList[j].id) {
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
