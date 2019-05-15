#include <obj.h>

using namespace Eigen;
using std::string;
using std::vector;

MatrixXd Obj::rodrigues_rotation(Vector3d vp, Vector3d vn, double radian){
    double a, b;
    Vector3d rotated = Vector3d::Zero();
    Matrix3d rodmat;
    a = std::cos(radian);
    b = std::sin(radian);

    // Initialize matrix
    rodmat(0, 0) = a + std::pow(vn(0), 2) * (1 - a);
    rodmat(0, 1) = vn(0) * vn(1) * (1 - a) - vn(2) * b;
    rodmat(0, 2) = vn(0) * vn(2) * (1 - a) + vn(1) * b;
    rodmat(1, 0) = vn(1) * vn(0) * (1 - a) + vn(2) * b;
    rodmat(1, 1) = a + std::pow(vn(1), 2) * (1 - a);
    rodmat(1, 2) = vn(1) * vn(2) * (1 - a) - vn(0) * b;
    rodmat(2, 0) = vn(2) * vn(0) * (1 - a) - vn(1) * b;
    rodmat(2, 1) = vn(2) * vn(1) * (1 - a) + vn(0) * b;
    rodmat(2, 2) = a + std::pow(vn(2), 2) * (1 - a);

    rotated = rodmat * vp;
    return rotated;
}

void Obj::write_mtl(string fn_obj, string fn_mtl, MaterialColor color, int filemode){
    FILE *fp;
    if (filemode == mode_write)
    {
        fp = fopen(fn_obj.c_str(), "w");
        fprintf(fp, "mtllib %s\n", fn_mtl.c_str());
    }
    else if (filemode = mode_append)
    {
        fp = fopen(fn_obj.c_str(), "a");
    }
    fprintf(fp, "usemtl %s\n", color.name.c_str());
    fclose(fp);

    if (filemode == mode_write)
    {
        fp = fopen(fn_mtl.c_str(), "w");
    }
    else if (filemode = mode_append)
    {
        fp = fopen(fn_mtl.c_str(), "a");
    }
    fprintf(fp, "newmtl %s\n", color.name.c_str());
    fprintf(fp, "Ka %f %f %f\n", color.ambient[0], color.ambient[1], color.ambient[2]);
    fprintf(fp, "Kd %f %f %f\n", color.diffuse[0], color.diffuse[1], color.diffuse[2]);
    fprintf(fp, "Ks %f %f %f\n\n", color.specular[0], color.specular[1], color.specular[2]);
}

