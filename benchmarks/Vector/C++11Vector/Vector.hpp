#ifndef __VECTOR_HPP__
# define __VECTOR_HPP__

# include <cmath>

struct Vector
{
  double _x;
  double _y;
  double _z;

  inline Vector operator-(const Vector& v) const
  {
    return Vector{this->_x - v._x, this->_y - v._y, this->_z - v._z};
  }

  inline Vector operator/(const double& nb) const
  {
    return Vector{this->_x / nb, this->_y / nb, this->_z / nb};
  }

  inline Vector operator+(const Vector& v) const
  {
    return Vector{this->_x + v._x, this->_y + v._y, this->_z + v._z};
  }
  
  inline Vector cross(const Vector& v) const
  {
    return Vector{this->_y * v._z - this->_z * v._y, this->_z * v._x - this->_x * v._z, this->_x * v._y - this->_y * v._x};
  }

  inline double dot(double const& x, double const& y, double const& z) const
  {
    return (this->_x * x + this->_y * y + this->_z * z);
  }

  inline double dot(const Vector& vector) const
  {
    return (this->_x * vector._x + this->_y * vector._y + this->_z * vector._z);
  }

  inline double norm(void) const
  {
    return (std::sqrt(this->_x * this->_x + this->_y * this->_y + this->_z * this->_z));
  }
};

#endif // __VECTOR_HPP__
