#include "cmd.h"
#include <iostream>
#include <boost/algorithm/string.hpp> 

cmd::cmd(std::string input){
    std::cout<<input<<std::endl;
    size_t found = input.find("!");
    if(found!=std::string::npos){
        op = user;
    } else {
        op = program;
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
    std::string final_cmd = "mpiexec -n " +std::to_string(hosts.size()) + " -hosts ";
    for(int i =0;i<hosts.size();i++){
        if(i!=0){
            final_cmd+=",";
        }
        final_cmd+=hosts.at(i);
    }
    final_cmd+=command;
    return final_cmd;
}
