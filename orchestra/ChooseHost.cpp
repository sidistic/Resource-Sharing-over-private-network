/* Code to find the best PC to run a given process
It concides following thinks
-> CPU Usage
-> Max CPU speed(in Mhz)
-> Free Memory (in GB)
-> Round trip time between master and slave (ms)

Hieristic function used 
(1-CPU Usage)*Max CPU speed + Free Memory - Round trip time

master/root - 0 world rank
*/
#include <iostream>
#include <fstream>
#include <numeric> 
#include <map>
#include <vector>
#include <mpi.h>
#include <boost/algorithm/string.hpp>
#include "commands.h"

using namespace std;

#define CPU_MHZ "CPU MHz"
#define CPU_max_MHZ "CPU max MHz"
#define CPU_min_MHZ "CPU min MHz"
#define MemFree "MemFree"
#define MemTotal "MemTotal"

int main(int argc, char** argv){
    
    MPI_Init(&argc, &argv);

     // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

/*
-------------------------------------------------------------
            Getting round trip time to master/root
-------------------------------------------------------------
*/
    char* master;
    int size;
    if(world_rank==0){
        master = processor_name;
        size = sizeof(master);
    }
    // MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // if(world_rank!=0){
    //     master = (char *) malloc(size*sizeof(char));
    // }
    // // MPI_Barrier(MPI_COMM_WORLD);
    // MPI_Bcast(&master, size, MPI_CHAR, 0, MPI_COMM_WORLD);
    // MPI_Barrier(MPI_COMM_WORLD);
    // cout<<world_rank<<"----"<<master<<"--"<<processor_name<<"--"<<master<<endl;
/*
-------------------------------------------------------------
            Getting free memory
-------------------------------------------------------------
*/
    map<string, double> mem_info = get_mem_info(); 
    float free_mem = mem_info.find(MemFree)->second;
    // cout<<"total mem - "<<processor_name<<"->"<<mem_info.find(MemTotal)->second<<endl;

    float *free_mems = NULL;
    if (world_rank == 0) {
        free_mems = (float *)malloc(sizeof(float) * world_size);
    }
/*
-------------------------------------------------------------
            Getting cpu utilistion
-------------------------------------------------------------
*/
    size_t previous_idle_time=0, previous_total_time=0;
    size_t idle_time, total_time;
    float utilization;
    get_cpu_times(idle_time, total_time);
    int n=0;
    for (size_t idle_time, total_time; get_cpu_times(idle_time, total_time); sleep(1)) {
        float idle_time_delta = idle_time - previous_idle_time;
        float total_time_delta = total_time - previous_total_time;
        utilization = 100.0 * (1.0 - idle_time_delta / total_time_delta);
        
        previous_idle_time = idle_time;
        previous_total_time = total_time;
        if(n++>1) break;
    }
    
    float *utilisations = NULL;
    if (world_rank == 0) {
        utilisations = (float *)malloc(sizeof(float) * world_size);
    }
    
/*
-------------------------------------------------------------
            Getting cpu speeds
-------------------------------------------------------------
*/
    string r = exec("lscpu | grep \"MHz\"");
    
    map<string, double> cpu_mhz;
    vector<string> result;
    vector<string> temp;

    boost::split(result, r, boost::is_any_of("\n"),boost::token_compress_on);
    
    for (int i = 0; i < result.size(); i++){
        boost::split(temp, result[i], boost::is_any_of(":"),boost::token_compress_on);
        if(temp.size()==2){
            cpu_mhz.insert({temp[0],stof(temp[1])});
        }
    }
    // cout<<processor_name<<cpu_mhz.size()<<endl;
    // for (auto itr = cpu_mhz.begin(); itr != cpu_mhz.end(); ++itr) { 
    //     cout <<itr->first<<"->"<< itr->second << '\n'; 
    // }

    float cpu_fz;
    if(cpu_mhz.size()==1){
        cpu_fz = cpu_mhz.begin()->second;
    }else if(cpu_mhz.size() == 3){
        cpu_fz = cpu_mhz.find(CPU_max_MHZ)->second;
    }

    float *cpu_fzs = NULL;
    if (world_rank == 0) {
        cpu_fzs = (float *)malloc(sizeof(float) * world_size);
    }

    //Gathering all variables at the root/master node
    MPI_Gather(&cpu_fz, 1, MPI_FLOAT, cpu_fzs, 1, MPI_FLOAT, 0,
           MPI_COMM_WORLD);
    MPI_Gather(&free_mem, 1, MPI_FLOAT, free_mems, 1, MPI_FLOAT, 0,
           MPI_COMM_WORLD);
    MPI_Gather(&utilization, 1, MPI_FLOAT, utilisations, 1, MPI_FLOAT, 0,
           MPI_COMM_WORLD);
    
    if (world_rank == 0) {
        vector<float> values;
        for(int i=0;i<world_size;i++){
            // cout<<i<<" "<<cpu_fzs[i]/1000<<"->"<<free_mems[i]/(1024*1024)<<"->"<<utilisations[i]<<endl;
            values.push_back((100-utilisations[i]) * cpu_fzs[i] / 1000 + free_mems[i]/(1024*1024));
            // cout<<"value->"<<values[i]<<endl;
        }
        cout<<max_element(values.begin(),values.end()) - values.begin()<<endl;
    }
    // Finalize the MPI environment. No more MPI calls can be made after this
    MPI_Finalize();
    
    return 0;
}