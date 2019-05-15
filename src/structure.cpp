#include <structure.h>

structure::structure(const structure* structure0 ){
    copy(structure0);
}

structure::structure(int natom, MatrixXd slattice_vector, MatrixXd sposition, vector<string> species, vector<int> ntypes){
    this->natom = natom;
    this->slattice_vector = slattice_vector;
    this->sposition = sposition;
    this->species = species;
    this->ntypes = ntypes;
}

structure::structure(const char* filename){
    string str;
    string coordinate; // Direct or Cartesian 
    std::stringstream ss;
    double mag;
    int natom = 0;
    vector<string> species;
    vector<int> ntypes;
    MatrixXd slattice_vector(3, 3);
    MatrixXd sposition;


    std::ifstream ifs(filename);
    if (ifs.fail())
    {
        std::cout << "Fail to open " << filename << std::endl;
        exit(1);
    }

    int cnt = 0;
    while (getline(ifs, str))
    {
        // Comment line
        if (cnt == 0)
        {
            std::cout << "line 1: " << str << std::endl;
        }
        // Magnification
        else if (cnt == 1)
        {
            //  << str << endl;
            sscanf(str.c_str(), "%lf", &mag);
            std::cout << "line 2: magnification = " << mag << std::endl;
        }
        // Lattice vectors
        else if (2 <= cnt && cnt <= 4)
        {
            // Clear
            ss.str("");
            ss.clear(std::stringstream::goodbit);

            ss << str;
            ss >> slattice_vector(cnt - 2, 0) >> slattice_vector(cnt - 2, 1) >> slattice_vector(cnt - 2, 2);
        }
        // Atomic species
        else if (cnt == 5)
        {
            // Clear
            ss.str("");
            ss.clear(std::stringstream::goodbit);

            ss << str;
            string specie;
            while (std::getline(ss, specie, ' '))
            {
                if (specie != "")
                {
                    species.push_back(specie);
                    std::cout << specie << " ";
                }
            }
            std::cout << std::endl;
        }
        // Number of each atom
        else if (cnt == 6)
        {
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
                    std::cout << ntype << " ";
                }
            }
            std::cout << std::endl;
            // Summation
            natom = std::accumulate(ntypes.begin(), ntypes.end(), 0);
            sposition.resize(natom, 3);
            std::cout << "Number of atom: " << natom << std::endl;
        }
        // Direct or Cartesian
        else if (cnt == 7){
            coordinate = str;
        }
        // Positions of atom
        else if (cnt >= 8 && cnt - 8 < sposition.rows())
        {

            // Clear
            ss.str("");
            ss.clear(std::stringstream::goodbit);

            ss << str;
            ss >> sposition(cnt - 8, 0) >> sposition(cnt - 8, 1) >> sposition(cnt - 8, 2);
        }


        // cout << str << endl;
        cnt++;
    }
    std::cout << slattice_vector << std::endl;
    std::cout << sposition << std::endl;
    this->natom = natom;
    this->slattice_vector = slattice_vector;
    this->sposition = sposition;
    this->species = species;
    this->ntypes = ntypes;

    if (coordinate[0] == 'd' or coordinate[0] == 'D'){
        this->sposition = this->direct2cartesian(this->sposition);
    }
}

structure::structure(const string filename, const string type){
    if (type=="poscar"){
        structure(filename.c_str());
    }
    else if(type=="xyz"){
        structure temp = xyz2structure(filename);
        this->natom = temp.natom;
        this->slattice_vector = temp.slattice_vector;
        this->sposition = temp.sposition;
        this->species = temp.species;
        this->ntypes = temp.ntypes;
    }
    else if(type=="xyz2"){
        structure temp = xyz2_2structure(filename);
        this->natom = temp.natom;
        this->slattice_vector = temp.slattice_vector;
        this->sposition = temp.sposition;
        this->species = temp.species;
        this->ntypes = temp.ntypes;
    }
}

