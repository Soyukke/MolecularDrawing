#include <periodic_table.h>
#include <iostream>
#include <sstream>
#include <algorithm>

using std::cout; using std::cin; using std::endl;

periodic_table::periodic_table()
{
    std::stringstream ss, ss_Atoms;
    string str, word;
    vector<string> vword;
    int num, outermost_nelectron;
    double mass;
    string name;
    // https://github.com/jjuback/gc-excelviewer/blob/master/data/Periodic%20Table%20of%20Elements%20Semicolon.csv
    // Atomic Number;Element;Symbol;Atomic Weight;Period;Group;Phase;Most Stable Crystal;Type;Ionic Radius;Atomic Radius;Electronegativity;First Ionization Potential;Density;Melting Point (K);Boiling Point (K);Isotopes;Discoverer;Year of Discovery;Specific Heat Capacity;Electron Configuration;Display Row;Display Column
    // 0: Atomic Number
    // 2: Symbol
    // 3: Atomic Weight
    // 4: Period
    // 5: Group
    // 9: Ionic Radius
    // 10: Atomic Radius
    const int atomic_number=0, symbol=2, atomic_weight=3, period=4, group=5, ionic_radius=9, atomic_radius=10;
    const string Atoms =
    #include "Periodic Table of Elements Semicolon.csv"
    ;
    ss_Atoms << Atoms;

    // split file by line
    int cnt = 0;
    while (getline(ss_Atoms, str))
    {
        if (cnt==0){
            // Skip first line
        }
        else{
            // split line by Semicolon
            // Clear
            ss.str("");
            ss.clear(std::stringstream::goodbit);
            ss << str;
            
            while (getline(ss, word, ';'))
            {
                vword.push_back(word);
            }

            if (stoi(vword.at(0)) <= this->size){
                // For empty
                if (vword.at(ionic_radius)==""){
                    vword.at(ionic_radius) = "-1";
                }
                if (vword.at(atomic_radius) == "")
                {
                    vword.at(atomic_radius) = "-1";
                }

                this->vnum.push_back(std::stoi(vword.at(atomic_number)));
                this->vname.push_back(vword.at(symbol));
                this->vmass.push_back(std::stod(vword.at(atomic_weight)));
                this->vperiod.push_back(std::stoi(vword.at(period)));
                this->vgroup.push_back(std::stoi(vword.at(group)));
                this->vionic_radius.push_back(std::stod(vword.at(ionic_radius)));
                this->vatomic_radius.push_back(std::stod(vword.at(atomic_radius)));
                vword.clear();
            }
        }
        cnt++;
    }
}

string periodic_table::num2name(int num){
    return this->vname.at(num-1);
}

int periodic_table::name2num(string name)
{
    auto result = std::find(this->vname.begin(), this->vname.end(), name);
    if (result == vname.end()){
        // None
        return -1;
    }
    else {
        auto idx = std::distance(vname.begin(), result);
        return this->vnum.at(idx);
    }
}

double periodic_table::num2mass(int num)
{
    return this->vmass.at(num-1);
}

double periodic_table::name2mass(string name){
    return this->num2mass(this->name2num(name));
}

double periodic_table::num2atomic_radius(int num){
    return this->vatomic_radius.at(num-1);
}

double periodic_table::name2atomic_radius(string name){
    double radius = this->num2atomic_radius(this->name2num(name));
    return radius;
}