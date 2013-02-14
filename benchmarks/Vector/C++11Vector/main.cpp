#include "Vector.hpp"

int main(void)
{
  int a = 0;
  int b = 100000000;
  while (a < b)
    {
      Vector v1{3.4, 5.4, 4.2};
      Vector v2{4.3, 4.5, 2.4};
      Vector v3 = v1 - v2;
      Vector v4 = v1 / 42.0;
      Vector v5 = v1 + v2;
      Vector v6 = v1.cross(v2);
      double dot = v1.dot(v2);
      double norm = v1.norm();
      ++a;
    }
  return 0;
}
