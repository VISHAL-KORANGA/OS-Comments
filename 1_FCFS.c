#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int pid;   // Process ID
    int at;    // Arrival Time
    int bt;    // Burst Time (Execution time)
    int ct;    // Completion Time
    float tat; // Turnaround Time (CT - AT)
    float wt;  // Waiting Time (TAT - BT)
    int rt;    // Response Time (equal to WT in FCFS)
    int st;    // Start Time (RT + AT)
};

int compare(const void *p1, const void *p2) // Basically comparing the process so that arrival time will be in sorted order.
{
    int a = ((struct Process *)p1)->at;
    int b = ((struct Process *)p2)->at;

    if (a < b)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}
int main()
{
    int n, swt = 0, stat = 0, sbt = 0, srt = 0;   // sum of waiting,turn around,burst,response time.
    float awt, atat, cpu_utilization, throughput; // average waiting time and average turn around time.

    printf("\nEnter the number of processes."); // Inputting the number of processes;
    scanf("%d", &n);

    struct Process p[n]; // Creating the structure for n processes..

    // Taking the inputs....
    for (int i = 0; i < n; i++)
    {
        printf("\nEnter the value of  arrival time and burst time for process %d: ", i + 1);
        p[i].pid = i + 1; // assigning the process id to each process.
        scanf("%d %d", &p[i].at, &p[i].bt);
    }

    qsort((void *)p, n, sizeof(struct Process), compare); // Will sort the processes on the basis of their arrival time.

    // Calculation of Completion time.
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            p[i].ct = p[i].at + p[i].bt; // arrival of first process
        }
        else if (p[i - 1].ct <= p[i].at)
        {
            p[i].ct = p[i].at + p[i].bt; // Process starts when it arrives.
        }
        else // here condition is p[i-1].ct > p[i].at
        {
            p[i].ct = p[i - 1].ct + p[i].bt; // process starts when previous process completes
        }

        p[i].tat = p[i].ct - p[i].at; // TAT = CT - AT
        p[i].wt = p[i].tat - p[i].bt; // WT = TAT - BT

        sbt += p[i].bt;    // sum of burst time.
        swt += p[i].wt;    // sum of waiting time.
        stat += p[i].tat;  // sum of turn around time.
        p[i].rt = p[i].wt; // For FCFS non premptive the response time is equal to waiting time.
    }

    awt = swt / n;   // Average waiting time.
    atat = stat / n; // Average turn around time.

    float max; // taking this variable to calculate maximum completion time for calculation of CPU utilization.

    for (int i = 0; i < n; i++)
    {
        p[i].st = p[i].rt + p[i].at; // since we know response time = start time - arrival time.
        max = 0;
        if (p[i].ct > max)
        {
            max = p[i].ct;
        }
    }

    cpu_utilization = (sbt / max) * 100;
    throughput = n / max;
    printf("\nProcess ID\tAriival Time\tBurst Time\tStart Time\tCompletion Time\t  Turn Around Time\tWaiting Time\t   Response Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("  P%d\t\t  %d\t\t  %d\t\t  %d\t\t  %d\t\t    %f\t\t  %f\t\t  %d\n", p[i].pid, p[i].at, p[i].bt, p[i].st, p[i].ct, p[i].tat, p[i].wt, p[i].rt);
    }

    printf("\nSum of Turn Around Time: %d\nAverage of Turn Around Time: %f", stat, atat);
    printf("\nSum of Waiting Time: %d\nAverage of Waiting Time: %f", swt, awt);
    printf("\nCPU Utilization is: %f", cpu_utilization);
    printf("\nThroughput is: %f", throughput);
    return 0;
}