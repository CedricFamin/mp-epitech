#include "Vector.hpp"

#include <typeinfo>
#include <iostream>
#include <iomanip>

int main(void)
{
  Vector* v1 = new Vector{3.4, 5.4, 4.2};
  Vector v42 = {0, 1, 1};
  Vector v30 = {0, 1, 2};
  Vector v2{32.4, 8.1, 1.3};
  Vector v4{3.4, 2.3, 9.7};
  Vector v3 = v1->cross(v2);
  Vector v5 = {1, 1, 1};

  Vector* v = new Vector{4.5, 2.3, 4.6};
  std::cout << v3._x << " " << v3._y << " " << v3._z << std::endl;
  std::cout << v2._x << " " << v2._y << " " << v2._z << std::endl;
  std::cout << v1->_x << " " << v1->_y << " " << v1->_z << std::endl;
 
  std::cout << v42._x << " " << v42._y << " " << v42._z << std::endl;
  std::cout << v30._x << " " << v30._y << " " << v30._z << std::endl;
  double dotResult = v1->dot(5.4, 2.3, 6.7);
  double normResult = v1->norm();
  std::cout << dotResult << std::endl;
  std::cout << normResult << std::endl;
  delete v1;
  delete v;
  return 0;
}
