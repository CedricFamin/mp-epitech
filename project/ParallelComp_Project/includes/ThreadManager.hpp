#ifndef THREADMANAGER_H_
# define THREADMANAGER_H_

#include <list>
#include <vector>
#include <queue>
#include <atomic>

class ThreadLauncher
{
public:
    ThreadLauncher() {}
    ~ThreadLauncher() {}
    
    void Run() {}
protected:
private:
}

class ThreadManager
{
public:
    ThreadManager();
    ~ThreadManager();
    
    void AddDeferredCall(ThreadLauncher & thread);
    void JoinAllThread();
    void SetNbSimultaneousThead(unsigned int nb);
    void LaunchThreadIFN();
    
    static void IncrementThreadCounter();
    static void DeccrementThreadCounter();
protected:
private:
    std::queue<ThreadLauncher*> _threadsToLaunch;
    unsigned int                _nbSimultaneousThread;
    
    std::list<ThreadManager*>   _threadManagers;
    static std::atomic__uchar   _nbThreadLaunched;
};

#endif