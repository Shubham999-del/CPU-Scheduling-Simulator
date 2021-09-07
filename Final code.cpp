#include<iostream>
#include <fstream>
#include<iomanip>
#include <cstdlib>
#include<bits/stdc++.h> 
using namespace std;

class Process{
	public:
		int turnaround_time;
		int response_time;
		int waiting_time;
		int pId;
		int executed;  //to indicate if the process has been executed completely . -1 for executed.
		int remaining_time;
		int completion_time;
		int arrival_time;
		int burst_time;
		Process(){
			executed=0;
			pId=0;
			arrival_time=0;
			turnaround_time=0;
			burst_time=0;
			remaining_time=0;   //needed to do round robin as in that burst_time will change so remaining burst time needed
			response_time=-1;
			waiting_time=0;
			completion_time=0;
			cout << "process object created" << endl;
		}
		
		void init(int pId,float arrival_time, float burst_time){
			this->arrival_time=arrival_time;
			this->burst_time=burst_time;
			this->pId=pId;
			remaining_time=burst_time;
			
		}
		
		void completion(float completion_time=0){
			this->completion_time=completion_time;
			turnaround_time=completion_time-arrival_time;
			waiting_time=turnaround_time-burst_time;
		}
		
		void response(int t){
			if(response_time==-1)response_time=t-arrival_time;
		}
};

class Process_Creator{
	private:
		int AT;  // to store the arrival time
		int BT;  // to store the burst time
		
	public:
		Process* arr = new Process[100];  //creation of array of objects
		Process_Creator(){
		for(int i=0;i<100;i++){
			AT=rand()%100+1;
			BT=rand()%5+1;
			arr[i].init(i,AT,BT);
		}
		cout << "Processes initialised"<< endl;
	}
};


class Scheduler{
	private:
		int queue[30]={0}; //running queue for the simulator .stores the process ids.
		int bucket[100]={0};  //to store the processes id which are ready to get executed at the given point of time
		int K=-1;  //index to indicate the top element of bucket .just like top in queue.
		int marker;  //index to the top element in queue
		int dqt=-1;  //used to store the time the runnng process started its execution 
		int min_burst=INT_MAX;   //to store the minimum burst time required in STRF nad round robin
		int min_burst_index=-1;  //to store the index of minimum burst time process present in the bucket
		
	public:
		ofstream out;
		Process_Creator processes;
		Scheduler(){
			cout << "scheduler running" << endl;
			marker=-1;	
		}
		bool enqueue(int pId,int t){
			if(marker==29)return false;
			else{
				cout<< "enqueued" << endl;
				marker++;
				queue[marker]=pId;
				if(marker==0){
					dqt=t;
					out.open("status.txt",ios_base::app);
					out << setw(15) << t;
					out << setw(15) << processes.arr[queue[0]].pId;
					out << setw(15) << "arrived";
					out << endl;
					out.close();
					processes.arr[queue[0]].response(t);
				}
				return true;
			}
		}
		
		bool dequeue(int t){
			if(marker==-1)return false;
			else{
				cout << "dequeued" << endl;
				
				int p_executed=queue[0];
				processes.arr[queue[0]].completion(t);
				dqt=t;
				for(int i=0;i<marker;i++)queue[i]=queue[i+1];
				queue[marker]=0;
				marker--;
				out.open("status.txt",ios_base::app);
				out << setw(15) << t;
				out << setw(15) << processes.arr[p_executed].pId;
				out << setw(15) << "exit";
				out << endl;
				if(marker!=-1){
					out << setw(15) << t;
					out << setw(15) << processes.arr[queue[0]].pId;
					out << setw(15) << "arrived";
					out << endl;
					out.close();
					processes.arr[queue[0]].response(t);
				}		
				processes.arr[p_executed].executed=-1;
				return true;
			}
		}
		
		int FCFS(int t){
				cout << t<<" millisec" << endl;
				for(int i=0;i<100;i++){
					if(processes.arr[i].executed!=-1 && processes.arr[i].arrival_time==t){
						K++;
						bucket[K]=processes.arr[i].pId;
					}
				}
				
				if(marker!=-1 && t-dqt==processes.arr[queue[0]].burst_time){
					if(dequeue(t)==0){}
				}else if(marker!=-1){
					out.open("status.txt",ios_base::app);
					out << setw(15) << t;
					out << setw(15) << processes.arr[queue[0]].pId;
					out << setw(15) << "running";
					out << endl;
					out.close();
				}
				if(K!=-1 && enqueue(bucket[0],t)){
					for(int j=0;j<K;j++){
						bucket[j]=bucket[j+1];
					}
					bucket[K]=0;
					K--;
				}
				
				return 0;
			}
			
