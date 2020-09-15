#include <iostream>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
// #include "../commands.h"
using namespace std;

// //function to return the print statement from a terminal command
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

int main(){

    vector<string> hosts{"siddharth","kawhi"};


    map<string,double> ping;
    
   
    for(int i=0;i<hosts.size();i++){
        string result = exec(("ping -w 2 -q " + hosts[i] + "  | cut -d \"/\" -s -f5").c_str());
        if(result != "\0"){
             ping.insert({hosts[i],stof(result)});
        }else{
            cout<<hosts[i]<<" not found"<<endl;
            ping.insert({hosts[i],NULL});
        }
       
    }
    
    for (auto itr = ping.begin(); itr != ping.end(); ++itr) { 
        cout << itr->first<<"->"<< itr->second << '\n'; 
    } 
    return 0;
}
