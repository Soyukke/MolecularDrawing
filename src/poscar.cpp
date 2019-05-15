// #include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <cmath>
#include <complex>
#include <eigen3/Eigen/Core>
#include <structure.h>

using std::cout; using std::cin; using std::endl; using std::string; using std::vector;
using namespace Eigen;


int read_poscar(char* filename){
    string str;
    
    std::stringstream ss;
    double mag;
    int natom = 0;
    vector<string> species;
    vector<int> ntypes;
    MatrixXd slattice_vector(3, 3);
    MatrixXd sposition;

    std::ifstream ifs(filename);
    if (ifs.fail()){
        cout << "Fail to open " << filename << endl;
    }

    int cnt=0;
    while (getline(ifs, str)){
        // Comment line
        if (cnt==0){
            cout << "line 1: " << str << endl;
        }
        // Magnification
        else if (cnt==1){
            //  << str << endl;
            sscanf(str.c_str(), "%lf", &mag);
            cout << "line 2: magnification = " << mag << endl;
        }
        // Lattice vectors
        else if (2 <= cnt && cnt <= 4){
            // Clear
            ss.str("");
            ss.clear(std::stringstream::goodbit);

            ss << str;
            ss >> slattice_vector(cnt - 2, 0) >> slattice_vector(cnt - 2, 1) >> slattice_vector(cnt - 2, 2);
        }
        // Atomic species
        else if (cnt==5){
            // Clear
            ss.str("");
            ss.clear(std::stringstream::goodbit);

            ss << str;
            string specie;
            while (std::getline(ss, specie, ' ')){
                if (specie!=""){
                    species.push_back(specie);
                    cout << specie << " ";
                }
            }
            cout << endl;
        }
        // Number of each atom
        else if (cnt==6){
            // Clear
            ss.str("");
            ss.clear(std::stringstream::goodbit);

            ss << str;
            string ntype;
            while (std::getline(ss, ntype, ' '))
            {
                if (ntype != "")
                {
                    ntypes.push_back(atoi(ntype.c_str()));
                    cout << ntype << " ";
                }
            }
            cout << endl;
            // Summation
            natom = std::accumulate(ntypes.begin(), ntypes.end(), 0);
            sposition.resize(natom, 3);
            cout << "Number of atom: " << natom << endl; 
        }
        // Positions of atom
        else if (cnt >= 7 && cnt-7<sposition.rows())
        {

            // Clear
            ss.str("");
            ss.clear(std::stringstream::goodbit);

            ss << str;
            ss >> sposition(cnt-7, 0) >> sposition(cnt-7, 1) >> sposition(cnt-7, 2);
        }

        
        // cout << str << endl;
        cnt++;
    }
    cout << slattice_vector << endl;
    cout << sposition << endl;

    structure my_struct(natom, slattice_vector, sposition, species, ntypes);
    cout << "natom " << natom << endl;
    cout << "my_struct.natom " << my_struct.natom << endl;
    cout << "my_struct.slattice_vector\n " << my_struct.slattice_vector << endl;
}

int main(int argc, char *argv[])
{
    cout << argv[1] << endl;
    // read_poscar(argv[1]);
    // POSCAR の読み込み
    structure my_str(argv[1]);
    return 0;
}