void Obj::write_sphere(float radius, int stacks, int slices, Vector3d position, string filename, int filemode){
    // Make normal vectors
    double x, y, z;
    vector<Vector3d> vnormal;
    for (int i = 0; i < stacks + 1; i++)
    {
        for (int j = 0; j < slices; j++)
        {
            if (i == 0)
            {
                x = 0;
                y = 0;
                z = 1;
                vnormal.push_back(Vector3d(x, y, z));
                break;
            }
            else if (i == stacks)
            {
                x = 0;
                y = 0;
                z = -1;
                vnormal.push_back(Vector3d(x, y, z));
                break;
            }
            x = std::cos(2 * M_PI * j / slices) * std::sin(M_PI * i / stacks);
            y = std::sin(2 * M_PI * j / slices) * std::sin(M_PI * i / stacks);
            z = std::cos(M_PI * i / stacks);
            vnormal.push_back(Vector3d(x, y, z));
        }
    }

    // Make vertex vector
    vector<Vector3d> vvertex;
    for (int i = 0; i < vnormal.size(); i++)
    {
        vvertex.push_back(radius * vnormal.at(i) + position);
    }

    // Make face node set v//vn
    int idx_1, idx_2;
    int nvvertex = vvertex.size();
    vector<vector<RowVector2i>> vvface;
    vector<RowVector2i> vface;
    int cnt = 2;
    // 無理やり場合分けしてるけど%を使えばいらなくなるはず．
    for (int i = 1; i <= stacks; i++)
    {
        for (int j = 1; j < slices + 1; j++)
        {
            if (i == 1)
            {
                if (j == slices)
                {
                    vface.push_back(RowVector2i({-1, -1}));
                    vface.push_back(RowVector2i({-(j + 1), -(j + 1)}));
                    vface.push_back(RowVector2i({-(j + 2 - slices), -(j + 2 - slices)}));
                }
                else
                {
                    vface.push_back(RowVector2i({-1, -1}));
                    vface.push_back(RowVector2i({-(j + 1), -(j + 1)}));
                    vface.push_back(RowVector2i({-(j + 2), -(j + 2)}));
                }
            }
            else if (i == stacks)
            {
                if (j == slices)
                {
                    vface.push_back(RowVector2i({-nvvertex, -nvvertex}));
                    vface.push_back(RowVector2i({-nvvertex + j, -nvvertex + j}));
                    vface.push_back(RowVector2i({-nvvertex + j + 1 - slices, -nvvertex + j + 1 - slices}));
                }
                else
                {
                    vface.push_back(RowVector2i({-nvvertex, -nvvertex}));
                    vface.push_back(RowVector2i({-nvvertex + j, -nvvertex + j}));
                    vface.push_back(RowVector2i({-nvvertex + j + 1, -nvvertex + j + 1}));
                }
            }
            else
            {
                if (j == slices)
                {
                    vface.push_back(RowVector2i({-cnt, -cnt}));
                    vface.push_back(RowVector2i({-(cnt + slices), -(cnt + slices)}));
                    vface.push_back(RowVector2i({-(cnt + 1), -(cnt + 1)}));
                    vface.push_back(RowVector2i({-(cnt + 1 - slices), -(cnt + 1 - slices)}));
                }
                else
                {
                    vface.push_back(RowVector2i({-cnt, -cnt}));
                    vface.push_back(RowVector2i({-(cnt + slices), -(cnt + slices)}));
                    vface.push_back(RowVector2i({-(cnt + slices + 1), -(cnt + slices + 1)}));
                    vface.push_back(RowVector2i({-(cnt + 1), -(cnt + 1)}));
                }
                cnt++;
            }
            vvface.push_back(vface);
            vface.clear();
        }
    }

    FILE *fp;
    if (filemode == mode_write)
    {
        fp = fopen(filename.c_str(), "w");
    }
    else if (filemode = mode_append)
    {
        fp = fopen(filename.c_str(), "a");
    }
    // Write vertex
    for (int i = 0; i < nvvertex; i++)
    {
        fprintf(fp, "v  %5.5f %5.5f %5.5f\n", vvertex.at(i)(0), vvertex.at(i)(1), vvertex.at(i)(2));
    }
    // Write normal
    for (int i = 0; i < nvvertex; i++)
    {
        fprintf(fp, "vn %5.5f %5.5f %5.5f\n", vnormal.at(i)(0), vnormal.at(i)(1), vnormal.at(i)(2));
    }
    // Write face
    string face = "";
    for (int i = 0; i < vvface.size(); i++)
    {
        vface = vvface.at(i);
        face = "f";
        for (int j = 0; j < vface.size(); j++)
        {
            face += " " + std::to_string(vface.at(j)(0)) + "//" + std::to_string(vface.at(j)(1));
        }
        fprintf(fp, "%s\n", face.c_str());
    }
    fclose(fp);
}

