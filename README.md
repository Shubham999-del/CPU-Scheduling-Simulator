# CPU-Scheduling-Simulator
In this project, I have designed a simulator to simulate the behavior of a CPU scheduler, i.e.,
choosing a process from the ready queue based on a scheduling algorithm to be executed  by the
processor. The simulator has been implemented using C++. 

The simulator consists of the following classes:

(i) Process: The data members of this class stores process id, arrival time in the ready queue,
CPU burst time, completion time, turn around time, waiting time, and response time. The member
functions of this class assigns values to the data members and print them. A constructor
is also be used for initialising the data members.

(ii) Process_Creator: This class creates an array of processes and assign a random arrival time
and burst time to each process. Data members, constructor and member functions are written
accordingly.

(iii) Scheduler: This class implements the scheduling algorithm. The class maintains a ready
queue of fixed capacity. The ready queue is a queue in which only enqueue and dequeue operations
can be done. The process to be enqueued or dequeued will depend on the scheduling algorithm. 
Since, the size of the queue is limited, possibilities of swap in and swap out of a process is there.
In the project, three scheduling algorithms have been implemented, viz., First Come First Serve (FCFS),
Shortest Remaining Time First (SRTF), and Round Robin.

(iv) Simulator: This class starts the simulation. If the simulation time is 1 second and arrival
time and burst time of a process are in terms of milliseconds then the class runs a ‘for’ loop from
t=0 to 1000. At each iteration, the class executes all the necessary functions and capture the
required values. The class also prints the output in the following ways:

  (a) A file named as processes.txt will be created which will contain a table having all the details regarding a particular process as follows :-
  
  | Process ID    |   Arrival Time    | Burst Time  | Completion Time | Turn Around Time | Waiting Time | Response Time |
  | ------------- |:-----------------:|:-----------:|:---------------:|:----------------:|:------------:|:-------------:|
  |      1        |       1 ms        |    4 ms     |      5 ms       |      4 ms        |     0 ms     |    0 ms       |
  |      2        |       2 ms        |    3 ms     |      8 ms       |      6 ms        |     3 ms     |    3 ms       |
  |      3        |       5 ms        |    4 ms     |     12 ms       |      7 ms        |     3 ms     |    3 ms       |
  
  Here, the number of rows will depend on the number of processes. 


  (b) A file named as status.txt which will print a table. For each millisecond, the file will show the  process id of the processes arrived in the system, running by the           processor and exiting from the  system. The number of rows in the table will depend on the number of times the ‘for’ loop is  running, i.e., the number of milliseconds in the     simulation time. However, if in a particular  millisecond, no process arrived or exited and the process being run by the processor is the same as  that of previous     
  millisecond then the current millisecond can be skipped from printing in the table. For example,  

  | Milisecond Number | Process ID | Status  |
  | ----------------- |:----------:|:-------:|
  |        1          |      1     | Arrived |
  |        1          |      1     | Running |
  |        2          |      2     | Arrived |
  |        5          |      1     | Exit    |
  |        5          |      2     | Running |
  |        5          |      3     | Arrived |
  |        8          |      2     | Exit    |
  |        8          |      3     | Running |
  |        12         |      3     | Exit    |


The main() function takes the following input from the user: (i) Simulation time (ii) Name of  scheduling algorithm (iii) Time quantum in case of Round Robin.
  
