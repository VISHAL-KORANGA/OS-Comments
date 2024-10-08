#include <stdio.h>

// Define a structure for Process to store its details
struct Process
{
    int id;             // Process ID
    int arrivalTime;    // Arrival time of the process
    int burstTime;      // Burst time of the process
    int remainingTime;  // Remaining time to execute the process
    int completionTime; // Completion time when the process finishes
    int waitingTime;    // Waiting time for the process
    int turnAroundTime; // Turnaround time for the process
};

// Function to execute SRTF scheduling algorithm
void srtfScheduling(struct Process p[], int n)
{
    int totalTime = 0, completed = 0, shortest = 0;
    int minRemainingTime, isProcessSelected = 0, endTime;
    float avgWaitingTime = 0, avgTurnAroundTime = 0;

    // Loop until all processes are completed
    while (completed != n)
    {
        minRemainingTime = 10000; // Minimum remaining time to find the shortest process

        // Traverse all processes to find the process with shortest remaining time
        for (int i = 0; i < n; i++)
        {
            // Check if the process has arrived and its remaining time is less than the current minimum
            if (p[i].arrivalTime <= totalTime && p[i].remainingTime > 0 && p[i].remainingTime < minRemainingTime)
            {
                minRemainingTime = p[i].remainingTime;
                shortest = i;
                isProcessSelected = 1; // A process has been selected
            }
        }

        // If no process is found, move to the next unit of time
        if (isProcessSelected == 0)
        {
            totalTime++;
            continue;
        }

        // Reduce the remaining time of the selected process by 1 unit
        p[shortest].remainingTime--;

        // If the process is completed
        if (p[shortest].remainingTime == 0)
        {
            completed++;                          // Increment the count of completed processes
            endTime = totalTime + 1;              // The end time of the process
            p[shortest].completionTime = endTime; // Store the completion time

            // Calculate turnaround time and waiting time
            p[shortest].turnAroundTime = p[shortest].completionTime - p[shortest].arrivalTime;
            p[shortest].waitingTime = p[shortest].turnAroundTime - p[shortest].burstTime;

            // Add to total waiting and turnaround time
            avgWaitingTime += p[shortest].waitingTime;
            avgTurnAroundTime += p[shortest].turnAroundTime;
        }

        totalTime++;           // Increment the current time
        isProcessSelected = 0; // Reset the selection flag
    }

    // Print the process details after completion
    printf("\nProcess\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\n", p[i].id, p[i].arrivalTime, p[i].burstTime, p[i].completionTime, p[i].turnAroundTime, p[i].waitingTime);
    }

    // Calculate and print average waiting and turnaround times
    avgWaitingTime /= n;
    avgTurnAroundTime /= n;
    printf("\nAverage Waiting Time: %.2f", avgWaitingTime);
    printf("\nAverage Turnaround Time: %.2f", avgTurnAroundTime);
}

int main()
{
    int n;

    // Ask the user for the number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Create an array of Process structures
    struct Process p[n];

    // Input for Arrival and Burst times for each process
    for (int i = 0; i < n; i++)
    {
        p[i].id = i + 1; // Assigning process ID (starting from 1)
        printf("Enter arrival time for process %d: ", p[i].id);
        scanf("%d", &p[i].arrivalTime);
        printf("Enter burst time for process %d: ", p[i].id);
        scanf("%d", &p[i].burstTime);
        p[i].remainingTime = p[i].burstTime; // Initially, remaining time is the burst time
    }

    // Execute the SRTF scheduling algorithm
    srtfScheduling(p, n);

    return 0;
}
