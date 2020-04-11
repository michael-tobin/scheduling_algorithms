#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class process
{
private:
    int proc_burst_length;         // Time required to complete the process
    int proc_burst_remaining;         // Time left for process to complete
    int proc_wait_time;    // Total wait time a process endured.
    int proc_priority;           // Priority of process
    string proc_id;                // Process Id
    string proc_status;          // Status of process
public:
    process();
    ~process();

    process(string id, int priority, int burst);

    void set_status(string status);
    string get_status();

    string get_pid();

    int get_burst();
    int get_burst_remaining();
    void dec_burst();

    int get_total_wait_time();
    void add_wait(int time);
    void set_wait(int time);

    int get_priority();
    int get_turnaround();

};

#endif
