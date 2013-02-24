#pragma once

// C++ includes
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <random>
#include <chrono>
#include <vector>

// C includes
#include <math.h>

//BLINK Logservice
#include "logservice.hpp"

//BLINK includes
#include "htm.hpp"

using namespace ICoDF;
using namespace ICoDF_HTM;

namespace ICoDF_HTM
{
	class HTM;
	class HTMAsciiParser //: public IHTMParser
	{
	public:
		bool Parse(std::string& filename);
		unsigned int getNbObj(void) const;
        std::vector<std::pair<double, double>> const & getObjects(void) const;
		
		// DEFAULT CTOR
		HTMAsciiParser();
		
		// DEFAULT DTOR
		~HTMAsciiParser(void);
        
        double getMinRa()  const { return _minRa;  }
        double getMinDec() const { return _minDec; }
        double getMaxRa()  const { return _maxRa;  }
        double getMaxDec() const { return _maxDec; }
	private:
	  unsigned int                           nbObj;
	  double                                 _minRa;
	  double                                 _minDec;
	  double                                 _maxRa;
	  double                                 _maxDec;
	  std::vector<std::pair<double, double>> _objects;
	};
}
