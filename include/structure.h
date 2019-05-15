#ifndef MyHeader_Structure
#define MyHeader_Structure

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <cmath>
#include <complex>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/LU>
#include <eigen3/Eigen/Geometry>
#include <periodic_table.h>
// Structure of atom
using std::vector;
using std::string;
using namespace Eigen;
class structure
{
    private:
    periodic_table periodic;
    public:
    int natom;
    MatrixXd slattice_vector;
    MatrixXd sposition;
    vector<string> species;
    vector<int> ntypes;
    structure(const structure* structure0);
    structure(int natom, MatrixXd slattice_vector, MatrixXd sposition, vector<string> species, vector<int> ntypes);
    structure(const char* filename);
    structure(const string filename, const string type);
    structure xyz2structure(const string filename);                 // Normal xyz
    structure xyz2_2structure(const string filename);               // xyz
    void copy(const structure* structure0);                         // Copy object
    MatrixXd cartesian2direct();                                    // Cartesian coordinate to Direct coordinate
    MatrixXd direct2cartesian(MatrixXd sdirect_position);           // Direct coordinate to Cartesina coordinate
    MatrixXd rodrigues_rotation(Vector3d vn, double radian);        // Rodrigues rotation
    vector<int> typat();
    Vector3d gravity_center();                                      // Center of gravity
};

#endif