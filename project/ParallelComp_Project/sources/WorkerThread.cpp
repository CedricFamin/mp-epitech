#include "../includes/WorkerThread.h"

void WorkerThread::Init(ProgramConfig & config, std::vector<std::pair<double, double>> const & objects,
          double raMin, double raMax, double decMin, double decMax)
{
    _programConfig = config;
    _objects = objects;
    _raMin = raMin;
    _raMax = raMax;
    _decMin = decMin;
    _decMax = decMax;
}

void WorkerThread::Launch()
{
    std::stringstream tmp;
    _htm = new HTM();
    
    _htm->CreateOctahedron();
    _htm->UniformNumberGenerator(_objects.size(), _raMin, _raMax, _decMin, _decMax);
    _htm->CreateHTM();
    _rr = _htm->TwoPointsCorrelation(_programConfig.radius, _programConfig.delta);
    
    tmp << "Two POint Correlation have been computed for the Random Catalog [" << _rr << "]";
    LS_ADDMSG(LogService::NOTICE, "Worker", tmp.str());
    
    _htm->GeneratePoint(_objects);
    _htm->CreateHTM();
    _nr= _htm->TwoPointsCorrelation(_programConfig.radius, _programConfig.delta);
    
    tmp.str("");
    tmp << "Two POint Correlation have been computed for the Hybrid Catalog [" << _nr << "]";
    LS_ADDMSG(LogService::NOTICE, "Worker", tmp.str());
    
    
    _htm->DeleteOctahedron();
    
    delete _htm;
    _htm = 0;
}

void WorkerThread::Clean()
{
    
}