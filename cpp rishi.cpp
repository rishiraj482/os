#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

class SRTF {
public:
    int i;
    vector<int> remainingTime;

    void Scheduling(int p, vector<int> burstTime, vector<int> arrivalTime) {
        vector<string> GanttChart;
        vector<int> GanttTime;

        remainingTime = burstTime;
        vector<int> completionTime(p), turnAroundTime(p), waitingTime(p), responseTime(p, -1);
        int completed = 0, time = 0, shortest = 0, minTime;
        bool found = false;
        int contextSwitches = 0;
        GanttTime.push_back(0);

        while (completed != p) {
            minTime = numeric_limits<int>::max();
            found = false;

            for (i = 0; i < p; i++) {
                if ((arrivalTime[i] <= time) && (remainingTime[i] < minTime) && remainingTime[i] > 0) {
                    minTime = remainingTime[i];
                    shortest = i;
                    found = true;
                }
            }

            if (!found) {
                time++;
                continue;
            }

            if (responseTime[shortest] == -1) {
                responseTime[shortest] = time - arrivalTime[shortest];
            }

            remainingTime[shortest]--;
            time++;

            if (GanttChart.empty() || GanttChart.back() != "P" + to_string(shortest + 1)) {
                if (!GanttChart.empty()) {
                    contextSwitches++;
                }
                GanttChart.push_back("P" + to_string(shortest + 1));
                GanttTime.push_back(time);
            } else {
                GanttTime.back() = time;
            }

            if (remainingTime[shortest] == 0) {
                completed++;
                completionTime[shortest] = time;
                turnAroundTime[shortest] = completionTime[shortest] - arrivalTime[shortest];
                waitingTime[shortest] = turnAroundTime[shortest] - burstTime[shortest];
            }
        }

        double totalBurstTime = 0, totalWaitingTime = 0, totalTurnAroundTime = 0, totalResponseTime = 0;
        for (i = 0; i < p; i++) {
            totalBurstTime += burstTime[i];
            totalWaitingTime += waitingTime[i];
            totalTurnAroundTime += turnAroundTime[i];
            totalResponseTime += responseTime[i];
        }
        double avgBurstTime = totalBurstTime / p;
        double avgWaitingTime = totalWaitingTime / p;
        double avgTurnAroundTime = totalTurnAroundTime / p;
        double avgResponseTime = totalResponseTime / p;

        // Enhanced design for the Gantt Chart and statistics
        cout << "\nGantt Chart:\n";
        for (const auto& process : GanttChart) {
            cout << process << " ";
        }
        cout << "\n";
        for (const auto& time : GanttTime) {
            cout << time << "   ";
        }
        cout << "\n";

        cout << "\nSummary of CPU Scheduling:\n";
        cout << "-----------------------------------\n";
        cout << fixed << setprecision(2);
        cout << "Average CPU Burst Time   : " << avgBurstTime << " units\n";
        cout << "Average Waiting Time     : " << avgWaitingTime << " units\n";
        cout << "Average Turn Around Time : " << avgTurnAroundTime << " units\n";
        cout << "Average Response Time    : " << avgResponseTime << " units\n";
        cout << "Total Context Switches   : " << contextSwitches << "\n";
        cout << "-----------------------------------\n";
    }
};

int main() {
    int p, i;
    cout << "-----------------------------------\n";
    cout << "          SRTF Scheduler\n";
    cout << "-----------------------------------\n";
    cout << "Enter the number of processes: ";
    cin >> p;
    vector<int> arrivalTime(p);
    vector<int> burstTime(p);

    cout << "\nEnter the CPU Burst Time for each process:\n";
    cout << "-----------------------------------\n";
    for (i = 0; i < p; i++) {
        cout << "Process " << i + 1 << ": ";
        cin >> burstTime[i];
    }

    cout << "\nEnter the Arrival Time for each process:\n";
    cout << "-----------------------------------\n";
    for (i = 0; i < p; i++) {
        cout << "Process " << i + 1 << ": ";
        cin >> arrivalTime[i];
    }

    SRTF s;
    s.Scheduling(p, burstTime, arrivalTime);
    return 0;
}