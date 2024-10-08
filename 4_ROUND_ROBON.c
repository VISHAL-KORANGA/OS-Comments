#include <stdio.h>   // Standard input/output library for printf and scanf
#include <limits.h>  // Provides INT_MIN for maximum completion time comparison
#include <stdbool.h> // For using boolean data types (true/false)
#include <stdlib.h>  // For using qsort function for sorting

// Structure to represent each process
struct process_struct
{
    int pid;          // Process ID
    int at;           // Arrival Time
    int bt;           // Burst Time
    int ct;           // Completion Time
    int wt;           // Waiting Time
    int tat;          // Turnaround Time
    int rt;           // Response Time
    int start_time;   // Start Time of execution
    int bt_remaining; // Remaining Burst Time after execution
} ps[100];            // Array to hold up to 100 processes

// Function to find the maximum of two integers
int findmax(int a, int b)
{
    return a > b ? a : b; // Return the greater of a or b
}

// Comparator function to sort processes by arrival time (for qsort)
int comparatorAT(const void *a, const void *b)
{
    int x = ((struct process_struct *)a)->at; // Arrival time of process a
    int y = ((struct process_struct *)b)->at; // Arrival time of process b
    return (x < y) ? -1 : 1;                  // Return -1 if a arrives before b, otherwise return 1
}

// Comparator function to sort processes by process ID (for qsort)
int comparatorPID(const void *a, const void *b)
{
    int x = ((struct process_struct *)a)->pid; // Process ID of process a
    int y = ((struct process_struct *)b)->pid; // Process ID of process b
    return (x < y) ? -1 : 1;                   // Return -1 if a's PID is less than b's, otherwise return 1
}

int main()
{
    int n, index;                                             // Number of processes and the index for the current process
    float cpu_utilization;                                    // Variable to store CPU utilization
    bool visited[100] = {false}, is_first_process = true;     // Track visited processes and if it's the first process
    int current_time = 0, max_completion_time;                // Current time and maximum completion time
    int completed = 0, tq, total_idle_time = 0, length_cycle; // Variables for process completion count, time quantum, idle time, and length of cycle
    int queue[100], front = -1, rear = -1;                    // Queue for Round Robin scheduling
    float sum_tat = 0, sum_wt = 0, sum_rt = 0;                // Variables to sum turnaround, waiting, and response times

    printf("Enter total number of processes: ");
    scanf("%d", &n); // Input the total number of processes

    // Input arrival time for each process
    for (int i = 0; i < n; i++)
    {
        printf("\nEnter Process %d Arrival Time: ", i);
        scanf("%d", &ps[i].at); // Read arrival time for process i
        ps[i].pid = i;          // Assign process ID
    }

    // Input burst time for each process
    for (int i = 0; i < n; i++)
    {
        printf("\nEnter Process %d Burst Time: ", i);
        scanf("%d", &ps[i].bt);        // Read burst time for process i
        ps[i].bt_remaining = ps[i].bt; // Initialize remaining burst time
    }

    printf("\nEnter time quanta: ");
    scanf("%d", &tq); // Input the time quantum for Round Robin

    qsort((void *)ps, n, sizeof(struct process_struct), comparatorAT); // Sort processes by arrival time

    front = rear = 0;  // Initialize the queue
    queue[rear] = 0;   // First process goes into the queue
    visited[0] = true; // Mark the first process as visited

    // Main loop: continue until all processes are completed
    while (completed != n)
    {
        index = queue[front]; // Get the process at the front of the queue
        front++;              // Simulate dequeue operation

        // If this is the first time this process is being executed
        if (ps[index].bt_remaining == ps[index].bt)
        {
            ps[index].start_time = findmax(current_time, ps[index].at);                              // Set the start time for the process
            total_idle_time += (is_first_process == true) ? 0 : ps[index].start_time - current_time; // Calculate idle time
            current_time = ps[index].start_time;                                                     // Update the current time to the start time of this process
            is_first_process = false;                                                                // Mark that the first process has been executed
        }

        // If remaining burst time is greater than time quantum
        if (ps[index].bt_remaining - tq > 0)
        {
            ps[index].bt_remaining -= tq; // Decrease burst time
            current_time += tq;           // Increment current time by time quantum
        }
        else // If remaining burst time is less than or equal to time quantum
        {
            current_time += ps[index].bt_remaining;             // Add remaining burst time to current time
            ps[index].bt_remaining = 0;                         // Process is completed
            completed++;                                        // Increment completed process count
            ps[index].ct = current_time;                        // Set completion time
            ps[index].tat = ps[index].ct - ps[index].at;        // Calculate turnaround time
            ps[index].wt = ps[index].tat - ps[index].bt;        // Calculate waiting time
            ps[index].rt = ps[index].start_time - ps[index].at; // Calculate response time

            // Sum turnaround, waiting, and response times
            sum_tat += ps[index].tat;
            sum_wt += ps[index].wt;
            sum_rt += ps[index].rt;
        }

        // Add new processes to the ready queue that have arrived
        for (int i = 1; i < n; i++)
        {
            if (ps[i].bt_remaining > 0 && ps[i].at <= current_time && visited[i] == false)
            {
                queue[++rear] = i; // Add process to the queue
                visited[i] = true; // Mark as visited
            }
        }

        // If process is not finished, add it back to the ready queue
        if (ps[index].bt_remaining > 0)
        {
            queue[++rear] = index; // Add it back to the queue
        }

        // If the queue is empty, add one process from the remaining list
        if (front > rear)
        {
            for (int i = 1; i < n; i++)
            {
                if (ps[i].bt_remaining > 0)
                {
                    queue[rear++] = i; // Add process to the queue
                    visited[i] = true; // Mark as visited
                    break;             // Break to prevent adding multiple processes at once
                }
            }
        }
    }

    max_completion_time = INT_MIN; // Initialize max completion time
    for (int i = 0; i < n; i++)
    {
        max_completion_time = findmax(max_completion_time, ps[i].ct); // Find the maximum completion time
    }
    length_cycle = max_completion_time - ps[0].at; // Calculate total length of the process cycle

    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle; // Calculate CPU utilization

    qsort((void *)ps, n, sizeof(struct process_struct), comparatorPID); // Sort processes by their IDs for displaying output

    // Output the final results
    printf("\nProcess No.\tAT\tCPU Burst Time\tStart Time\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t\t%d\t%d\t\t%d\t\t%d\t%d\t%d\t%d\n", i, ps[i].at, ps[i].bt, ps[i].start_time, ps[i].ct, ps[i].tat, ps[i].wt, ps[i].rt);
    }
    printf("\n");

    // Output averages and performance metrics
    printf("\nAverage Turn Around Time= %.2f", (float)sum_tat / n);
    printf("\nAverage Waiting Time= %.2f", (float)sum_wt / n);
    printf("\nAverage Response Time= %.2f", (float)sum_rt / n);
    printf("\nThroughput= %.2f", n / (float)length_cycle);
    printf("\nCPU Utilization (Percentage)= %.2f%%", cpu_utilization * 100);

    return 0; // Return 0 to indicate successful execution
}
