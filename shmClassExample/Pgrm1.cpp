/* 
 * File:   Pgrm1.cpp
 * Author: jeremylim
 *
 * Created on September 23, 2015, 9:17 PM
 * This is the server process for the shared memory
 * IPC example.
 *
 * Also includes a quick example of graceful shutdown using
 * SIGINT(Control-C)
 */

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "CTopicPortal.h"
#include "2DPoint.h"

static volatile bool isRunning = true;

void callbackExit(int signum)
{

    //This will clean us up by allowing us to exit the infinit loop.
    //std::cout << "SIGINT DETECTED. GRACEFULL SHUTDOWN." << std::endl;
    isRunning = false;
}

int main(int argc, char** argv) 
{

    //register our SIGINT handler.
    struct sigaction callback;
    callback.sa_handler = callbackExit;
    sigaction(SIGINT, &callback, NULL);
    
    //hardcoded key file
    //string fPath = "/derp";
    int memkey = 444;
    //hardcoded semaphore name
    std::string semName = "DaSemaphore";
    //the shared memory segment has a size of 1.
    //all processes using it must know it.
    CTopicPortal<struct TwoDPoint> topic1(memkey, semName, 1);
    //QuickClass test;
    
    struct timespec t;
    //we sleep for 1 second at a time.
    t.tv_sec = 1;
    //clock_gettime(CLOCK_MONOTONIC, &t);
    int counter = 0;
    struct TwoDPoint place;
    place.y = 0;
    place.x = 0;

    //crappy timing code.
    while(isRunning)
    {
        place.x += 1;
        place.y += 2;
        //write to the shared memory.
        topic1.writeTopic(&place,0);
        
        //delay a bit.
        nanosleep(&t,NULL);
    }
    return 0;
}

