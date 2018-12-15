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
    int burstTime = 0;
    bool is_finish = false;
};

// sort by burstTime
bool compareBurstTime(const Process &a, const Process &b) {
    return a.burstTime < b.burstTime;
}

// sort by arriveTime
bool compareArriveTime(const Process &a, const Process &b) {
    return a.arriveTime <= b.arriveTime;
}

int main(int argc, char** argv) {
    ifstream inputFile(argv[1]);
    int numProcess;
    inputFile >> numProcess;
    vector <Process> processList(numProcess);
    for (int i = 0; i < numProcess; ++i) {
        processList[i].id = i;
        inputFile >> processList[i].arriveTime;
    }
    for (int i = 0; i < numProcess; ++i) {
        inputFile >> processList[i].burstTime;
    }
    inputFile.close();
    sort(processList.begin(), processList.end(), compareArriveTime);
    // count waiting and turnaround time
    int processCount = 0;
    int timeCount = 0;
    int tmp_index = 0;
    vector <Process> tmpQueue = processList;
    while (processCount != numProcess) {
        int index = -1;
        sort(tmpQueue.begin(), tmpQueue.end(), compareBurstTime);
        for (int i = 0; i < numProcess; ++i) {
            int tt_index = tmpQueue[i].id;
            if (tmpQueue[i].arriveTime <= timeCount && !tmpQueue[i].is_finish) {
                for (int j = 0; j < numProcess; j++) {
                    if (processList[j].id == tt_index) {
                        index = j;
                        break;
                    }
                }
                tmp_index = i;
                tmpQueue[tmp_index].burstTime -= 1;
                processList[index].waitTime += timeCount - tmpQueue[tmp_index].arriveTime;
                break;
            }
        }

        // update time now
        timeCount += 1;
        if (index != -1) {
            tmpQueue[tmp_index].arriveTime = timeCount;
            if (tmpQueue[tmp_index].burstTime == 0) {
                processList[index].turnAroundTime = timeCount - processList[index].arriveTime;
                tmpQueue[tmp_index].is_finish = true;
                processCount += 1;
            }
        }
    }
    // print out waiting and turnaround time
    int waitTime_total = 0;
    int turnAroundTime_total = 0;
    ofstream outputFile("ans2.txt");
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
