#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* CITS2002 Final Project 2020
   Name(s):             Grant Alexander Taylor
   Student number(s):   21947126
 */


//  besttq (v1.0)
//  Written by Chris.McDonald@uwa.edu.au, 2019, free for all to copy and modify
//  Modified by Grant Taylor, 21947126@student.uwa.edu.au.


//  Compile with:  cc -std=c99 -Wall -Werror -o besttq besttq.c


//  THESE CONSTANTS DEFINE THE MAXIMUM SIZE OF TRACEFILE CONTENTS (AND HENCE
//  JOB-MIX) THAT YOUR PROGRAM NEEDS TO SUPPORT.  YOU'LL REQUIRE THESE
//  CONSTANTS WHEN DEFINING THE MAXIMUM SIZES OF ANY REQUIRED DATA STRUCTURES.

#define MAX_DEVICES             4
#define MAX_DEVICE_NAME         20
#define MAX_PROCESSES           50
#define PROCESS_STATE_LENGTH    9
// DO NOT USE THIS - #define MAX_PROCESS_EVENTS      1000
#define MAX_EVENTS_PER_PROCESS	100
#define MAX_EVENT_TYPE_LENGTH   5

#define TIME_CONTEXT_SWITCH     5
#define TIME_ACQUIRE_BUS        5


//  NOTE THAT DEVICE DATA-TRANSFER-RATES ARE MEASURED IN BYTES/SECOND,
//  THAT ALL TIMES ARE MEASURED IN MICROSECONDS (usecs),
//  AND THAT THE TOTAL-PROCESS-COMPLETION-TIME WILL NOT EXCEED 2000 SECONDS
//  (SO YOU CAN SAFELY USE 'STANDARD' 32-BIT ints TO STORE TIMES).

int optimal_time_quantum                = 0;
int total_process_completion_time       = 0;

// All connected device variables
char device_name[MAX_DEVICES][MAX_DEVICE_NAME];
int device_speed[MAX_DEVICES];
int device_count = 0;

// All connected process variables
// Process states are: "ready" and "blocked"
int process_id[MAX_PROCESSES];
char process_state[MAX_PROCESSES][PROCESS_STATE_LENGTH];
int process_start_time[MAX_PROCESSES];
int process_time_running[MAX_PROCESSES];
int process_event_counter[MAX_PROCESSES];
int process_count = 0;

// All connected event variables
char event_type[MAX_PROCESSES][MAX_EVENTS_PER_PROCESS][MAX_EVENT_TYPE_LENGTH];
char event_device[MAX_PROCESSES][MAX_EVENTS_PER_PROCESS][MAX_DEVICE_NAME];
int event_transfer_size[MAX_PROCESSES][MAX_EVENTS_PER_PROCESS];
int event_start_time[MAX_PROCESSES][MAX_EVENTS_PER_PROCESS];
int event_time_running[MAX_PROCESSES];
int event_count = 0;

// All connected data bus variables
int databus_queue[MAX_PROCESSES];
int processes_blocked = 0;
int processes_ready = 0;
//  ----------------------------------------------------------------------

#define CHAR_COMMENT            '#'
#define MAXWORD                 20

