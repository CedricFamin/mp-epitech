#ifndef THREADMANAGER_H_
# define THREADMANAGER_H_

#include <list>
#include <vector>
#include <queue>
#include <atomic>
#include <mutex>
#include "WorkerThread.h"

class ExecutionQueue
{
public:
    ExecutionQueue()  {}
    ~ExecutionQueue() {}
    
    void Run();
    void AddCall(WorkerThread* worker);
    
protected:
private:
    std::vector<WorkerThread*> _exec;
};

#endif