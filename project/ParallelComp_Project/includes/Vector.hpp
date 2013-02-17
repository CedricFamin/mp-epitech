#ifndef __VECTOR_HPP__
# define __VECTOR_HPP__

# include <cmath>

struct Vector3d
{
    double _x;
    double _y;
    double _z;
    
    inline Vector3d operator-(const Vector3d& v) const
    {
        return Vector3d{this->_x - v._x, this->_y - v._y, this->_z - v._z};
    }
    
    inline Vector3d operator/(const double& nb) const
    {
        return Vector3d{this->_x / nb, this->_y / nb, this->_z / nb};
    }
    
    inline bool operator==(const Vector3d& lhs) const
    {
        return _x == lhs._x && _y == lhs._y && _z == lhs._z;
    }
    
    inline Vector3d operator+(const Vector3d& v) const
    {
        return Vector3d{this->_x + v._x, this->_y + v._y, this->_z + v._z};
    }
    
    inline Vector3d cross(const Vector3d& v) const
    {
        return Vector3d{this->_y * v._z - this->_z * v._y, this->_z * v._x - this->_x * v._z, this->_x * v._y - this->_y * v._x};
    }
    
    inline double dot(double const& x, double const& y, double const& z) const
    {
        return (this->_x * x + this->_y * y + this->_z * z);
    }
    
    inline double dot(const Vector3d& vector) const
    {
        return (this->_x * vector._x + this->_y * vector._y + this->_z * vector._z);
    }
    
    inline double norm(void) const
    {
        return (std::sqrt(this->_x * this->_x + this->_y * this->_y + this->_z * this->_z));
    }
};

#endif // __VECTOR_HPP__
