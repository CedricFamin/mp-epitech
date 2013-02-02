//
//  Program.cpp
//  LandySzalayEstimator
//
//  Created by Cédric Famin on 01/02/13.
//
//

#include "../includes/Program.h"
#include "../includes/htm.hpp"

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
    delete _htm;
    _htm = 0;
    delete _parser;
    _parser = 0;
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
    for (int i = 0; i != this->_config.loop; ++i)
    {
        tmp.str("");
        tmp << "Computing loop " << i + 1 << " on " << this->_config.loop;
        this->Log(LogService::NOTICE, tmp.str());
        _htm->DeleteOctahedron();
        _htm->CreateOctahedron();
        _htm->UniformNumberGenerator(_parser->getNbObj(), _raMin, _raMax, _decMin, _decMax);
        _htm->CreateHTM();
        
        unsigned int currentRR = _htm->TwoPointsCorrelation(this->_config.radius, this->_config.delta);
        rr += currentRR;
        tmp.str("");
        tmp << "Two POint Correlation have been computed for the Random Catalog [" << currentRR << "] mean [" << (rr / (i + 1)) << "]";
        
        this->Log(LogService::NOTICE, tmp.str());
        
        _htm->GeneratePoint(_parser->getObjects());
        _htm->CreateHTM();
        unsigned int currentNR = _htm->TwoPointsCorrelation(this->_config.radius, this->_config.delta);
        nr += currentNR;
        tmp.str("");
        tmp << "Two POint Correlation have been computed for the Hybrid Catalog [" << currentNR << "] mean [" << (nr / (i + 1)) << "]";
        this->Log(LogService::NOTICE, tmp.str());
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
    _logService.AddMessage(level, "Program", message);
}