#include <stdio.h>   // Standard input-output library
#include <stdbool.h> // For using boolean data type
#include <limits.h>  // For using INT_MIN and INT_MAX

// Structure to represent process details
struct process_struct
{
    int at;                          // Arrival Time
    int bt;                          // Burst Time
    int priority;                    // Priority of the process
    int ct, wt, tat, rt, start_time; // Completion Time, Waiting Time, Turnaround Time, Response Time, Start Time
} ps[100];                           // Array of processes

// Function to find the maximum of two values
int findmax(int a, int b)
{
    return a > b ? a : b;
}

// Function to find the minimum of two values
int findmin(int a, int b)
{
    return a < b ? a : b;
}

int main()
{
    int n;                                                     // Total number of processes
    bool is_completed[100] = {false}, is_first_process = true; // Boolean array to track completed processes
    int current_time = 0;                                      // Current time in the scheduling process
    int completed = 0;                                         // Number of completed processes
    int total_idle_time = 0, prev = 0, length_cycle;           // Variables to calculate idle time and process cycle length
    float cpu_utilization;                                     // CPU utilization calculation
    int max_completion_time, min_arrival_time;                 // Variables to track max completion and min arrival times

    // Taking input for the number of processes
    printf("\nEnter the number of processes:");
    scanf("%d", &n);

    float sum_tat = 0, sum_wt = 0, sum_rt = 0; // Variables for summing up total times (Turnaround, Waiting, Response)
    int i;

    // Taking input for process arrival time
    printf("\nEnter Process Arrival Time\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &ps[i].at);
    }

    // Taking input for process burst time
    printf("\nEnter Process Burst Time\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &ps[i].bt);
    }

    // Taking input for process priority
    printf("\nEnter Priority\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &ps[i].priority);
    }

    // Processing loop until all processes are completed
    while (completed != n)
    {
        // Find process with maximum priority in the ready queue at current time
        int max_index = -1;
        int maximum = -1; // Using -1 to ensure any valid priority is greater

        for (int i = 0; i < n; i++)
        {
            if (ps[i].at <= current_time && is_completed[i] == false) // Check if process has arrived and is not completed
            {
                if (ps[i].priority > maximum)
                {
                    maximum = ps[i].priority;
                    max_index = i;
                }
                else if (ps[i].priority == maximum)
                {
                    if (ps[i].at < ps[max_index].at) // Tie-breaking based on arrival time
                    {
                        max_index = i;
                    }
                }
            }
        }

        if (max_index == -1) // No process is available in the ready queue
        {
            current_time++;
        }
        else
        {
            // Assign start time and calculate the completion, turnaround, waiting, and response times
            ps[max_index].start_time = current_time;
            ps[max_index].ct = ps[max_index].start_time + ps[max_index].bt; // Completion time
            ps[max_index].tat = ps[max_index].ct - ps[max_index].at;        // Turnaround time
            ps[max_index].wt = ps[max_index].tat - ps[max_index].bt;        // Waiting time
            ps[max_index].rt = ps[max_index].start_time - ps[max_index].at; // Response time

            // Idle time calculation for first process or subsequent processes
            total_idle_time += (is_first_process == true) ? 0 : (ps[max_index].start_time - prev);

            // Sum up the total times for averaging later
            sum_tat += ps[max_index].tat;
            sum_wt += ps[max_index].wt;
            sum_rt += ps[max_index].rt;

            completed++;                     // Increment the number of completed processes
            is_completed[max_index] = true;  // Mark the process as completed
            current_time = ps[max_index].ct; // Move current time to process's completion time
            prev = current_time;             // Update the previous process completion time
            is_first_process = false;        // First process has been scheduled, set flag to false
        }
    }

    // Calculate the length of the process completion cycle
    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;

    for (int i = 0; i < n; i++)
    {
        max_completion_time = findmax(max_completion_time, ps[i].ct);
        min_arrival_time = findmin(min_arrival_time, ps[i].at);
    }

    length_cycle = max_completion_time - min_arrival_time;

    // Calculate CPU utilization
    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle;

    // Printing table header
    printf("\nProcess\tAT\tBT\tST\tCT\tTAT\tWT\tRT\n");

    // Loop through each process to print details in tabular form
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               i + 1, ps[i].at, ps[i].bt, ps[i].start_time, ps[i].ct,
               ps[i].tat, ps[i].wt, ps[i].rt);
    }

    // Output the average turnaround time, waiting time, response time, throughput, and CPU utilization
    printf("\nAverage Turnaround Time: %.2f\n", sum_tat / n);
    printf("Average Waiting Time: %.2f\n", sum_wt / n);
    printf("Average Response Time: %.2f\n", sum_rt / n);
    printf("Throughput: %.2f\n", (float)n / length_cycle);
    printf("CPU Utilization: %.2f%%\n", cpu_utilization * 100);

    return 0;
}
