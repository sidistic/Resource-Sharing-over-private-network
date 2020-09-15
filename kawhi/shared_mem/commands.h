
using namespace std;

string exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


map<string, double> get_mem_info(){
    ifstream f("/proc/meminfo");
    map<string, double> mem_info;
    int i = 0;
    while(f){
        string attr;
        f >> attr;
        string val;
        f >> val;
        mem_info.insert({attr.substr(0,attr.length()-1),atof(val.c_str())});
        f>>attr;
    }
    return mem_info;
}

vector<size_t> get_cpu_times() {
    ifstream proc_stat("/proc/stat");
    proc_stat.ignore(5, ' '); // Skip the 'cpu' prefix.
    vector<size_t> times;
    for (size_t time; proc_stat >> time; times.push_back(time));
    return times;
}
 
bool get_cpu_times(size_t &idle_time, size_t &total_time) {
    const vector<size_t> cpu_times = get_cpu_times();
    if (cpu_times.size() < 4)
        return false;
    idle_time = cpu_times[3];
    total_time = accumulate(cpu_times.begin(), cpu_times.end(), 0);
    return true;
}