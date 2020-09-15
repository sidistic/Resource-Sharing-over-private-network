/* This code is the interface for users to use the distributed system.
 The command has 3 parts:
    1.The hosts/device part. beginning
    2.The option. ? or !
    3.The linux command. This is any generic linux command
 The user has 2 options:
    1. Specify which device to run the command on. Done by '!'.
    2. Let the program choose the device on which to run the command.
    The user can also give priority to a few devices. Done by '?'.

    <hosts separated by ,> <?/!> <command>

 The user cannot use commands such as sudo commands, ssh commands or
 or commands which give the user in the terminal more function. In
 general he can use any single shot commands.*/

#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include "cmd.h"
using namespace std;

cmd input_command();
void get_network_stats();

int main(){

    while(true){
        cout<<"Enter the input:"<<endl;
        cmd input_cmd = input_command();
        cout<<"__________________________"<<input_cmd.get_string().c_str()<<endl;
        string exec_cmd = input_cmd.get_string();
        if(exec_cmd.compare("invalid")){
           system(input_cmd.get_string().c_str());
        }else{
           cout<<"Invalid Option"<<endl;
        }

    }
    return 0;
}

cmd input_command(){
   string input;
   getline(cin,input);
   return cmd(input);
}