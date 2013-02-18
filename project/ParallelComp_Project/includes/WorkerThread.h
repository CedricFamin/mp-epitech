#ifndef WORKERTHREAD_H_
# define WORKERTHREAD_H_

#include "htm.hpp"
#include "Program.h"
#include "logservice.hpp"

class WorkerThread
{
public:
    WorkerThread() : _shouldLogHTM(false) { _logQueue = ICoDF::LogService::GetInstance()->CreateNewLogQueue(); }
    ~WorkerThread() {}
    
    void Init(ProgramConfig & config, std::vector<std::pair<double, double>> const & objects,
              double raMin, double raMax, double decMin, double decMax);
    void Launch();
    void Clean();
    
    double GetRR() const { return _rr; }
    double GetNR() const { return _nr; }
    void SetShouldLogHTM(bool parValue) { _shouldLogHTM = parValue; }
    
protected:
private:
    HTM * _htm;
    ProgramConfig _programConfig;
    std::vector<std::pair<double, double>> _objects;
    double _rr;
    double _nr;
    double _raMin;
    double _raMax;
    double _decMin;
    double _decMax;
    bool   _shouldLogHTM;
    
    ICoDF::LogQueue * _logQueue;
};


#endif