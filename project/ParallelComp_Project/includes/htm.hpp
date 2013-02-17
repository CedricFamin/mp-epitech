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
#include "htmconstraint.hpp"

using namespace ICoDF;
using namespace ICoDF_HTM;

namespace ICoDF_HTM
{
	class HTMAsciiParser;
	
	class HTM
	{
	private:
		Octahedron_t* _octahedron;				//< base HTM Octahedron
        
		std::queue<PointInfo_t*> _pointList;		//< List of points you are working with
        std::vector<PointInfo_t*> _pointsToCompute; //< List of points for TwoPointsCorrelation
		
		std::ofstream stream;				//< Output stream to write HTM description
		
	public:
        /// DEFAULT CTOR
		HTM(void);
		
        /// DEFAULT DTOR
		~HTM(void);
		
        /// Add a new point to the working list
		void AddPoint(const double& ra, const double& dec);
		
        /// Launch the creation of the HTM using the current working list
		bool CreateHTM(void);
		
        /// Assign a point (single operation) to the HTM
		void AssignPoint(PointInfo_t *pt);
		
        /// Load points from a file
		void LoadCatalog(std::string& file);
		
        /// Create the base octahedron
		void CreateOctahedron(void);
		
        /// Delete the base Octahedron
		void DeleteOctahedron(void);
		
        /// Get number of pairs a the radius -+ delta from the given object
		unsigned int TwoPointsCorrelation(double &radius, double &delta);
		
        // Cedric: this method was removed from the parser
		void UniformNumberGenerator(unsigned int const nbObject, double const raMin, double const raMax, double const decMin, double const decMax);
		void GeneratePoint(std::vector<std::pair<double, double>> const & parPointsDefinition);
        void LogIntoFile(std::string const & parFilename);
	private:
		
        ///
		bool SelectRootTrixel(PointInfo_t* pt);
		
        ///
		inline std::pair<double, double> CalcCoordPoint(std::pair<double, double>& a, std::pair<double, double>& b);
		
        /// Check if a point is in a triangle describe by the given boundaries
		bool PointInTriangle(const double& ra, const double& dec, double* boundaries);
		
        /// send all trixels from the given one into the given output stream
		void Display(trixel_t* current, std::ofstream& fstream);
		
        /// Free all trixels
		void FreeAllTrixels(trixel_t* current);
		
        /// BOUNDARIES
        /// Double[4] = decMin, decMax, raMin, raMax
		double* ComputeRootTrixelBounds(trixel_t* trixel);
		
        /// Compute new bounds of a trixel's child from it's parent bounds and index
		double* ComputeTrixelBounds(const double* fatherBounds, unsigned int& index, bool& reverse);
		
        /// get the index an point using the parent trixel bounds
		unsigned int getIndex(double* boundaries, bool& reverse, const double& ra, const double& dec);
	};
}