structure structure::xyz2structure(const string filename){
    string str, substr;
    vector<string> vsubstr;
    std::stringstream ss;
    int natom = 0;
    vector<string> species;
    vector<int> ntypes;

    vector<string> vspecie;
    vector<int> typat;
    vector<double> vposition;

    MatrixXd slattice_vector(3, 3);
    MatrixXd sposition;
    std::ifstream ifs(filename);

    char atom;
    double pos_x, pos_y, pos_z;

    // Split to line
    int nline = 0;
    while (getline(ifs, str))
    {
        if (nline>1){
            // Clear
            ss.str("");
            ss.clear(std::stringstream::goodbit);
            ss << str;
            std::cout << str << std::endl;
            // Split to word
            while (getline(ss, substr, ' '))
            {
                if (substr != "")
                {
                    vsubstr.push_back(substr);
                }
            }
            vspecie.push_back(vsubstr.at(0));
            // c++11
            vposition.push_back(stof(vsubstr.at(1)));
            vposition.push_back(stof(vsubstr.at(2)));
            vposition.push_back(stof(vsubstr.at(3)));
            vsubstr.clear();
        }
        nline++;
    }

    natom = vspecie.size();
    sposition = MatrixXd(natom, 3);
    int cnt = 0;
    for (int i = 0; i < natom; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            sposition(i, j) = vposition.at(cnt);
            cnt++;
        }
    }
    std::cout << "----------test----------" << std::endl;
    std::cout << sposition << std::endl;
    // ここまでok
    slattice_vector = 15 * Matrix3d::Identity();
    for (int i = 0; i < natom; i++)
    {
        typat.push_back(this->periodic.name2num(vspecie.at(i)));
        printf("typat %5d %3d\n", i, typat.at(i));
    }
    vector<int> typat_buf = typat;
    // ここまでok
    std::sort(typat.begin(), typat.end());
    typat.erase(std::unique(typat.begin(), typat.end()), typat.end());
    for (int i = 0; i < typat.size(); i++)
    {
        printf("sorted typat %3d\n", typat.at(i));
    }
    // ここまでおk
    for (int i = 0; i < typat.size(); i++)
    {
        species.push_back(this->periodic.num2name(typat.at(i)));
        ntypes.push_back(0);
        for (int j = 0; j < typat_buf.size(); j++)
        {
            if (typat.at(i) == typat_buf.at(j))
            {
                ntypes.at(i) = ntypes.at(i) + 1;
            }
        }
    }
    return structure(natom, slattice_vector, sposition, species, ntypes);
}

structure structure::xyz2_2structure(const string filename){
    string str, substr;
    vector<string> vsubstr;
    std::stringstream ss;
    int natom = 0;
    vector<string> species;
    vector<int> ntypes;

    vector<string> vspecie;
    vector<int> typat;
    vector<double> vposition;

    MatrixXd slattice_vector(3, 3);
    MatrixXd sposition;
    std::ifstream ifs(filename);

    char atom;
    double pos_x, pos_y, pos_z;

    // Split to line
    while (getline(ifs, str)){
        // Clear
        ss.str("");
        ss.clear(std::stringstream::goodbit);
        ss << str;
        std::cout << str << std::endl;
        // Split to word
        while (getline(ss, substr, ' '))
        {
            if (substr != "")
            {
                vsubstr.push_back(substr);
            }
        }
        vspecie.push_back(vsubstr.at(0));
        // c++11
        vposition.push_back(stof(vsubstr.at(1)));
        vposition.push_back(stof(vsubstr.at(2)));
        vposition.push_back(stof(vsubstr.at(3)));
        vsubstr.clear();
    }
    
    natom = vspecie.size();
    sposition = MatrixXd(natom, 3);
    int cnt = 0;
    for (int i=0;i<natom;i++){
        for (int j=0;j<3;j++){
            sposition(i, j) = vposition.at(cnt);
            cnt++;
        }
    }
    std::cout << "----------test----------" << std::endl;
    std::cout << sposition << std::endl;
    // ここまでok
    slattice_vector = 15*Matrix3d::Identity();
    for (int i=0;i<natom;i++){
        typat.push_back(this->periodic.name2num(vspecie.at(i)));
        printf("typat %5d %3d\n", i, typat.at(i));
    }
    vector<int> typat_buf = typat;
    // ここまでok
    std::sort(typat.begin(), typat.end());
    typat.erase(std::unique(typat.begin(), typat.end()), typat.end());
    for (int i=0;i<typat.size();i++){
        printf("sorted typat %3d\n", typat.at(i));
    }
    // ここまでおk
    for (int i=0;i<typat.size();i++){
        species.push_back(this->periodic.num2name(typat.at(i)));
        ntypes.push_back(0);
        for (int j=0;j<typat_buf.size();j++){
            if (typat.at(i)==typat_buf.at(j)){
                ntypes.at(i) = ntypes.at(i) + 1;
            }
        }
    }
    return structure(natom, slattice_vector, sposition, species, ntypes);
}

