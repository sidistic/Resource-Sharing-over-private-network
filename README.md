# Resource Sharing using MPI Package in C++ with Load Balancing Algorithm

**Project Duration: January 2020 - May 2020**

## Introduction

This project aims to address the challenge of efficiently sharing hardware resources among multiple unique devices in a distributed system. By leveraging the Message Passing Interface (MPI) package in C++, we implement a resource sharing system that incorporates a load balancing algorithm. The load balancing algorithm considers various factors such as network latency, hardware specifications, and current device load to distribute the resource sharing load effectively.

## Motivation

In today's technology landscape, the physical components of devices often become the main bottleneck. Many devices are either idle or working at their maximum capacity, while others have unused processing and storage capabilities. The idea behind this project is to explore the possibility of sharing hardware resources among devices to alleviate the load from overworked devices and utilize the underutilized ones effectively.

## Approach

### Distributed System Architecture

The resource sharing system is designed as a distributed system, enabling devices to collaborate and share their resources. We draw inspiration from cloud computing and storage, which employ similar concepts on a larger scale. However, unlike traditional cloud solutions that rely on centralized servers, our approach leverages the existing devices in the network, utilizing their idle resources efficiently.

### Network File System (NFS)

To facilitate the sharing of storage resources across devices in the network, we utilize the Network File System (NFS). NFS allows us to mount Linux directories over the network, enabling easy resource sharing among devices. By leveraging NFS, we can leave the task of data movement to the relevant device while building our resource sharing system on top of it.

### Message Passing Interface (MPI)

The resource sharing system is implemented using the Message Passing Interface (MPI) package. MPI provides a standardized and portable message-passing standard, enabling efficient communication between processes on different devices. We leverage MPI's core library routines in C++, C++, and Fortran to develop our resource sharing functionality.

### Load Balancing Algorithm

The key component of the resource sharing system is the load balancing algorithm. This algorithm takes into account several factors to calculate scores for each device in the network. The factors considered include CPU utilization, CPU speed, free memory, and network latency (ping). By assigning scores based on these factors, the load balancing algorithm optimizes resource utilization and ensures that the resource sharing load is distributed evenly across the network.

## Implementation Details

### System Setup

The system assumes the following conditions have been met:

- The hostnames of all devices in the network are known.
- There is a root device where the main interface runs.
- A configuration file contains the names of each host.
- The storage resources have been configured and made available.
- MPI version 3 is installed on each device.
- Passwordless connection has been established between devices using RSA keys.
- The username logged in on each device is the same (required for NFS).

The workflow of the resource sharing system can be described as follows:

1. Get the command from the user.
2. Check if the option is '?' or '!'.
   - If it is '?':
     1. Gather network statistics.
     2. Generate scores for each device.
     3. If specific hosts are mentioned, pad the scores on the right for those hosts.
     4. Identify the device with the highest score.
3. Get the actual command to be run.
4. Send the command to each device using MPI.
5. Run the command on each device.
6. Repeat the above steps in a loop until the desired condition is met.
7. End the workflow.

In this workflow, the user provides a command to the system. If the command starts with '?', the system gathers network statistics, generates scores for each device based on the statistics, and identifies the device with the highest score. If specific hosts are mentioned, their scores are padded on the right to align with other devices. Afterward, the actual command to be run is obtained, and it is sent to each device using MPI. Finally, the command is executed on each device. This loop continues until the desired condition is met, and the workflow ends.

### Load Balancing Algorithm - Scoring

The load balancing algorithm calculates the score for each device using the following formula:

Score = α x (100 - CPU_util) x Max_CPU + β x Free_Mem + γ x Ping

- CPU

_util: The percentage utilization of the CPU in the device.
- Max_CPU: The maximum CPU clock rate in GHz.
- Free_Mem: The current amount of free memory in GB.
- Ping: The average round-trip time (RTT) in milliseconds.
- α, β, γ: Factors determining the importance of each unit in the score calculation.

### Evaluation

To evaluate the effectiveness of the resource sharing system, we conducted experiments using two benchmark programs: Hanoi Standard Code and Fstime. These benchmarks were chosen to test different types of programs and assess the system's performance under varying workloads.

The Hanoi Standard Code was executed with 10 stacks and ran for 5 seconds. The resource sharing system correctly identified the device with the highest score, indicating that the load balancing algorithm effectively selected the appropriate device.

Similarly, the Fstime program, which calculates the total time taken for file reading, writing, and copying, was executed. The resource sharing system correctly identified the device with the best performance for this program.

However, it was observed that in some cases, the resource sharing system chose a device different from the one with the best performance. This discrepancy can be attributed to the system not considering certain factors, such as storage read/write speeds, in its load balancing algorithm. Future improvements should incorporate these additional statistics to enhance the system's decision-making process.

## Conclusion

The resource sharing system implemented using MPI package in C++ with a load balancing algorithm shows promising results in efficiently distributing hardware resources among multiple unique devices in a distributed system. By leveraging MPI's communication capabilities and NFS for resource sharing, the system optimizes resource utilization and balances the load across the network. Further enhancements can be made by considering additional statistics and benchmarking different types of programs.

## Thank you
