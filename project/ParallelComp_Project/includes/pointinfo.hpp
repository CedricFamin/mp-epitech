#pragma once

// Eigen Includes
#include <Eigen/Dense>

#include "Vector.hpp"

namespace ICoDF_HTM
{
    struct trixel_s;
    
    typedef struct PointInfo_s
    {
        double           _ra;		// Right ascension
        double           _dec;		// Declination
        Vector3d         _position; // Position in Vector3d
        struct trixel_s* _current;  // Current position in HTM
    } PointInfo_t;
}
