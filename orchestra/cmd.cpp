#include <iostream>
// #include <fstream>
#include <boost/algorithm/string.hpp>
#include "commands.h"
#include "cmd.h"

#define choose "./choose"

inline bool exists_file (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

cmd::cmd(std::string input){
    std::cout<<input<<std::endl;
    size_t found = input.find("!");
    if(found!=std::string::npos){
        op = user;
    } else {
        found = input.find("?");
        if(found!=std::string::npos){
            op = program;
        }else{
            op = invalid;
        }    
    }

    std::string hosts_str = input.substr(0,found-1);
    std::string cmd_str = input.substr(found+1,input.length());
    
    command = cmd_str;
    if(hosts_str.length()!=0){
        boost::split(hosts, hosts_str, boost::is_any_of(","));
    }
}

std::vector<std::string> cmd::get_hosts(){
    return hosts;
}

option cmd::get_option(){
    return op;
}

std::string cmd::get_command(){
    return command;
}

bool cmd::is_path(){
    return contains_path;
}

std::string cmd::get_device_in_path(){
    return device;
}
std::string cmd::get_string(){
    int chosen_host = 0;
    if(op==2){
        return "invalid";
    }
    //Choosing best hosts
    if(op==1){
        int run;
        std::string choose_cmd = "mpiexec -n " +std::to_string(hosts.size()) + " -hosts ";
        for(int i =0;i<hosts.size();i++){
            if(i!=0){
                choose_cmd+=",";
            }
            choose_cmd+=hosts.at(i);
        }
        std::cout<<"ll"<<choose_cmd<<std::endl;
        choose_cmd+=choose;
        if(exists_file("./choose")){
            std::string result = exec(choose_cmd.c_str());
            std::cout<<"ll"<<result<<std::endl;
        }else{
            system("g++ ChooseHost.cpp -o choose");
            std::string result = exec(choose_cmd.c_str());
        }
    }

    std::string final_cmd = "mpiexec -n 1 -hosts " + hosts[chosen_host];
    
    final_cmd+=command;
    return final_cmd;
}
