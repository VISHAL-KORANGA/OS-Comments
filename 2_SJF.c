#include <stdio.h>   // Standard I/O for printf and scanf
#include <stdbool.h> // To use bool, true/false
#include <limits.h>  // To use INT_MAX and INT_MIN for comparisons

// Structure to store details of each process
struct process_struct
{
    int pid;                         // Process ID
    int at;                          // Arrival Time
    int bt;                          // Burst Time
    int ct, wt, tat, rt, start_time; // ct: Completion Time, wt: Waiting Time, tat: Turnaround Time, rt: Response Time, start_time: When process starts execution
} ps[100];                           // Array to hold up to 100 processes

// Function to find the maximum of two integers (used for max completion time)
int findmax(int a, int b)
{
    return a > b ? a : b; // If a is greater than b, return a; otherwise, return b
}

// Function to find the minimum of two integers (used for min arrival time)
int findmin(int a, int b)
{
    return a < b ? a : b; // If a is less than b, return a; otherwise, return b
}

int main()
{
    int n;                                                     // Number of processes
    bool is_completed[100] = {false}, is_first_process = true; // Array to track completed processes, flag for first process
    int current_time = 0;                                      // Current time in the system
    int completed = 0;                                         // Track the number of processes completed
    printf("Enter total number of processes: ");
    scanf("%d", &n); // Input number of processes

    int sum_tat = 0, sum_wt = 0, sum_rt = 0, total_idle_time = 0, prev = 0, length_cycle; // Sum of turnaround, waiting, response time, idle time
    float cpu_utilization;                                                                // To calculate CPU utilization
    int max_completion_time, min_arrival_time;                                            // Variables to find max completion time and min arrival time

    // Input arrival time for each process
    for (int i = 0; i < n; i++)
    {
        printf("\nEnter Process %d Arrival Time: ", i);
        scanf("%d", &ps[i].at); // Arrival time of process i
        ps[i].pid = i;          // Assign process ID as i
    }

    // Input burst time for each process
    for (int i = 0; i < n; i++)
    {
        printf("\nEnter Process %d Burst Time: ", i);
        scanf("%d", &ps[i].bt); // Burst time of process i
    }

    // Main scheduling loop (SJF non-preemptive)
    while (completed != n) // Repeat until all processes are completed
    {
        // Find process with minimum burst time in the ready queue at current time
        int min_index = -1;    // Index of process with min burst time
        int minimum = INT_MAX; // Set minimum burst time as a very large number initially

        for (int i = 0; i < n; i++)
        {
            if (ps[i].at <= current_time && is_completed[i] == false) // Check if process has arrived and is not completed
            {
                if (ps[i].bt < minimum) // Check if this process has the shortest burst time
                {
                    minimum = ps[i].bt;
                    min_index = i; // Store index of process with min burst time
                }
                if (ps[i].bt == minimum) // If burst times are equal, choose the one with the earlier arrival time
                {
                    if (ps[i].at < ps[min_index].at)
                    {
                        minimum = ps[i].bt;
                        min_index = i;
                    }
                }
            }
        }

        // If no process is ready, increment time (idle time)
        if (min_index == -1)
        {
            current_time++;
        }
        else
        {
            // Process the selected process
            ps[min_index].start_time = current_time;                        // Record start time
            ps[min_index].ct = ps[min_index].start_time + ps[min_index].bt; // Calculate completion time
            ps[min_index].tat = ps[min_index].ct - ps[min_index].at;        // Turnaround time = completion time - arrival time
            ps[min_index].wt = ps[min_index].tat - ps[min_index].bt;        // Waiting time = turnaround time - burst time
            ps[min_index].rt = ps[min_index].wt;                            // Response time = waiting time (in non-preemptive SJF)

            sum_tat += ps[min_index].tat;                                                          // Sum turnaround times
            sum_wt += ps[min_index].wt;                                                            // Sum waiting times
            sum_rt += ps[min_index].rt;                                                            // Sum response times
            total_idle_time += (is_first_process == true) ? 0 : (ps[min_index].start_time - prev); // Calculate idle time

            completed++;                     // One more process is completed
            is_completed[min_index] = true;  // Mark this process as completed
            current_time = ps[min_index].ct; // Move the current time to the completion time of the current process
            prev = current_time;             // Store the last completed time
            is_first_process = false;        // Mark that the first process is now done
        }
    }

    // Calculate the length of the process completion cycle
    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;

    for (int i = 0; i < n; i++)
    {
        max_completion_time = findmax(max_completion_time, ps[i].ct); // Find the maximum completion time
        min_arrival_time = findmin(min_arrival_time, ps[i].at);       // Find the minimum arrival time
    }

    length_cycle = max_completion_time - min_arrival_time; // Calculate the total length of the cycle

    // Output the process details
    printf("\nProcess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t\t%d\t%d\t\t%d\t%d\t%d\t%d\n", ps[i].pid, ps[i].at, ps[i].bt, ps[i].ct, ps[i].tat, ps[i].wt, ps[i].rt);
    }

    // Calculate and display performance metrics
    printf("\nAverage Turn Around Time = %f ", (float)sum_tat / n);
    printf("\nAverage Waiting Time = %f ", (float)sum_wt / n);
    printf("\nAverage Response Time = %f ", (float)sum_rt / n);
    printf("\nThroughput = %f", n / (float)length_cycle);
    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle;
    printf("\nCPU Utilization (Percentage) = %f", cpu_utilization * 100);

    return 0;
}
