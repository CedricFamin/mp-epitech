//
//  Program.cpp
//  LandySzalayEstimator
//
//  Created by Cédric Famin on 01/02/13.
//
//

#include <mutex>
#include <thread>

#include "../includes/Program.h"
#include "../includes/htm.hpp"
#include "../includes/WorkerThread.h"
#include "../includes/ThreadManager.hpp"

using namespace ICoDF;
using namespace ICoDF_HTM;

Program::Program()
: _logQueue(0)
, _htm(0)
, _parser(0)
, _raMin(.0)
, _raMax(.0)
, _decMin(.0)
, _decMax(.0)
{
    _logQueue = LogService::GetInstance()->CreateNewLogQueue();
    this->Log(LogService::NOTICE, "Create");
}

Program::~Program()
{
    
    this->Log(LogService::NOTICE, "Delete");
}

void Program::Init(ProgramConfig const & config)
{
    this->Log(LogService::NOTICE, "Init");
    _config = config;
    
    _htm = new HTM();
    _parser = new HTMAsciiParser();
    this->Log(LogService::NOTICE, "Computing Normal Catalog...");
    
    _htm->CreateOctahedron();
    _parser->Parse(this->_config.filename);
    _htm->GeneratePoint(_parser->getObjects());
    _htm->CreateHTM();
    
    this->Log(LogService::NOTICE, "HTM Created for Normal Catalog");
    
    _raMin = _parser->getMinRa();
    _raMax = _parser->getMaxRa();
    _decMin = _parser->getMinDec();
    _decMax = _parser->getMaxDec();
    
    std::stringstream tmp;
    tmp << "Computing Mean values for Random and Hybrid Catalog on " << this->_config.loop << " loops using " << _parser->getNbObj() << " random objects...";
    this->Log(LogService::NOTICE, tmp.str());
}

void Program::Clean()
{
    this->Log(LogService::NOTICE, "Clean");
    delete _parser;
    _parser = 0;
    _raMin = .0;
    _raMax = .0;
    _decMin = .0;
    _decMax = .0;
}

void Program::Launch()
{
    this->Log(LogService::NOTICE, "Launch");
    
    unsigned int nn = _htm->TwoPointsCorrelation(this->_config.radius, this->_config.delta);
    std::stringstream tmp;
    tmp << "Two Point Correlation have been computed for the Normal Catalog [" << nn << "] pairs";
    this->Log(LogService::NOTICE, tmp.str());
    _htm->DeleteOctahedron();
    delete _htm;
    _htm = 0;
    
    unsigned int rr = 0;
    unsigned int nr = 0;
    
    std::vector<std::pair<std::thread*, ExecutionQueue*>> execQueue;
    std::vector<WorkerThread*> workerList;
    unsigned int nbThread=  std::thread::hardware_concurrency();
    for (int i = 0; i < nbThread; ++i)
    {
        execQueue.push_back(std::make_pair((std::thread*)0, new ExecutionQueue()));
    }
    
    for (int i = 0; i < this->_config.loop; ++i)
    {
        WorkerThread * worker = new WorkerThread();
        worker->Init(_config, _parser->getObjects(), _raMin, _raMax, _decMin, _decMax);
        execQueue[i%nbThread].second->AddCall(worker);
        workerList.push_back(worker);
        if (i == 0)
            worker->SetShouldLogHTM(true);
    }
    
    LogService::GetInstance()->StartLogService();
    for (int i = 0; i < nbThread; ++i)
    {
        execQueue[i].first = new std::thread(&ExecutionQueue::Run, execQueue[i].second);
    }
    
    for (std::pair<std::thread*, ExecutionQueue*> q : execQueue)
    {
        tmp.str("");
        tmp << "Join the thread" << q.first->get_id();
        this->Log(LogService::NOTICE, tmp.str());
        
        if (q.first->joinable())
            q.first->join();
        
        delete q.first;
        delete q.second;
    }
    
    unsigned int i = 0;
    
    for (WorkerThread* worker : workerList)
    {
        worker->Clean();
        
        unsigned int currentRR = worker->GetRR();
        rr += currentRR;
        tmp.str("");
        tmp << "Two POint Correlation have been computed for the Random Catalog [" << currentRR << "] mean [" << (rr / (i + 1)) << "]";
        
        this->Log(LogService::NOTICE, tmp.str());
        
        unsigned int currentNR = worker->GetNR();
        nr += currentNR;
        tmp.str("");
        tmp << "Two POint Correlation have been computed for the Hybrid Catalog [" << currentNR << "] mean [" << (nr / (i + 1)) << "]";
        this->Log(LogService::NOTICE, tmp.str());
        delete worker;
        
        ++i;
    }
    
    
    this->Log(LogService::NOTICE, "...Done !");
    
    rr /= this->_config.loop;
    nr /= this->_config.loop;
    double estimator = 0;
    estimator = 2 * nr;
    estimator = nn - estimator;
    estimator = estimator - rr;
    estimator /= rr;
    tmp.str("");
    tmp << "Landy Szalay Estimator for current catalog returns : " << estimator;
    this->Log(LogService::NOTICE, tmp.str());
}


void Program::Log(unsigned int level, std::string const & message)
{
    _logQueue->AddLogMessage(level, "Program", message);
}