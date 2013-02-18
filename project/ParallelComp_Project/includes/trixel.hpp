#ifndef __BLINK_HTM_TRIXEL_HPP__
#define __BLINK_HTM_TRIXEL_HPP__

// C Includes
#include <assert.h>

// C++ Includes
#include <string>
#include <sstream>

// Eigen Includes
#include <Eigen/Dense>

// ICoDF
#include "logservice.hpp"
#include "pointinfo.hpp"
#include "Vector.hpp"

using namespace ICoDF;
using namespace ICoDF_HTM;

namespace ICoDF_HTM
{
    struct PointInfo_s;
    
    /// Structure that define a trixel (htm base object)
    typedef struct trixel_s
    {
        trixel_s()
        : _midPointsComputed(false)
        {
            _midPoints[0] = Vector3d{0, 0, 0};
            _midPoints[1] = Vector3d{0, 0, 0};
            _midPoints[2] = Vector3d{0, 0, 0};
            _trixelBoundary = {0, 0, 0};
            _phi = 0.0;
        }
        
        struct trixel_s**			_children;	//< trixel's subtrixels
        Vector3d			        _vertices[3];	//< Trixel's vertices
        Vector3d                    _midPoints[3];
        Vector3d                    _cross01;
        Vector3d                    _cross12;
        Vector3d                    _cross20;
        Vector3d                    _trixelBoundary;
        
        double _phi;
        
        bool                        _midPointsComputed;
        bool				        _reverse;	//< is an upside-down trixel ?
        std::string				    _HTMId;		//< N10120112121101
        unsigned int			    _nbChildObject; //< Number of objects contained in this trixel.
        struct PointInfo_s*			_info;		//< Point information structure for the actual
    } trixel_t;
    
    /// Delte the given trixel (but not the PointInfo_t)
    void ClearTrixel(trixel_t* trixel);
    
    /// Delete a trixel's children
    void ClearTrixelChildren(trixel_t *parent);
    
    /// Compute a trixel's midpoint vectors
    void ComputeTrixelMidpoints(trixel_t* trixel, Vector3d* outMidPoint);
    
    /// Create A Root Trixel
    trixel_t* CreateRootTrixel(std::string HTMId);
    
    /// Create a new trixel from its parent information and index
    trixel_t* CreateTrixelChild(trixel_t *parent, unsigned short int& index);
    
    /// Create a container for the 4 trixels that correspond to a new level
    trixel_t** CreateTrixelChildren(trixel_t* parent);
    
    /// Return which subtrixel the point is (vector version)
    unsigned short int GetIndex(trixel_t* trixel, Vector3d& pointVector);
    
    /// Return which subtrixel the point is (object version)
    unsigned short int GetIndex(trixel_t* trixel, PointInfo_t* object);
    
    /// Init a new trixel with default values
    void InitTrixel(trixel_t* trixel);
    
    /// Check if the given right ascension value is correct
    bool IsCorrectRA(double const ra);
    
    /// Check if the given declination value is correct
    bool IsCorrectDEC(double const dec);
}

#endif /* !__BLINK_HTM_TRIXEL_HPP__ */
