/* 
 * File:   CTopicPortal.h
 * Author: jeremylim
 *
 * Created on September 9, 2015, 2:41 PM
 * This is an example class that takes care of concurrency &
 * memory cleanup issues when using shared memory.
 *
 * NOTE: Work in Progress: capability is limited for this example
 *
 * Why is everything in the .h? See the README.
 */


#ifndef CTOPICPORTAL_H
#define	CTOPICPORTAL_H

#include <cstdlib>
//#include <sys/sem.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <cstring>
#include <string>
#include <cstring>
#include <stdio.h>

//using namespace std;

//T will be the type of the struct we send.
//template <typename T>
template <typename T> class CTopicPortal
{
public:
    //constants
    //this will probably change.
    const static int segmentSize = 1024;
    
    CTopicPortal();
    ~CTopicPortal();
    
    //CTopicPortal(std::string inTopicPath, std::string inTopName, int inSize);
    CTopicPortal(int setID, std::string inTopName, int inSize);
    
    //int setTopicPath(std::string inTopicPath);
    
    int getSize();//# of messages able to be stored.
    
    //the shared memory segment is treated as a static array.
    int writeTopic(T *inMsg, int index);
    //nice function will prevent out-of bounds access.
    int readTopic(T *outMsg, int index);
    
private:
    std::string m_intTopicPath;
    std::string m_semaphoreName;
    key_t m_memKey;
    int m_id;
    void * m_fd;
    int m_messagesSize;
    //we need to grab a semaphore as well (coordinating memory access)
    //key_t m_semKey;
    sem_t * m_semaphore;
};

//instantiations...

//#include "CTopicPortal.cpp"
template <typename T>
CTopicPortal<T>::CTopicPortal()
{
    
}

/*
template <typename T>
CTopicPortal<T>::CTopicPortal(std::string inFilePath, std::string inTopName, int inSize)
{
    int q = sizeof(T);
    
    m_messagesSize = inSize;
    
    //error checking required for each step!
    m_memKey = ftok(inFilePath.c_str(),'M');
    
    m_semaphoreName = inTopName;
    //okay, attempt to make/seize semaphores
    m_semaphore = sem_open(inTopName.c_str(), O_CREAT | O_EXCL, 0666, 0);
    
    if(m_semaphore == SEM_FAILED)
    {
        //it already exists, simply connect to it.
        m_semaphore = sem_open(inTopName.c_str(), 0);
        if(m_semaphore == SEM_FAILED)
        {
            //failed again.
            //printf("OW NOES. SEMAPHORE FAILED.");
        }
    }
    else
    {
        //unlock it; we're making the segment.
        sem_post(m_semaphore);
    }
    
    sem_wait(m_semaphore);
    
    //make the segment
    m_id = shmget(m_memKey, q*inSize, 0666 | IPC_CREAT);
    //attach to it.
    m_fd = shmat(m_id, (void *) 0, 0);//read and write to this segment.
    if(m_fd == -1)
    {
        //printf("OW NOES. SHMAT FAILED.");
    }
    
    sem_post(m_semaphore);
}
*/

template <typename T>
CTopicPortal<T>::CTopicPortal(int setID, std::string inTopName, int inSize)
{
    int q = sizeof(T);
    
    m_messagesSize =  inSize;
    
    //In this case, we dumbly set the key directly.
    m_memKey = setID;//ftok(inFilePath.c_str(),'M');
    
    m_semaphoreName = inTopName;
    
    //okay, attempt to make/seize semaphores
    m_semaphore = sem_open(inTopName.c_str(), O_CREAT | O_EXCL, 0666, 0);
    
    if(m_semaphore == SEM_FAILED)
    {
        //it already exists, simply connect to it.
        m_semaphore = sem_open(inTopName.c_str(), 0);
        if(m_semaphore == SEM_FAILED)
        {
            //failed again.
            //printf("OW NOES. SEMAPHORE FAILED.");
            return;
        }
    }
    else
    {
        //unlock it; we're making the segment.
        sem_post(m_semaphore);
    }
    
    sem_wait(m_semaphore);
    
    //make the segment
    m_id = shmget(m_memKey, q*inSize, 0666 | IPC_CREAT);
    //attach to it.
    m_fd = shmat(m_id, (void *) 0, 0);//read and write to this segment.
    if(m_fd == (char *) -1)
    {
        //printf("OW NOES. SHMAT FAILED.");
        return;
    }
    
    sem_post(m_semaphore);
}

template <typename T>
CTopicPortal<T>::~CTopicPortal()
{
    
    sem_wait(m_semaphore);
    //see how many attached processes there are
    //if there is only one, we gotta clean up.
    struct shmid_ds stats;
    shmctl(m_id, IPC_STAT, &stats);
    
    if(stats.shm_nattch == 1)
    {
        //mark for removal
        if(shmctl(m_id, IPC_RMID, &stats) == -1)
        {
             //printf("OW NOES. SHMCTL DIED!");
        }
        
        //detach
        if(shmdt(m_fd) == -1)
        {
            //printf("OW NOES. DIDN'T DETACH PROPERLY!");
        }
        //we gotta kill the semaphore too.
        sem_post(m_semaphore);
        sem_unlink(m_semaphoreName.c_str());
    }
    else
    {
        //detach
        if(shmdt(m_fd) == -1)
        {
            //printf("OW NOES. DIDN'T DETACH PROPERLY!");
        }
        sem_post(m_semaphore);
    }
}

template <typename T>
int CTopicPortal<T>::writeTopic(T *inMsg, int index)
{
    //seize semaphore
    sem_wait(m_semaphore);
    int q = sizeof(T);
    //might work...
    if( index < 0 || index >= m_messagesSize)
    {
        printf("out of bounds. error.");
    }
    else
    {
        //m_fd + (index * q)
        memcpy(((T *)m_fd) + index, inMsg, q);
    }
    //release semaphore
    sem_post(m_semaphore);
    
    return 0;
}


template <typename T>
int CTopicPortal<T>::readTopic(T *outMsg, int index)
{
    //seize semaphore
    sem_wait(m_semaphore);
    int q = sizeof(T);
    //might work...
    if( index < 0 || index >= m_messagesSize)
    {
        printf("out of bounds. error.");
    }
    else
    {
        memcpy(outMsg, (((T *)m_fd) + index), q);
    }
    //release semaphore
    sem_post(m_semaphore);
    
    return 0;
}

#endif	/* CTOPICPORTAL_H */