void structure::copy(const structure *structure0)
{
    this->natom = structure0->natom;
    this->slattice_vector = structure0->slattice_vector;
    this->sposition = structure0->sposition;
    this->species = structure0->species;
    this->ntypes = structure0->ntypes;
}

// cartesian to direct coordinate
MatrixXd structure::cartesian2direct(){
    MatrixXd slattice_vector = this->slattice_vector;
    MatrixXd inv_slattice;
    MatrixXd sposition = this->sposition;
    inv_slattice = slattice_vector.inverse();
    return MatrixXd(sposition * inv_slattice);
}

// cartesian to direct coordinate
MatrixXd structure::direct2cartesian(MatrixXd sdirect_position)
{
     return MatrixXd(sdirect_position * this->slattice_vector);
}
/*
    INPUT: Vector3d vn: rotate center axis, double radian: rotate radian
    OUTPUT: MatrixXd rotated: rotated positions
*/
MatrixXd structure::rodrigues_rotation(Vector3d vn, double radian)
{
    double a, b;
    MatrixXd rotated;
    MatrixXd sposition = this->sposition;
    Matrix3d rodmat;
    a = std::cos(radian);
    b = std::sin(radian);
    // Initialize matrix
    rodmat(0, 0) = a + std::pow(vn(0), 2)*(1-a);
    rodmat(0, 1) = vn(0)*vn(1)*(1-a)-vn(2)*b;
    rodmat(0, 2) = vn(0)*vn(2)*(1-a)+vn(1)*b;
    rodmat(1, 0) = vn(1)*vn(0)*(1-a)+vn(2)*b;
    rodmat(1, 1) = a + std::pow(vn(1), 2) * (1-a);
    rodmat(1, 2) = vn(1)*vn(2)*(1-a)-vn(0)*b;
    rodmat(2, 0) = vn(2)*vn(0)*(1-a)-vn(1)*b;
    rodmat(2, 1) = vn(2)*vn(1)*(1-a)+vn(0)*b;
    rodmat(2, 2) = a + std::pow(vn(2), 2)*(1-a);

    rotated = MatrixXd(rodmat * sposition.transpose()).transpose();
    return rotated;
}

/*
    INPUT:
    OUTPUT: vector<int> indexed all atomic number. e.g. 1 1 1 1 2 2 3
*/
vector<int> structure::typat()
{
    vector<int> vtypat;
    for (int i = 0; i < this->species.size(); i++)
    {
        for (int j = 0; j < this->ntypes.at(i); j++)
        {
            vtypat.push_back(i+1);
        }
    }
    return vtypat;
}

/*
    INPUT:
    OUTPUT: MatrixXd vcenter: gravity of center's vector
*/
Vector3d structure::gravity_center()
{
    Vector3d vcenter = Vector3d::Zero();
    MatrixXd sposition = this->sposition;
    vector<double> smass;
    double mass_total=0, mass;
    for (int i=0;i<this->species.size();i++){
        for (int j=0;j<this->ntypes.at(i);j++){
            mass = this->periodic.name2mass(this->species.at(i));
            smass.push_back(mass);
            mass_total += mass;
        }
    }

    // v_center = \frac{\sum_i m_i p_i}{\sum_i m_i}
    for (int i=0;i<this->natom;i++){
        vcenter += smass.at(i) * sposition.row(i);
    }
    vcenter /= mass_total;

    return vcenter;
}
