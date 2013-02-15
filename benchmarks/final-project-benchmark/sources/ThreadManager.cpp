#include "../includes/ThreadManager.hpp"

void ExecutionQueue::Run()
{
    for (WorkerThread * work : _exec)
    {
        work->Launch();
    }
}

void ExecutionQueue::AddCall(WorkerThread* worker)
{
    _exec.push_back(worker);
}