void parse_tracefile(char program[], char tracefile[])
{
//  ATTEMPT TO OPEN OUR TRACEFILE, REPORTING AN ERROR IF WE CAN'T
    FILE *fp    = fopen(tracefile, "r");

    if(fp == NULL) {
        printf("%s: unable to open '%s'\n", program, tracefile);
        exit(EXIT_FAILURE);
    }

    char line[BUFSIZ];
    int  lc     = 0;

//  READ EACH LINE FROM THE TRACEFILE, UNTIL WE REACH THE END-OF-FILE
    while(fgets(line, sizeof line, fp) != NULL) {
        ++lc;
        
        //  COMMENT LINES ARE SIMPLY SKIPPED
        if(line[0] == CHAR_COMMENT) {
            continue;
        }

        //  ATTEMPT TO BREAK EACH LINE INTO A NUMBER OF WORDS, USING sscanf()
        char    word0[MAXWORD], word1[MAXWORD], word2[MAXWORD], word3[MAXWORD];
        int nwords = sscanf(line, "%s %s %s %s", word0, word1, word2, word3);

        printf("%i = %s", nwords, line);

        //  WE WILL SIMPLY IGNORE ANY LINE WITHOUT ANY WORDS
        if(nwords <= 0) {
            continue;
        }

        //  LOOK FOR LINES DEFINING DEVICES, PROCESSES, AND PROCESS EVENTS
        else if(nwords == 4 && strcmp(word0, "device") == 0) {
            strcpy(device_name[device_count], word1);
            device_speed[device_count] = atoi(word2);
            device_count++;
            // FOUND A DEVICE DEFINITION, WE'LL NEED TO STORE THIS SOMEWHERE
        }

        else if(nwords == 1 && strcmp(word0, "reboot") == 0) {
            continue;   // NOTHING REALLY REQUIRED, DEVICE DEFINITIONS HAVE FINISHED
        }

        else if(nwords == 4 && strcmp(word0, "process") == 0) {
            process_id[process_count] = atoi(word1);
            process_start_time[process_count] = atoi(word2);
            strcpy(process_state[process_count], "waiting");
            process_time_running[process_count] = 0;
            // FOUND THE START OF A PROCESS'S EVENTS, STORE THIS SOMEWHERE
        }

        else if(nwords == 4 && strcmp(word0, "i/o") == 0) {
            strcpy(event_type[process_count][event_count], word0);
            event_start_time[process_count][event_count] = atoi(word1);
            strcpy(event_device[process_count][event_count], word2);
            event_transfer_size[process_count][event_count] = atoi(word3);
            event_time_running[process_count] = 0;
            event_count++;
            //  AN I/O EVENT FOR THE CURRENT PROCESS, STORE THIS SOMEWHERE
        }

        else if(nwords == 2 && strcmp(word0, "exit") == 0) {
            strcpy(event_type[process_count][event_count], word0);
            event_start_time[process_count][event_count] = atoi(word1);
            process_event_counter[process_count]++;
            event_count++;
            //  PRESUMABLY THE LAST EVENT WE'LL SEE FOR THE CURRENT PROCESS
        }

        else if(nwords == 1 && strcmp(word0, "}") == 0) {
            process_event_counter[process_count] = event_count;
            event_count = 0;
            process_count++;
            //  JUST THE END OF THE CURRENT PROCESS'S EVENTS
        }
        else {
            printf("%s: line %i of '%s' is unrecognized",
                program, lc, tracefile);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);
}

#undef  MAXWORD
#undef  CHAR_COMMENT

//  ----------------------------------------------------------------------

//  SIMULATE THE JOB-MIX FROM THE TRACEFILE, FOR THE GIVEN TIME-QUANTUM
void simulate_job_mix(int time_quantum)
{
    printf("running simulate_job_mix( time_quantum = %i usecs )\n",
        time_quantum);
}

//  ----------------------------------------------------------------------

void usage(char program[])
{
    printf("Usage: %s tracefile TQ-first [TQ-final TQ-increment]\n", program);
    exit(EXIT_FAILURE);
}

//  ----------------------------------------------------------------------

// Validate time quantum arguments
void validate_args(int argcount, char *argvalue[]){
    //  CALLED WITH THE PROVIDED TRACEFILE (NAME) AND THREE TIME VALUES
    if(argcount == 5) {
        int TQ0     = atoi(argvalue[2]);
        int TQfinal = atoi(argvalue[3]);
        int TQinc   = atoi(argvalue[4]);
        if(TQ0 < 1 || TQfinal < TQ0 || TQinc < 1) {
            usage(argvalue[0]);
        }
    }
    //  CALLED WITH THE PROVIDED TRACEFILE (NAME) AND ONE TIME VALUE
    else if(argcount == 3) {
        int TQ0     = atoi(argvalue[2]);
        if(TQ0 < 1) {
            usage(argvalue[0]);
        }
    }
    //  CALLED INCORRECTLY, REPORT THE ERROR AND TERMINATE
    else {
        usage(argvalue[0]);
    }
}

//  ----------------------------------------------------------------------

// Initialise Time Quantum Variables
void initialise_tq_variables(int argcount, char *argvalue[], int *TQ0, int *TQfinal, int *TQinc){
    //TQ VALUES FOR 5 ARGUMENTS CALLED
    if(argcount == 5) {
        *TQ0     = atoi(argvalue[2]);
        *TQfinal = atoi(argvalue[3]);
        *TQinc   = atoi(argvalue[4]);
    }
    // TQ VALUES FOR 3 ARGUMENTS CALLED
    if(argcount == 3) {
        *TQ0     = atoi(argvalue[2]);
        *TQfinal = *TQ0;
        *TQinc   = 1;
    }
}

//  ----------------------------------------------------------------------

// Reset all global variables for next time quantum iteration
//Resets process_time_running[MAX_PROCESSES]
//Resets event_time_running[MAX_PROCESSES][MAX_EVENTS_PER_PROCESS]
//Resets process_state[MAX_PROCESSES][PROCESS_STATE_LENGTH]
//Resets processes_blocked counter
//Resets processes_ready counter
void reset_run_times(int saved_process_count){
    for(int i = 0; i < saved_process_count; i++){
        process_time_running[i] = 0;
        event_time_running[i] = 0;
        strcpy(process_state[i], "waiting");
    }
    for(int i=0; i < MAX_PROCESSES; i++){
        databus_queue[i] = 0;
    }
    processes_blocked = 0;
    processes_ready = 0;
    process_count = saved_process_count;
}

//  ----------------------------------------------------------------------

// Find the event a process is currently up to
int find_current_event(int current_process){
    for(int i = 0; i < process_event_counter[current_process]; i++){
        if(event_time_running[current_process] < event_start_time[current_process][i]){
            return i;
        }
    }
    return 0;
}

//  ----------------------------------------------------------------------

// Find the device being used in a current event
int find_device(char current_device_name[MAX_DEVICE_NAME]){
    for(int i = 0; i < device_count; i++){
        if(strcmp(device_name[i], current_device_name) == 0){
            return i;
        }
    }
    return 0;
}

//  ----------------------------------------------------------------------

// Shuffle the bus queue left by one
void bus_shuffle_left(){
    for(int i = MAX_PROCESSES-1; i > 0; i--){
        databus_queue[i-1] = databus_queue[i];
    }
    databus_queue[MAX_PROCESSES-1] = 0;
}

//  ----------------------------------------------------------------------

// Run the current process in time quantum iteration
void run_process(int current_process, int time_quantum, int *completion_time){
    // If process is waiting
    if(strcmp(process_state[current_process],"waiting") == 0){
        // If time quantum is less than next process waiting time:
        // Update completion time by the sum of time quantum.
        // Subtract process_start_time of upcoming event by the time quantum.
        // Time quantum expires, next process begins
        if( (time_quantum + process_time_running[current_process]) < process_start_time[current_process]){
            process_time_running[current_process] += time_quantum;
            *completion_time += time_quantum;
            return;
        }

        // Else process will begin
        // Change process_state to ready
        // Update completion time by sum of process_start_time
        // Calculate remaining time quantum
        // run_process with remaining time quantum recusively
        else{            
            strcpy(process_state[current_process],"ready");
            *completion_time += process_start_time[current_process] + 5;
            time_quantum -= process_start_time[current_process];
            processes_ready++;
            run_process(current_process, time_quantum, completion_time);
            return;
        }
    }
    
    // If the process is ready
    else if(strcmp(process_state[current_process],"ready") == 0){
        int current_event = find_current_event(current_process);
        // If time quantum is less than next event waiting time:
        // Update completion time by the sum of time quantum.
        // Subtract event_start_time of upcoming event by the time quantum.
        // Time quantum expires, next process begins
        if( (time_quantum + event_time_running[current_process]) < event_start_time[current_process][current_event]){
            *completion_time += time_quantum;
            event_time_running[current_process] += time_quantum;
        }

        // An event will run
        else {
            *completion_time += (event_start_time[current_process][current_event] - event_time_running[current_process]);
            // Block i/o events
            if(strcmp(event_type[current_process][current_event],"i/o") == 0){
                strcpy(process_state[current_process], "blocked");
                databus_queue[processes_blocked] = current_process;
                processes_blocked++;
                processes_ready--;
            }
            // Remove process from roation
            else if(strcmp(event_type[current_process][current_event],"exit") == 0){
                strcpy(process_state[current_process],"complete");
                process_count--;
                processes_ready--;
            }
        }
    }

    // If the process is blocked
    else if(strcmp(process_state[current_process],"blocked") == 0){
        int current_event = find_current_event(current_process);
        char current_device_name[MAX_DEVICE_NAME];
        strcpy(current_device_name, event_device[current_process][current_event]);
        int current_device = find_device(current_device_name);
        int current_device_speed = device_speed[current_device];
        
        // If all processes are blocked, and the current process has the bus
        // complete the transfer and shift bus queue
        // set process state to ready
        if(processes_ready == 0 && process_id[current_process] == databus_queue[0]){
            *completion_time += (int)(event_transfer_size[current_process][current_event] / (((double)current_device_speed) / 1000000));
            bus_shuffle_left();
            strcpy(process_state[current_process],"ready");
            processes_ready++;
            processes_blocked--;
        }
        //If current process does not have the bus, move to next process.
    }
}

//  ----------------------------------------------------------------------

int main(int argcount, char *argvalue[])
{
    // VALIDATE ARGUMENTS AND TERMINATE IF INVALID
    validate_args(argcount, argvalue);

    // CREATE AND INITIALISE TIME QUANTUM VARIABLES

    int TQ0 = 0, TQfinal = 0, TQinc = 0;
    initialise_tq_variables(argcount, argvalue, &TQ0, &TQfinal, &TQinc);

//  READ THE JOB-MIX FROM THE TRACEFILE, STORING INFORMATION IN DATA-STRUCTURES
    parse_tracefile(argvalue[0], argvalue[1]);


//  SIMULATE THE JOB-MIX FROM THE TRACEFILE, VARYING THE TIME-QUANTUM EACH TIME.
//  WE NEED TO FIND THE BEST (SHORTEST) TOTAL-PROCESS-COMPLETION-TIME
//  ACROSS EACH OF THE TIME-QUANTA BEING CONSIDERED
    int buffer_time = process_start_time[0];
    int completion_time = 0;
    int current_process = 0;
    int saved_process_count = process_count;
    
    // Begin time quantum iterations
    for(int time_quantum=TQ0 ; time_quantum<=TQfinal ; time_quantum += TQinc) {
        simulate_job_mix(time_quantum);
        
        // Run all processes in current iteration
        while(process_count > 0){
            run_process(current_process, time_quantum, &completion_time);
            if(process_count == 0){break;}
            else{current_process = (current_process+1) % saved_process_count;}
        }
        
        // Update best time values
        if(completion_time < total_process_completion_time || total_process_completion_time == 0){
            optimal_time_quantum = time_quantum;
            total_process_completion_time = completion_time;
        }

        // Reset all used global variables for next time quantum iteration
        reset_run_times(saved_process_count);
        completion_time = 0;
    }

    //remove accumulated tq before first process began    
    total_process_completion_time -= buffer_time;
    //  PRINT THE PROGRAM'S RESULT
    printf("best %i %i\n", optimal_time_quantum, total_process_completion_time);
    exit(EXIT_SUCCESS);
}

//  vim: ts=8 sw=4