#include <stdio.h>   // Standard input/output header for using printf and scanf functions
#include <stdbool.h> // Boolean header to use bool, true, false types

// Define structure to store the information of each process
struct process_info
{
    int max[10];       // Max resources the process may need
    int allocated[10]; // Resources currently allocated to the process
    int need[10];      // Resources still needed by the process (max - allocated)
};

// Global variables to store number of processes and number of resources
int no_of_process, no_of_resources;

// Function to take input for processes' maximum needs, allocated resources, and available resources
void input(struct process_info process[], int available[])
{
    // Loop to input data for each process
    for (int i = 0; i < no_of_process; i++)
    {
        printf("Enter process[%d] info\n", i);
        printf("Enter Maximum Need: ");
        // Input maximum resources the process will need for each resource type
        for (int j = 0; j < no_of_resources; j++)
            scanf("%d", &process[i].max[j]);

        printf("Enter No. of Allocated Resources for this process: ");
        // Input allocated resources for the process and calculate the need
        for (int j = 0; j < no_of_resources; j++)
        {
            scanf("%d", &process[i].allocated[j]);
            // Calculate the need as (max - allocated) resources
            process[i].need[j] = process[i].max[j] - process[i].allocated[j];
        }
    }

    // Input available resources in the system
    printf("Enter Available Resources: ");
    for (int i = 0; i < no_of_resources; i++)
        scanf("%d", &available[i]);
}

// Function to display the process information in tabular form
void showTheInfo(struct process_info process[])
{
    printf("\nPID\tMaximum\t\tAllocated\tNeed\n");
    // Loop through each process and print the details
    for (int i = 0; i < no_of_process; i++)
    {
        printf("P[%d]\t", i); // Process ID

        // Print maximum resources needed by the process
        for (int j = 0; j < no_of_resources; j++)
            printf("%d ", process[i].max[j]);

        printf("\t\t");

        // Print currently allocated resources
        for (int j = 0; j < no_of_resources; j++)
            printf("%d ", process[i].allocated[j]);

        printf("\t\t");

        // Print remaining resources needed (calculated earlier)
        for (int j = 0; j < no_of_resources; j++)
            printf("%d ", process[i].need[j]);

        printf("\n");
    }
}

// Function to apply Banker's Safety Algorithm
bool applySafetyAlgo(struct process_info process[], int available[], int safeSequence[])
{
    bool finish[no_of_process]; // Array to mark if a process is finished
    int work[no_of_resources];  // Copy of available resources (work)

    // Initialize work array with available resources
    for (int i = 0; i < no_of_resources; i++)
        work[i] = available[i];

    // Initialize all processes as unfinished
    for (int i = 0; i < no_of_process; i++)
        finish[i] = false;

    bool proceed = true; // Boolean to check if we can proceed with the next process
    int k = 0;           // To store safe sequence

    // While we can proceed and find processes in safe state
    while (proceed)
    {
        proceed = false;
        // Loop through each process to check if it can finish
        for (int i = 0; i < no_of_process; i++)
        {
            bool flag = true; // Flag to check if current process can complete

            // If the process is not finished
            if (finish[i] == false)
            {
                // Check if the process's need is less than or equal to work
                for (int j = 0; j < no_of_resources; j++)
                {
                    if (process[i].need[j] > work[j])
                    {
                        flag = false; // If the process's need > work, it can't proceed
                        break;
                    }
                }
                // If the process can finish, update the work and finish arrays
                if (flag == true)
                {
                    // Add the allocated resources of this process back to work
                    for (int j = 0; j < no_of_resources; j++)
                        work[j] = work[j] + process[i].allocated[j];

                    finish[i] = true;      // Mark process as finished
                    safeSequence[k++] = i; // Add this process to safe sequence
                    proceed = true;        // Set proceed to true as we found a safe process
                }
            }
        }
    }

    // Check if all processes have finished
    for (int i = 0; i < no_of_process; i++)
    {
        if (finish[i] == false)
            return false; // If any process is unfinished, return false (unsafe)
    }
    return true; // All processes finished, return true (safe)
}

// Function to check if the system is in a safe state
bool isSafeState(struct process_info process[], int available[], int safeSequence[])
{
    // Call safety algorithm and return the result
    if (applySafetyAlgo(process, available, safeSequence))
        return true;
    return false;
}

// Main function
int main()
{
    printf("Enter No of Processes\n");
    scanf("%d", &no_of_process); // Input number of processes

    printf("Enter No of Resource Instances in system\n");
    scanf("%d", &no_of_resources); // Input number of resources

    int available[no_of_resources];  // Available resources array
    int safeSequence[no_of_process]; // Array to store the safe sequence

    // Array of structures to store information about each process
    struct process_info process[no_of_process];

    printf("****Enter details of processes****\n");
    // Call the input function to get process details and available resources
    input(process, available);

    // If the system is in a safe state, print the safe sequence
    if (isSafeState(process, available, safeSequence))
    {
        printf("\nSystem is in SAFE State\n");
        printf("Safe Sequence is: ");
        for (int i = 0; i < no_of_process; i++)
            printf("P[%d] ", safeSequence[i]); // Print safe sequence
        printf("\n");
    }
    else
    {
        // If the system is not in a safe state, print unsafe
        printf("System is NOT in a SAFE State\n");
    }

    return 0; // End of program
}
