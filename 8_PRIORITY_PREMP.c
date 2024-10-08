#include <stdio.h>
#include <stdbool.h>

// Structure to represent process details
struct process_struct
{
    int at;                          // Arrival Time
    int bt;                          // Burst Time
    int priority;                    // Priority of the process
    int ct, wt, tat, rt, start_time; // Completion Time, Waiting Time, Turnaround Time, Response Time, Start Time
} ps[100];

int main()
{
    int n;                                     // Total number of processes
    bool is_completed[100] = {false};          // Boolean array to check if a process is completed
    int bt_remaining[100];                     // Array to store remaining burst time for each process
    int current_time = 0;                      // Current time in the scheduling process
    int completed = 0;                         // Number of completed processes
    float sum_tat = 0, sum_wt = 0, sum_rt = 0; // Variables to calculate total Turnaround, Waiting, and Response times
    int total_idle_time = 0, prev = 0;         // Variables to calculate idle time

    // Input: Number of processes
    printf("\nENter the number of processes:");
    scanf("%d", &n);

    // Input: Process Arrival Times
    printf("\nENter the arrival time of processes:");

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &ps[i].at);
    }

    // Input: Process Burst Times
    printf("\nENter the burst of processes:");

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &ps[i].bt);
        bt_remaining[i] = ps[i].bt; // Initialize remaining burst time for each process
    }

    // Input: Process Priorities
    printf("\nENter the priorities of processes:");

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &ps[i].priority);
    }

    // Scheduling process
    while (completed != n)
    {
        int max_index = -1; // Index of the process with the highest priority
        int maximum = -1;   // Variable to track the maximum priority value

        // Finding the process with the highest priority in the ready queue
        for (int i = 0; i < n; i++)
        {
            if (ps[i].at <= current_time && is_completed[i] == false) // Process has arrived and is not completed
            {
                if (ps[i].priority > maximum) // Check if the process has a higher priority
                {
                    maximum = ps[i].priority;
                    max_index = i;
                }
                else if (ps[i].priority == maximum) // If priorities are the same, choose the process with earlier arrival
                {
                    if (ps[i].at < ps[max_index].at)
                    {
                        max_index = i;
                    }
                }
            }
        }

        if (max_index == -1) // If no process is found in the ready queue, increment current time
        {
            current_time++;
        }
        else
        {
            // If it is the first time the process is executing, record the start time
            if (bt_remaining[max_index] == ps[max_index].bt)
            {
                ps[max_index].start_time = current_time;
                if (completed > 0)
                    total_idle_time += (ps[max_index].start_time - prev); // Calculate idle time
            }

            // Process executes for 1 unit of time (preemptive)
            bt_remaining[max_index]--;
            current_time++;

            // If the process finishes execution
            if (bt_remaining[max_index] == 0)
            {
                ps[max_index].ct = current_time;                                // Set Completion Time
                ps[max_index].tat = ps[max_index].ct - ps[max_index].at;        // Calculate Turnaround Time
                ps[max_index].wt = ps[max_index].tat - ps[max_index].bt;        // Calculate Waiting Time
                ps[max_index].rt = ps[max_index].start_time - ps[max_index].at; // Calculate Response Time

                // Add times to the total sums for averaging later
                sum_tat += ps[max_index].tat;
                sum_wt += ps[max_index].wt;
                sum_rt += ps[max_index].rt;

                completed++;                    // Increment number of completed processes
                is_completed[max_index] = true; // Mark the process as completed

                prev = current_time; // Update previous time
            }
        }
    }

    // Calculate length of the process completion cycle
    int max_completion_time = ps[0].ct, min_arrival_time = ps[0].at;
    for (int i = 1; i < n; i++)
    {
        if (ps[i].ct > max_completion_time)
            max_completion_time = ps[i].ct;
        if (ps[i].at < min_arrival_time)
            min_arrival_time = ps[i].at;
    }
    int length_cycle = max_completion_time - min_arrival_time;

    // Calculate throughput and CPU utilization
    float throughput = (float)n / length_cycle;                                           // Throughput: number of processes completed per unit time
    float cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle * 100; // CPU utilization

    // Output: Table showing Start Time (ST), Completion Time (CT), Turnaround Time (TAT), Waiting Time (WT), Response Time (RT)
    printf("\nProcess\tAT\tBT\tST\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               i + 1, ps[i].at, ps[i].bt, ps[i].start_time, ps[i].ct,
               ps[i].tat, ps[i].wt, ps[i].rt);
    }

    // Output: Average Turnaround Time, Waiting Time, Response Time, Throughput, and CPU Utilization
    printf("\nAverage Turnaround Time: %.2f", sum_tat / n);
    printf("\nAverage Waiting Time: %.2f", sum_wt / n);
    printf("\nAverage Response Time: %.2f", sum_rt / n);
    printf("\nThroughput: %.2f", throughput);
    printf("\nCPU Utilization: %.2f%%\n", cpu_utilization);

    return 0;
}
