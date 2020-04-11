#include <fstream>
#include "process.h"


// PURPOSE: Fills the vector of processes from the input file
// PARAMETER: The vector to be filled; called by reference.
// LOGIC: Open the file, read in the ID, Priority, and Burst length as
//        strings. Then convert the Priority and Burst length to ints.
//        Then create a new process with those parameters and push it
//        into the vector. Loop until the end of the file.
void fill_vector(vector<process>& processes)
{
    string id, pri, brst;

    ifstream fin;
    fin.open("schedule.txt");

    if(!fin)
    {
        cout << "File does not exist\n";
        return;
    }

    getline(fin, id, ',');
    getline(fin, pri, ',');
    int priority = stoi(pri);
    fin >> brst;
    int burst = stoi(brst);
    process p = process(id, priority, burst);
    processes.push_back(p);

    while (!fin.eof())
    {
        getline(fin, id, ',');
        getline(fin, pri, ',');
        priority = stoi(pri);
        fin >> brst;
        burst = stoi(brst);
        p = process(id, priority, burst);
        processes.push_back(p);
    }
    fin.close();
}


// PURPOSE: Simulate a First Come First Served scheduling algorithm
// PARAMETER: The (unprocessed) vector of processes to act on.
// LOGIC: Loop through the vector, in its original order, and run each
//        process completely before moving to the next.
void fcfs(vector<process>& processes)
{
    cout << "\n====================================================\n"
         << "First Come First Served\n"
         << "====================================================\n";

    int running_timer = 0;

    for (int i=0; i<(static_cast<signed int>(processes.size())); i++)
    {
        processes[i].add_wait(running_timer);
        running_timer += processes[i].get_burst();

        while (processes[i].get_burst_remaining() != 0)
        {
            processes[i].dec_burst();
        }
    }
}


// PURPOSE: Sort the vector of processes by burst length.
// PARAMETER: The (unprocessed) vector of processes to act on.
// LOGIC: Uses bubble sort based on the burst length of each process.
void sort_length(vector<process>& processes)
{
    int n = processes.size();
    for (int j = 0; j < n-1; j++)
    {
        for (int i = 0; i < n - 1; i++)
            if (processes[i].get_burst() > (processes[i + 1]).get_burst())
            {
                process temp = processes[i];
                processes[i] = processes[i + 1];
                processes[i + 1] = temp;
            }
    }
}


// PURPOSE: Simulate a Shortest Job First scheduling algorithm
// PARAMETER: The sorted (by burst length) vector of processes to act on.
// LOGIC: Call the sort function to put the vector in order by length of
//        the process bursts. Then loop through the vector, running each
//        process until completion.
void sjf(vector<process>& processes)
{
    cout << "\n====================================================\n"
         << "Shortest Job First\n"
         << "====================================================\n";

    sort_length(processes);

    int running_timer = 0;

    for (int i=0; i<(static_cast<signed int>(processes.size())); i++)
    {
        processes[i].add_wait(running_timer);
        running_timer += processes[i].get_burst();

        while (processes[i].get_burst_remaining() != 0)
        {
            processes[i].dec_burst();
        }
    }
}


// PURPOSE: Sort the vector of processes by priority.
// PARAMETER: The unsorted vector of processes to be sorted (by priority).
// LOGIC: Uses bubble sort based on the priority of each process.
void sort_priority(vector<process>& processes)
{
    int n = processes.size();
    for (int j = 0; j < n-1; j++)
    {
        for (int i = 0; i < n - 1; i++)
            if (processes[i].get_priority() > (processes[i + 1]).get_priority())
            {
                process temp = processes[i];
                processes[i] = processes[i + 1];
                processes[i + 1] = temp;
            }
    }
}


// PURPOSE: Simulate a Priority scheduling algorithm
// PARAMETER: The sorted (by priority) vector of processes to act on.
// LOGIC: Initially calls the sort function to get the vector into
//        priority order. Then loops through the vector, completing
//        each process one at a time as it goes.
void priority(vector<process>& processes)
{
    cout << "\n====================================================\n"
         << "Priority\n"
         << "====================================================\n";
    sort_priority(processes);

    int running_timer = 0;

    for (int i=0; i<(static_cast<signed int>(processes.size())); i++)
    {
        processes[i].add_wait(running_timer);
        running_timer += processes[i].get_burst();

        while (processes[i].get_burst_remaining() != 0)
        {
            processes[i].dec_burst();
        }
    }
}


