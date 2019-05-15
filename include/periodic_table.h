#ifndef MyHeader_Periodic_Table
#define MyHeader_Periodic_Table

#include <string>
#include <vector>

using std::string; using std::vector;

class periodic_table
{
    public:
    static const int size = 118;
    private:
    vector<int> vnum;
    vector<string> vname;
    vector<double> vmass;
    vector<int> vperiod;
    vector<int> vgroup;
    vector<double> vatomic_radius;
    vector<double> vionic_radius;
    // vector<int> voutermost_nelectron;

    public:
    periodic_table();
    string num2name(int num);
    int name2num(string name);
    double num2mass(int name);
    double name2mass(string name);
    double num2atomic_radius(int num);
    double name2atomic_radius(string name);
};

#endif
