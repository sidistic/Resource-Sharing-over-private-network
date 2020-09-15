#include <bits/stdc++.h>
#include <string>
#include <iostream>

using namespace std;

int main(){
    string shared_mem_dir = "~/shared_mem/";
    string cmd = "ifconfig";
    string hosts = "siddharth";
    string runcmd = "mpiexec";
    runcmd =runcmd + " -n 1 -hosts " + "siddharth " + cmd;
    system(runcmd.c_str());
    return 0;

}