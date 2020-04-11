#include "process.h"


// PURPOSE: Default constructor
// PARAMETER: None
// LOGIC: None
process::process()
{}

// PURPOSE: Default destructor
// PARAMETER: None
// LOGIC: None
process::~process()
{}

// PURPOSE: Constructor
// PARAMETER: ID, Priority, and Burst length. Everything else within the process
//            has a default or derived value.
// LOGIC: Sets the Id and priority based off of the input of the same names.
//        Sets the burst and burst remaining as the same initially. Wait time
//        is zero as it has not waited at all upon initalization
process::process(string id, int priority, int burst)
{
    proc_id = id;
    proc_burst_length = burst;
    proc_burst_remaining = burst;
    proc_priority = priority;
    proc_wait_time = 0;
    proc_status = "NEW";
}


// PURPOSE: Changes the status of the process
// PARAMETER: Status to be set: READY, WAITING, RUNNING, TERMINATED
// LOGIC: Sets the process status as the status that has been passed.
void process::set_status(string status)
{
    proc_status = status;
}


// PURPOSE: Returns the current status of the process
// PARAMETER: None
// LOGIC: None
string process::get_status()
{
    return proc_status;
}


// PURPOSE: Returns the ID of the process
// PARAMETER: None
// LOGIC: None
string process::get_pid()
{
    return proc_id;
}


// PURPOSE: Returns the ORIGINAL burst length. This number never changes.
// PARAMETER: None
// LOGIC: None
int process::get_burst()
{
    return proc_burst_length;
}


// PURPOSE: Returns the REMAINING burst length. This number is decremented
//          as the process is run.
// PARAMETER: None
// LOGIC: None
int process::get_burst_remaining()
{
    return proc_burst_remaining;
}


// PURPOSE: Each time it is called, decrement the REMAINING burst by 1.
// PARAMETER: None
// LOGIC: Decrement burst remaining by 1 for each time that this function is called.
void process::dec_burst()
{
    proc_burst_remaining--;
}


// PURPOSE: Returns the total accumulated wait time of the process.
// PARAMETER: None
// LOGIC: None
int process::get_total_wait_time()
{
    return proc_wait_time;
}


// PURPOSE: Adds the amount of time passed to the function to the accumulated
//          wait time. Is used to accumulate wait time as processes run.
// PARAMETER: The amount of time to add to the wait time accumulator.
// LOGIC: Adds the time parameter to the total wait time in order to
//        accumulate wait time.
void process::add_wait(int time)
{
    proc_wait_time += time;
}


// PURPOSE: Used to set the final wait time. Is used in instances where accumulating
//          over time does not work properly.
// PARAMETER: The total amount of time that it took until the process completed.
// LOGIC: The total time minus the time that the process was actually being run
//        will give us the total wait time for the process.
void process::set_wait(int time) {
    proc_wait_time = time - proc_burst_length;
}


// PURPOSE: Returns the priority of the process.
// PARAMETER: None
// LOGIC: None
int process::get_priority()
{
    return proc_priority;
}


// PURPOSE: Returns the total turnaround time of the process.
// PARAMETER: None
// LOGIC: The wait time plus the total burst length = total turnaround time
int process::get_turnaround()
{
    int turnaround = proc_burst_length + proc_wait_time;
    return turnaround;
}

