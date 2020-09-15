#include <iostream>
#include <unistd.h>
#include <map>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iterator>

using namespace std;

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

int main(){
    map<string, double>::iterator itr; 
    cout << "\nThe map gquiz1 is : \n"; 
    cout << "\tKEY\tELEMENT\n";
    map<string, double> gquiz1 = get_mem_info(); 
    for (auto itr = gquiz1.begin(); itr != gquiz1.end(); ++itr) { 
        cout << "start->"<<itr->first<<"<-end->"<< itr->second << "<-end\n"; 
    }
    cout<<gquiz1.find("MemAvailable")->first<<" "<<gquiz1.find("MemTotal")->second<<endl;

    return 0;
}