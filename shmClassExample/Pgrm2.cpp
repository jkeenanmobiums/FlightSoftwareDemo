/* 
 * File:   Pgrm2.cpp
 * Author: jeremylim
 *
 * Created on September 23, 2015, 9:17 PM
 * This is the example client for the IPC example.
 */

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "CTopicPortal.h"
#include "2DPoint.h"

static volatile bool isRunning = true;

void exitSignal(int signum)
{
    //This allows us to exit the infinite loop.
    //std::cout << "SIGINT DETECTED. GRACEFULL CLEANUP." << std::endl;
    isRunning = false;
}

int main(int argc, char** argv) 
{
    struct sigaction exitCall;
    exitCall.sa_handler = exitSignal;
    sigaction(SIGINT, &exitCall, NULL);

    //hardcoded key file
    //string fPath = "/derp";
    int memkey = 444;
    //hardcoded semaphore name
    std::string semName = "DaSemaphore";
    //all processes using this shared memory must know
    //the type and the size
    CTopicPortal<struct TwoDPoint> readPlace(memkey, semName, 1);
    
    struct timespec t;
    //we sleep for 1 second at a time.
    t.tv_sec = 2;
    //t.tv_nsec = 500000000;
    //clock_gettime(CLOCK_MONOTONIC, &t);

    struct TwoDPoint readval;
    
    //loop, this will be the client
    while(isRunning)
    {
        //read and print each value
        int valx;
        int valy;
        readPlace.readTopic(&readval,0);
        valx = readval.x;
        valy = readval.y;
        std::cout << "x: " << valx << " y: " << valy << "." << std::endl;

        nanosleep(&t,NULL);
    }
    
    return 0;
}