		int SRTF(int t){
			cout << t<<" millisec" << endl;
			int new_process=0;
			for(int i=0;i<100;i++){
				if(processes.arr[i].executed!=-1 && processes.arr[i].arrival_time==t){
					new_process=1;
					K++;
					bucket[K]=processes.arr[i].pId;
				}
			}
			if(new_process==1){
				for(int i=0;i<=K;i++){
					if(processes.arr[bucket[i]].burst_time<min_burst){
						min_burst=processes.arr[bucket[i]].burst_time;
						min_burst_index=i;
					}
				}
			}
			
			
			
			if(marker!=-1 && t-dqt==processes.arr[queue[0]].burst_time){
				dequeue(t);
			}else if(marker!=-1){
				out.open("status.txt",ios_base::app);
				out << setw(15) << t;
				out << setw(15) << processes.arr[queue[0]].pId;
				out << setw(15) << "running";
				out << endl;
				out.close();
			}
			
			if(K!=-1 && enqueue(bucket[min_burst_index],t)){
				for(int i=min_burst_index;i<K;i++)bucket[i]=bucket[i+1];
				bucket[K]=0;
				K--;
				min_burst_index=-1;
				min_burst=INT_MAX;
				for(int i=0;i<=K;i++){
					if(processes.arr[bucket[i]].burst_time<min_burst){
						min_burst=processes.arr[bucket[i]].burst_time;
						min_burst_index=i;
					}
				}
			}
			return 0;
		}
		int context_switch(int t,int time_quantam){
			processes.arr[queue[0]].remaining_time=processes.arr[queue[0]].remaining_time-time_quantam;
			int id=processes.arr[queue[0]].pId;
			
			out.open("status.txt",ios_base::app);
			out << setw(15) << t;
			out << setw(15) << id;
			out << setw(15) << "context switch";
			out << endl;
			out.close();
			
			for(int i=0;i<marker;i++)queue[i]=queue[i+1];
			
			dqt=t;
			queue[marker]=0;
			marker--;
			if(marker!=-1){
				out.open("status.txt",ios_base::app);
				out << setw(15) << t;
				out << setw(15) << processes.arr[queue[0]].pId;
				out << setw(15) << "arrived";
				out << endl;
				out.close();
			}
			processes.arr[queue[0]].response(t);
			return id;
		}
		
		int round_robin(int t, int time_quantam){
			cout << t<<" millisec" << endl;
			int new_process=0;
			for(int i=0;i<100;i++){
				if(processes.arr[i].executed!=-1 && processes.arr[i].arrival_time==t){
					new_process=1;
					K++;
					bucket[K]=processes.arr[i].pId;
				}
			}
			if(new_process==1){
				for(int i=0;i<=K;i++){
					if(processes.arr[bucket[i]].remaining_time<min_burst){
						min_burst=processes.arr[bucket[i]].burst_time;
						min_burst_index=i;
					}
				}
			}
			
			if(marker!=-1 && t-dqt==processes.arr[queue[0]].remaining_time){
				if(dequeue(t)){}
			}
			else if(marker!=-1 && t-dqt==time_quantam){
				K++;
				bucket[K]=context_switch(t,time_quantam);
			}else{
				if(marker!=-1){
					out.open("status.txt",ios_base::app);
					out << setw(15) << t;
					out << setw(15) << processes.arr[queue[0]].pId;
					out << setw(15) << "running";
					out << endl;
					out.close();
				}
			}
			
			if(K!=-1 && enqueue(bucket[min_burst_index],t)){
				for(int i=min_burst_index;i<K;i++)bucket[i]=bucket[i+1];
				bucket[K]=0;
				K--;
				min_burst_index=-1;
				min_burst=INT_MAX;
				for(int i=0;i<=K;i++){
					if(processes.arr[bucket[i]].remaining_time<min_burst){
						min_burst=processes.arr[bucket[i]].burst_time;
						min_burst_index=i;
					}
				}
			}

			return 0;
		}
};

class Simulator{
	public:
		int timer=0,end;
		Scheduler S; 
		void simulating(int sim_time,string sch_name,int preempt_t=0){
			
			ofstream outf;
			outf.open("status.txt");
			outf << setw(15) << "Millisecond" ;
			outf << setw(15) << "Process Id" ;
			outf << setw(15) << "Status" ;
			outf << endl;
			outf.close();
			for(timer=1;timer<=sim_time;timer++){
				if(sch_name=="fcfs")end=S.FCFS(timer);
				else if(sch_name=="srtf")end=S.SRTF(timer);
				else if(sch_name=="round robin")end=S.round_robin(timer,preempt_t);
				if(end==1)break;
			}
			
			outf.open("Processes.txt");
			outf << setw(15) << "Process Id" ;
			outf << setw(15) << "Arrival Time";
			outf << setw(15) << "Burst Time";
			outf << setw(15) << "Comp Time";
			outf << setw(15) << "  Turnaround Time";
			outf << setw(15) << "Waiting Time";
			outf << setw(15) << "Response Time";
			outf << endl;
			for(int i=0;i<100;i++){
				outf << setw(15) << S.processes.arr[i].pId ;
				outf << setw(15) << S.processes.arr[i].arrival_time;
				outf << setw(15) << S.processes.arr[i].burst_time;
				outf << setw(15) << S.processes.arr[i].completion_time;
				outf << setw(15) << S.processes.arr[i].turnaround_time;
				outf << setw(15) << S.processes.arr[i].waiting_time;
				outf << setw(15) << S.processes.arr[i].response_time;
				outf << endl;
			}
			outf.close();
		}
};

int main(){
	int simulation_time;
	string scheduling_algo;
	int preempt_time;
	cout << "Enter simulation_time in milliseconds" << endl;
	cin >> simulation_time;
	cin.ignore();
	cout << "Enter the name of scheduling_algo" << endl;
	getline(cin,scheduling_algo);
	transform(scheduling_algo.begin(), scheduling_algo.end(), scheduling_algo.begin(), ::tolower);
	
	if(scheduling_algo=="round robin"){
		cout << "Enter preemtion time" << endl;
		cin >> preempt_time;
		Simulator sim;
		sim.simulating(simulation_time,"round robin",preempt_time);
	}else if(scheduling_algo=="fcfs" || scheduling_algo=="first come first serve"){
		Simulator sim;
		sim.simulating(simulation_time,"fcfs");
	}else if(scheduling_algo=="srtf" || scheduling_algo=="shortest remainig time first"){
		Simulator sim;
		sim.simulating(simulation_time,"srtf");
	}
}


