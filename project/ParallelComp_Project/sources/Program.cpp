//
//  Program.cpp
//  LandySzalayEstimator
//
//  Created by Cédric Famin on 01/02/13.
//
//

#include <thread>

#include "../includes/Program.h"
#include "../includes/htm.hpp"
#include "../includes/WorkerThread.h"

using namespace ICoDF;
using namespace ICoDF_HTM;

Program::Program()
: _logService()
, _htm(0)
, _parser(0)
, _raMin(.0)
, _raMax(.0)
, _decMin(.0)
, _decMax(.0)
{
    
    _logService.SetConfiguration(LogService::LS_PRINT_ON_COUT);
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
    _parser = new HTMAsciiParser(_htm);
    this->Log(LogService::NOTICE, "Computing Normal Catalog...");
    
    _htm->CreateOctahedron();
    _parser->Parse(this->_config.filename);
    _htm->GeneratePoint(_parser->getObjects());
    _htm->CreateHTM();
    
    this->Log(LogService::NOTICE, "HTM Created for Normal Catalog");
    std::stringstream tmp;
    
    _raMin = _htm->getMinRa();
    _raMax = _htm->getMaxRa();
    _decMin = _htm->getMinDec();
    _decMax = _htm->getMaxDec();
    tmp.str("");
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
    
    std::stringstream tmp;
    unsigned int nn = _htm->TwoPointsCorrelation(this->_config.radius, this->_config.delta);
    tmp.str("");
    tmp << "Two Point Correlation have been computed for the Normal Catalog [" << nn << "] pairs";
    this->Log(LogService::NOTICE, tmp.str());
    
    unsigned int rr = 0;
    unsigned int nr = 0;
    
    _htm->DeleteOctahedron();
    delete _htm;
    _htm = 0;
    
    std::vector<std::pair<std::thread*, WorkerThread*>> workerList;
    for (int i = 0; i < this->_config.loop; ++i)
    {
        WorkerThread * worker = new WorkerThread();
        worker->Init(_config, _parser->getObjects(), _raMin, _raMax, _decMin, _decMax);
        
        std::thread * thread = new std::thread(&WorkerThread::Launch, worker);
        workerList.push_back(std::make_pair(thread, worker));
    }
    
    unsigned int i = 0;
    for (std::pair<std::thread*, WorkerThread*> & worker : workerList)
    {
        tmp.str("");
        tmp << "Join the thread" << worker.first->get_id();
        this->Log(LogService::NOTICE, tmp.str());
        
        if (worker.first->joinable())
            worker.first->join();
        
        this->Log(LogService::NOTICE, "Done.");
        worker.second->Clean();
        
        unsigned int currentRR = worker.second->GetRR();
        rr += currentRR;
        tmp.str("");
        tmp << "Two POint Correlation have been computed for the Random Catalog [" << currentRR << "] mean [" << (rr / (i + 1)) << "]";
        
        this->Log(LogService::NOTICE, tmp.str());
        
        unsigned int currentNR = worker.second->GetNR();
        nr += currentNR;
        tmp.str("");
        tmp << "Two POint Correlation have been computed for the Hybrid Catalog [" << currentNR << "] mean [" << (nr / (i + 1)) << "]";
        this->Log(LogService::NOTICE, tmp.str());
        delete worker.first;
        delete worker.second;
        
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
    LS_ADDMSG(level, "Program", message);
    //_logService.AddMessage(level, "Program", message);
}