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
		void Parse(std::string& filename);
		unsigned int getNbObj(void) const;
        std::vector<std::pair<double, double>> const & getObjects(void) const;
		
		// DEFAULT CTOR
		HTMAsciiParser(HTM * parHtm);
		
		// DEFAULT DTOR
		~HTMAsciiParser(void);
	private:
		unsigned int                           nbObj;
		std::vector<std::pair<double, double>> _objects;
	};
}
