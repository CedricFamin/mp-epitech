#include <eigen3/Eigen/Dense>

int main(void)
{
  int a = 0;
  int b = 100000000;
  while (a < b)
    {
      Eigen::Vector3d v1(3.4, 5.4, 4.2);
      Eigen::Vector3d v2(4.3, 4.5, 2.4);
      Eigen::Vector3d v3 = v1 - v2;
      Eigen::Vector3d v4 = v1 / 42.0;
      Eigen::Vector3d v5 = v1 + v2;
      Eigen::Vector3d v6 = v1.cross(v2);
      double dot = v1.dot(v2);
      double norm = v1.norm();
      ++a;
    }
  return 0;
}
