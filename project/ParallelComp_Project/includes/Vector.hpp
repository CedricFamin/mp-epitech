#ifndef __Vector_HPP__
# define __Vector_HPP__

# include <cmath>

namespace SIMD {
    
    class Vector3f
    {
    private:
        union
        {
            __m128d _xy;
            struct
            {
                double _x;
                double _y;
            };
        };
        
        union
        {
            __m128 _z0;
            struct
            {
                double _z;
                double _0;
            };
        }
        
    public:
        Vector3f(double x, double y, double z)
        : _x(x)
        , _y(y)
        , _z(z)
        , _0(0)
        {
        }
        
        inline Vector3f operator-(const Vector3d& v) const
        {
            return Vector3d(this->_x - v._x, this->_y - v._y, this->_z - v._z);
        }
        
        inline Vector3f operator/(const float& nb) const
        {
            return Vector3f(this->_x / nb, this->_y / nb, this->_z / nb);
        }
        
        inline Vector3f operator+(const Vector3d& v) const
        {
            return Vector3f(this->_x + v._x, this->_y + v._y, this->_z + v._z);
        }
        
        inline Vector3f cross(const Vector3d& v) const
        {
            return Vector3f(this->_y * v._z - this->_z * v._y, this->_z * v._x - this->_x * v._z, this->_x * v._y - this->_y * v._x);
        }
        
        //inline float dot(float const& x, float const& y, float const& z) const
        //{
        //    return (this->_x * x + this->_y * y + this->_z * z);
        //}
        
        inline float dot(const Vector3d& v) const
        {
            return (this->_x * v._x + this->_y * v._y + this->_z * v._z);
        }
        
        inline float norm(void) const
        {
            return (std::sqrt(this->_x * this->_x + this->_y * this->_y + this->_z * this->_z));
        }
    }
}

struct Vector3d
{
    union
    {
        __m128 _xyz0;
        struct
        {
            float _x;
            float _y;
            float _z;
            float _0;
        };
    };

    inline Vector3d operator-(const Vector3d& v) const
    {
        return Vector3d{{this->_x - v._x, this->_y - v._y, this->_z - v._z}};
    }
    
    inline Vector3d operator/(const float& nb) const
    {
        return Vector3d{{this->_x / nb, this->_y / nb, this->_z / nb}};
    }
    
    inline Vector3d operator+(const Vector3d& v) const
    {
        return Vector3d{{this->_x + v._x, this->_y + v._y, this->_z + v._z}};
    }
    
    inline Vector3d cross(const Vector3d& v) const
    {
        return Vector3d{{this->_y * v._z - this->_z * v._y, this->_z * v._x - this->_x * v._z, this->_x * v._y - this->_y * v._x}};
    }
    
    //inline float dot(float const& x, float const& y, float const& z) const
    //{
    //    return (this->_x * x + this->_y * y + this->_z * z);
    //}
    
    inline float dot(const Vector3d& v) const
    {
        return (this->_x * v._x + this->_y * v._y + this->_z * v._z);
    }
    
    inline float norm(void) const
    {
        return (std::sqrt(this->_x * this->_x + this->_y * this->_y + this->_z * this->_z));
    }
};

#endif // __Vector3d_HPP__
