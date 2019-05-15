#ifndef MyHeader_Obj
#define MyHeader_Obj

#include <string>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/LU>
#include <vector>
#include <material_color.h>

class Obj {
    private:
    Obj();
    public:
    static const int mode_write = 1, mode_append = 2;
    static Eigen::MatrixXd rodrigues_rotation(Eigen::Vector3d vp, Eigen::Vector3d vn, double radian);
    static void write_mtl(std::string fn_obj, std::string fn_mtl, MaterialColor color, int filemode);
    static void write_sphere(float radius, int stacks, int slices, Eigen::Vector3d position, std::string filename, int filemode);
    static void write_cylinder(float radius, int N, Eigen::Vector3d vp1, Eigen::Vector3d vp2, std::string filename, int filemode);
};

#endif
