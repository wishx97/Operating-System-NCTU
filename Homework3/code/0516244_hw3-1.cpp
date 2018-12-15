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
    vector <Process> tmpQueue;
    while (processCount != numProcess) {
        int index = 0;
        if (!tmpQueue.empty()) {
            int tmp_index = tmpQueue[0].id;
            for (int i = 0; i < numProcess; i++) {
                if (processList[i].id == tmp_index) {
                    index = i;
                    break;
                }
            }
            // check time now and count wait time
            processList[index].waitTime = timeCount - processList[index].arriveTime;
            // count turnaround time
            processList[index].turnAroundTime = timeCount + processList[index].burstTime - processList[index].arriveTime;
            // update time now
            timeCount += processList[index].burstTime;
            processList[index].is_finish = true;
            processCount += 1;
        }
        tmpQueue.clear();
        bool allFinish = true;
        for (int i = 0; i < numProcess; ++i) {
            if (!processList[i].is_finish)
                allFinish = false;
            if (processList[i].arriveTime <= timeCount && !processList[i].is_finish) {
                tmpQueue.push_back(processList[i]);
            }
        }
        // if some arrive time is far than other
        if (!allFinish && tmpQueue.empty()) {
            for (int i = 0; i < numProcess; ++i) {
                if (!processList[i].is_finish) {
                    timeCount = processList[i].arriveTime;
                    tmpQueue.push_back(processList[i]);
                }
            }
        }
        // sort by shortest burst time
        sort(tmpQueue.begin(), tmpQueue.end(), compareBurstTime);
    }
    // print out waiting and turnaround time
    int waitTime_total = 0;
    int turnAroundTime_total = 0;
    ofstream outputFile("ans1.txt");
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
