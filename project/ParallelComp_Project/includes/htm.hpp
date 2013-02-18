#pragma once

// C++ includes
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <map>
#include <queue>
#include <unordered_map>

// C includes
#include <math.h>
#include <time.h>
#include <stdlib.h>

// EIGEN INCLUDES
#include <Eigen/Dense>

// BLINK includes
#include "logservice.hpp"
#include "trixel.hpp"
#include "octahedron.hpp"
#include "htmasciiparser.hpp"
#include "pointinfo.hpp"

using namespace ICoDF;
using namespace ICoDF_HTM;

namespace ICoDF_HTM
{
	class HTMAsciiParser;
	
	class HTM
	{
	private:
		Octahedron_t*             _octahedron;
		std::queue<PointInfo_t*>  _pointList;
        std::vector<PointInfo_t*> _pointsToCompute;
        
	public:
        HTM(void);
        ~HTM(void);
        void CreateOctahedron(void);
		void DeleteOctahedron(void);
		bool CreateHTM(void);
		
		void AddPoint(const double& ra, const double& dec);
		void AssignPoint(PointInfo_t *pt);
		unsigned int TwoPointsCorrelation(double &radius, double &delta);
		
        void UniformNumberGenerator(unsigned int const nbObject, double const raMin, double const raMax, double const decMin, double const decMax);
		void GeneratePoint(std::vector<std::pair<double, double>> const & parPointsDefinition);
        void LogIntoFile(std::string const & parFilename);
	private:
		bool SelectRootTrixel(PointInfo_t* pt);
		void Display(trixel_t* current, std::ofstream& fstream);
		void FreeAllTrixels(trixel_t* current);
	};
}
