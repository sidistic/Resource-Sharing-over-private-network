#include <mpi.h>
#include <iostream>
#include <numeric>
#include <unistd.h>
#include <vector>
#include <fstream>

std::vector<size_t> get_cpu_times() {
    std::ifstream proc_stat("/proc/stat");
    proc_stat.ignore(5, ' '); // Skip the 'cpu' prefix.
    std::vector<size_t> times;
    for (size_t time; proc_stat >> time; times.push_back(time));
    return times;
}
 
bool get_cpu_times(size_t &idle_time, size_t &total_time) {
    const std::vector<size_t> cpu_times = get_cpu_times();
    if (cpu_times.size() < 4)
        return false;
    idle_time = cpu_times[3];
    total_time = std::accumulate(cpu_times.begin(), cpu_times.end(), 0);
    return true;
}

int main(int argc, char *[]){
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char device_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(device_name, &name_len);

    //to calculate current device cpu usage
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

    float* all_cpu_usage = new float[world_size];

    //send the cpu usage details
    
    all_cpu_usage[world_rank] = utilization;
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Allgather(&utilization,1,MPI_FLOAT,all_cpu_usage,1,MPI_FLOAT,MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    // this is just to check if it is working
    for(int i=0;i<world_size;i++){
        std::cout<<device_name<<" Device, num = "<<i<<", CPU "<<all_cpu_usage[i]<<std::endl;
    }

    MPI_Finalize();


}