// PURPOSE: Simulate a Round Robin scheduling algorithm
// PARAMETER: The unsorted vector of processes to act on.
// LOGIC: Initializes a counter for completed processes, and a
//        running timer to keep track of progress. After receiving
//        the time quantum length from the user, loops until each
//        process has been completed.
void rr(vector<process>& processes)
{
    int quantum;
    int completed_procs = 0; // counter
    int running_timer = 0;

    cout << "Enter the desired time quantum: ";
    cin >> quantum;

    cout << "\n====================================================\n"
         << "Round Robin\n"
         << "Time quantum of " << quantum << endl
         << "====================================================\n";

    while (completed_procs < (static_cast<signed int>(processes.size())))// still have processes to run
    {
        for (int i=0; i<(static_cast<signed int>(processes.size())); i++) // loop through the vector to check each process
        {
            // If the process was previously completed
            if (processes[i].get_burst_remaining() == 0)
            {}


            // If the time quantum will finish off the remaining time
            else if ((processes[i].get_burst_remaining() <= quantum) && (processes[i].get_burst_remaining() > 0))
            {
                processes[i].add_wait(running_timer);

                for (int x = processes[i].get_burst_remaining(); x > 0; x--)
                {
                    processes[i].dec_burst();
                    running_timer += 1;
                }
                processes[i].set_wait(running_timer);
                completed_procs++;
            }


            // if the process will have time remaining after running
            else if (processes[i].get_burst_remaining() > quantum)
            {
                processes[i].add_wait(running_timer);
                for (int x = 0; x < quantum; x++)
                {
                    processes[i].dec_burst();
                    running_timer += 1;
                }
            }
        }
    }
}


// PURPOSE: Prints the contents of the processed vector.
// PARAMETER: The final processed vector.
// LOGIC: Create variables to accumulate total turnaround and wait times
//        and a counter of how many processes are being printed.
//        Loop through the vector, accumulating turnaround and wait time
//        in the variables as it loops. Once complete, use the variables
//        to compute to overall average turnaround and wait times.
void print_vector(vector<process> processes)
{
    double avg_turnaround = 0;
    double avg_wait = 0;
    int counter = 0;

    for (int i=0; i<(static_cast<signed int>(processes.size())); i++)
    {
        cout << endl << processes[i].get_pid() << " Turnaround time = " << processes[i].get_turnaround()
        << ", Waiting time = " << processes[i].get_total_wait_time();

        avg_turnaround += processes[i].get_turnaround();
        avg_wait += processes[i].get_total_wait_time();
        counter++;
    }

    avg_turnaround /= counter;
    avg_wait /= counter;
    cout << "\n\nAverage turn-around time = " << avg_turnaround << ", Average waiting time = " << avg_wait << endl << endl;
}


// PURPOSE: Acts as the driver for the whole program, calling the
//          appropriate function based on the input of the user.
// PARAMETER: None
// LOGIC: Creates the vector, calls the fill function, then
//        calls the appropriate algorithm function based on
//        the users choice.
int main()
{
    cout << "====================================================\n"
         << "Michael Tobin and Travis Vensel\n"
         << "CS433 - Operating Systems\n"
         << "Assignment 3 - Scheduling Algorithms\n"
         << "Simulates 4 different scheduling algorithms, giving\n"
         << "the algorithm choice to the user.\n"
         << "====================================================\n\n";

    vector <process> processes;
    fill_vector(processes);

    int choice;
    cout << "what sorting method would you like to use?\n\n"
         << "1. First-Come, First-Served (FCFS)\n"
         << "2. Shortest-Job-First (SJF)\n"
         << "3. Priority\n"
         << "4. Round Robin" <<endl;
    cin >> choice;

    switch(choice)
    {
        case 1: {fcfs(processes);
            print_vector(processes);}
            break;
        case 2: {sjf(processes);
            print_vector(processes);}
            break;
        case 3: {priority(processes);
            print_vector(processes);}
            break;
        case 4: {rr(processes);
            print_vector(processes);}
            break;
        default: ;
    }

    return 0;
}