//
//  Program.h
//  LandySzalayEstimator
//
//  Created by Cédric Famin on 01/02/13.
//
//

#ifndef LandySzalayEstimator_Program_h
#define LandySzalayEstimator_Program_h

#include "logservice.hpp"

namespace ICoDF_HTM
{
    class HTM;
    class HTMAsciiParser;
}

struct ProgramConfig
{
    double radius;
    double delta;
    unsigned int loop;
    std::string filename;
};

class Program
{
public:
    Program();
    ~Program();
    
    void Init(ProgramConfig const & config);
    void Clean();
    void Launch();
    
protected:
    void Log(unsigned int level, std::string const & message);
private:
    ICoDF::LogService           _logService;
    ProgramConfig               _config;
    
    ICoDF_HTM::HTM *            _htm;
    ICoDF_HTM::HTMAsciiParser * _parser;
    
    double _raMin;
    double _raMax;
    double _decMin;
    double _decMax;
};


#endif