void Obj::write_cylinder(float radius, int N, Vector3d vp1, Vector3d vp2, string filename, int filemode){
    // Normal of Upper Circle
    Vector3d n1 = (vp2 - vp1);
    double height = n1.norm();
    n1 /= n1.norm();
    // Normalized vector m: radius direction
    Vector3d m;
    if (n1(1) != 0)
    {
        double alpha = n1(0) / n1(1);
        m(0) = 1 / std::pow(1 + std::pow(alpha, 2), 0.5);
        m(1) = -alpha * m(0);
        m(2) = 0;
    }
    else if (n1(0) != 0)
    {
        double alpha = n1(1) / n1(0);
        m(1) = std::pow(1 - std::pow(alpha, 2), 0.5);
        m(0) = -alpha * m(1);
        m(2) = 0;
    }
    else if (n1(2) != 0)
    {
        double alpha = n1(0) / n1(2);
        m(0) = std::pow(1 - std::pow(alpha, 2), 0.5);
        m(2) = -alpha * m(0);
        m(1) = 0;
    }

    // Make vertex
    MatrixXd svertex_top(N, 3);
    MatrixXd svertex_bot(N, 3);
    // Make normal
    MatrixXd snormal_side(N, 3);
    double drad = 2 * M_PI / N;
    Vector3d vbase = radius * m;
    for (int i = 0; i < N; i++)
    {
        if (i == 0)
        {
            svertex_top.row(0) = vbase + vp2;
            svertex_bot.row(0) = vbase + vp1;
            snormal_side.row(0) = m;
        }
        else
        {
            snormal_side.row(i) = Obj::rodrigues_rotation(m, n1, i * drad).transpose();
            svertex_top.row(i) = radius * snormal_side.row(i) + vp2.transpose();
            svertex_bot.row(i) = radius * snormal_side.row(i) + vp1.transpose();
        }
    }

    // Write to File
    FILE *fp;
    if (filemode == mode_write)
    {
        fp = fopen(filename.c_str(), "w");
    }
    else if (filemode = mode_append)
    {
        fp = fopen(filename.c_str(), "a");
    }
    // fprintf(fp, "# Test object\n");
    // Upper circle vertex and normal
    for (int i = 0; i < N; i++)
    {
        fprintf(fp, "v  %5.5f %5.5f %5.5f\n", svertex_top(i, 0), svertex_top(i, 1), svertex_top(i, 2));
    }
    fprintf(fp, "vn %5.5f %5.5f %5.5f\n", n1(0), n1(1), n1(2));
    // Upper circle face
    string face = "f ";
    for (int i = 1; i <= N; i++)
    {
        face += " " + std::to_string(-i) + "//-1 ";
    }
    face += "\n";
    fprintf(fp, "%s", face.c_str());

    // Side rectangles vertex
    for (int i = 0; i < N; i++)
    {
        fprintf(fp, "v  %5.5f %5.5f %5.5f\n", svertex_bot(i, 0), svertex_bot(i, 1), svertex_bot(i, 2));
        fprintf(fp, "v  %5.5f %5.5f %5.5f\n", svertex_top(i, 0), svertex_top(i, 1), svertex_top(i, 2));
    }
    // Side rectangles normal
    for (int i = 0; i < N; i++)
    {
        fprintf(fp, "vn %5.5f %5.5f %5.5f\n", snormal_side(i, 0), snormal_side(i, 1), snormal_side(i, 2));
    }
    // Side face
    for (int i = 0; i < 2 * N ; i += 2)
    {
        face = "";
        // i1, i2, i3, i4 are -1 ~ -2N , Face is {-1, -2, -3, -4}, {-3, -4, -5, -6}, ..., {-2N, -(2N+1), -(2N+2), -(2N+3)}
        int i1 = -(i + 1);
        int i2 = -((i + 1) % (2 * N) + 1);
        int i3 = -((i + 2) % (2 * N) + 1);
        int i4 = -((i + 3) % (2 * N) + 1);

        face += "f " + std::to_string(i1) + "//" + std::to_string(-i / 2) + " " +
                std::to_string(i2) + "//" + std::to_string(-i / 2) + " " +
                std::to_string(i4) + "//" + std::to_string(-i / 2 - 1) + " " +
                std::to_string(i3) + "//" + std::to_string(-i / 2 - 1) + "\n";
        fprintf(fp, "%s", face.c_str());
    }

    // Bottom circle vertex
    for (int i = 0; i < N; i++)
    {
        fprintf(fp, "v %5.5f %5.5f %5.5f\n", svertex_bot(i, 0), svertex_bot(i, 1), svertex_bot(i, 2));
    }
    // Bottom circle normal
    fprintf(fp, "vn %5.5f %5.5f %5.5f\n", -n1(0), -n1(1), -n1(2));
    // Borttom circle face
    face = "f ";
    for (int i = 1; i <= N; i++)
    {
        face += " " + std::to_string(-i) + "//-1 ";
    }
    face += "\n";
    fprintf(fp, "%s", face.c_str());

    fclose(fp);
}

