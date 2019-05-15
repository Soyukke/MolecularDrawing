#include <iostream>
#include <cmath>
#include <vector>
#include <unistd.h>
#include <GL/freeglut.h>
#include <structure.h>
#include <material_color.h>
#include <getopt.h>
#include <periodic_table.h>
#include <obj.h>
#include <opencv/cxcore.h>  // For save video
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp> // For save image
#define KEY_ESC 27

// window size
int WIDTH = 1000;
int  HEIGHT = 1000;

periodic_table periodic;

//平行移動用
float mx, my, mz = 0.0f;
// float phi, theta = 0.0f;
bool flag = false;
// mode
int mode = 0;
// mode type
const int CAPTURE=10;
const double FPS(15.0);

// 画像保存用
int image_cnt = 0;
int video_cnt = 0;

// 引数のやつ
double MAGNIFICATION = 20.0;
double MOL_Z = 30;
int POSITION_Y = -500, POSITION_Z = 50;
bool flag_debug = false;

//緑
GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
//ライトの位置
GLfloat lightpos[] = {0, 0.0, 100.0, 0};
GLfloat lightambient[] = {1.0, 1.0, 1.0, 1.0};
// GLfloat lightdiffuse[] = {1.0, 1.0, 1.0, 1.0};
Vector3d vp_camera, vt_camera, vn_camera;

// Atomic condition
structure *my_structure; // molecule information
MatrixXd atoms;          // Positions
double bond_cutoff = 1.6; // Cut-off bond length
vector<vector<int>> vbond; // Table of bond

// For save video
IplImage *video_buf;
IplImage *video;
CvVideoWriter *videowriter;

void polarview(void);
void resetview(void);
int xBegin, yBegin;

int mButton;
double elevation, azimuth, scales=1.0;
double theta = 10.0;

// Declare
MatrixXd rodrigues_rotation(Vector3d vp, Vector3d vn, double radian);
void save_image(string filename);
void opencvInit(string filename);

void myMouse(int button, int state, int x, int y)
{
    // 押下
    if (state == GLUT_DOWN)
    {
        switch (button)
        {
        // mause left
        case GLUT_LEFT_BUTTON:
            mButton = button;
            break;
        // mause middle
        case GLUT_MIDDLE_BUTTON:
            mButton = button;
            break;
        }
        xBegin = x;
        yBegin = y;
    }
}

void myMotion(int x, int y)
{
    int xDisp, yDisp;
    // vector from mouse position on click to current mouse position 
    xDisp = x - xBegin;
    yDisp = y - yBegin;
    switch (mButton)
    {
    case GLUT_LEFT_BUTTON:
        azimuth += (double)xDisp / 2.0;
        elevation += (double)yDisp / 2.0;
        break;
    case GLUT_MIDDLE_BUTTON:
        // scale up/down
        scales += (double)yDisp / 20.0;
        break;
    }
    xBegin = x;
    yBegin = y;
    glutPostRedisplay();
}

void resetview(void)
{
    scales = 1.0;
    elevation = 0.0;
    azimuth = 0.0;
    // カメラの設定
    vp_camera = {0, -500, 200};
    vn_camera = {0, 0, 1.0};
    vt_camera = {0.0, 0.0, 0.0};
}

void polarview(void)
{
    // change scale x y z
    glScalef(scales, scales, scales);
    // rotate object
    glRotatef(elevation, 1.0, 0.0, 0.0);
    glRotatef(azimuth, 0.0, 1.0, 0.0);
}

void myKbd(unsigned char key, int x, int y)
{
    Vector3d vtp, vnn;
    double drad = M_PI / 18.0;
    switch (key)
    {
        // reset
    case 'r':
        resetview();
        break;
    // Enter screenshot
    case '\r':
    {
        char temp_filename[20];
        sprintf(temp_filename, "image%03d.png", image_cnt++);
        printf("Screenshot %s", temp_filename);
        save_image(string(temp_filename));
    }
    break;
    case 'w': // Move camera up
        vtp = vp_camera - vt_camera;
        vnn = vn_camera.cross(vtp).normalized();
        vp_camera = rodrigues_rotation(vp_camera, vnn, drad);
        vn_camera = rodrigues_rotation(vn_camera, vnn, drad);
        break;
    case 's': // Move camera down
        vtp = vp_camera - vt_camera;
        vnn = vn_camera.cross(vtp).normalized();
        vp_camera = rodrigues_rotation(vp_camera, vnn, -drad);
        vn_camera = rodrigues_rotation(vn_camera, vnn, -drad);
        break;
    case 'a': // Move camera <-
        vp_camera = rodrigues_rotation(vp_camera - vt_camera, vn_camera, drad);
        break;
    case 'd': // Move camera ->
        vp_camera = rodrigues_rotation(vp_camera - vt_camera, vn_camera, -drad);
        break;
    case 'V': // Capture video
        if (mode==CAPTURE){
            // Finish capture
            printf("Finish capture video!\n");
            cvReleaseVideoWriter(&videowriter);
            cvReleaseImage(&video_buf);
            cvReleaseImage(&video);
            exit(0);
        }else{
            // Start capture
            printf("Start capture video...\n");
            mode = CAPTURE;
            char temp_filename[20];
            sprintf(temp_filename, "video%03d.avi", video_cnt++);
            opencvInit(string(temp_filename)); // For OpenCV
        }
        break;
    case KEY_ESC:
        exit(0);
    }
    glutPostRedisplay();
}


void opencvInit(string filename)
{
    video_buf = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
    video = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
    videowriter = cvCreateVideoWriter(
        filename.c_str(),
        CV_FOURCC('X', 'V', 'I', 'D'), // Xvid
        FPS,
        cvSize(video->width, video->height),
        1);
}



MaterialColor symbol2color(string symbol)
{
    if (symbol == "C")
    {
        return MaterialColor::obsidian;
    }
    else if (symbol == "H")
    {
        return MaterialColor::white_plastic;
    }
    else if (symbol == "O")
    {
        return MaterialColor::ruby;
    }
    else if (symbol == "S")
    {
        return MaterialColor::yellow_plastic;
    }
    else if (symbol == "N")
    {
        return MaterialColor::cyan_plastic;
    }
    else if (symbol == "Br")
    {
        return MaterialColor::chrome;
    }
    else
    {
        return MaterialColor::brass;
    }
}


void save_image(string filename){
    // https://stackoverflow.com/questions/53558514/opengl-render-to-a-opencv-mat-as-texture-and-reuse-it-in-opencv
    cv::Mat image(HEIGHT, WIDTH, CV_8UC3);
    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, (image.step & 3) ? 1 : 4);
    glPixelStorei(GL_PACK_ROW_LENGTH, image.step / image.elemSize());
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    cv::flip(image, image, 0); // x軸反転
    cv::imwrite(filename, image);
    printf("Save Image\n");
}

// ロドリゲスの回転公式 vpをvnまわりにradian回転
MatrixXd rodrigues_rotation(Vector3d vp, Vector3d vn, double radian)
{
    if (flag_debug){
        std::cout << "rodrigues_rotation()\n"
                << std::endl;
    }
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

// 円柱
void cylinder(float radius, float height, int N, Vector3d vp1, Vector3d vp2)
{
    if (flag_debug){
        std::cout << "cylinder()\n"
                  << std::endl;
    }
    //上面
    // 上面の法線ベクトル
    Vector3d n1 = (vp2 - vp1);
    height = n1.norm();
    n1 /= n1.norm();
    // 同心円方向単位ベクトル
    Vector3d m;
    if (n1(1)!=0){
        double alpha = n1(0)/n1(1);
        m(0) = 1 / std::pow(1 + std::pow(alpha, 2), 0.5);
        m(1) = -alpha * m(0);
        m(2) = 0;
    }
    else if (n1(0)!=0){
        double alpha = n1(1) / n1(0);
        m(1) = std::pow(1 - std::pow(alpha, 2), 0.5);
        m(0) = -alpha * m(1);
        m(2) = 0;
    }
    else if (n1(2)!=0)
    {
        double alpha = n1(0) / n1(2);
        m(0) = std::pow(1 - std::pow(alpha, 2), 0.5);
        m(2) = -alpha * m(0);
        m(1) = 0;
    }

    // Vertexの計算
    MatrixXd svertex_top(N, 3);
    MatrixXd svertex_bot(N, 3);
    // 側面の法線ベクトル
    MatrixXd snormal_side(N, 3);
    double drad = 2 * M_PI / N;
    // 基準
    Vector3d vbase = radius * m;
    // std::cout << "vbase : " << vbase << std::endl;
    // 積で0になれ
    // std::cout << "積で0になれ : " << vbase.transpose() * n1 << std::endl;
    for (int i=0;i<N;i++){
        if (i==0){
            svertex_top.row(0) = vbase + vp2;
            svertex_bot.row(0) = vbase + vp1;
            snormal_side.row(0) = m;
        }
        else {
            snormal_side.row(i) = rodrigues_rotation(m, n1, i * drad).transpose();
            svertex_top.row(i) = radius * snormal_side.row(i) + vp2.transpose();
            svertex_bot.row(i) = radius * snormal_side.row(i) + vp1.transpose();
        }
    }

    glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialColor::silver.specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialColor::silver.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialColor::silver.diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, &MaterialColor::silver.shininess);

    glNormal3d(n1(0), n1(1), n1(2));
    glBegin(GL_POLYGON);
    for (int i=0;i<N;i++)
    {
        glVertex3d(svertex_top(i, 0), svertex_top(i, 1), svertex_top(i, 2));
    }
    glEnd();
    //側面
    glBegin(GL_QUAD_STRIP);
    for (int i=0;i<N;i++)
    {
        glNormal3f(snormal_side(i, 0), snormal_side(i, 1), snormal_side(i, 2));
        glVertex3f(svertex_bot(i, 0), svertex_bot(i, 1), svertex_bot(i, 2));
        glVertex3f(svertex_top(i, 0), svertex_top(i, 1), svertex_top(i, 2));
    }
    // glNormal3f(snormal_side(0, 0), snormal_side(0, 1), snormal_side(0, 2));
    glVertex3f(svertex_bot(0, 0), svertex_bot(0, 1), svertex_bot(0, 2));
    glVertex3f(svertex_top(0, 0), svertex_top(0, 1), svertex_top(0, 2));
    glEnd();
    //下面
    glNormal3d(-n1(0), -n1(1), -n1(2));
    glBegin(GL_POLYGON);
    for (int i=0;i<N;i++)
    {
        glVertex3d(svertex_bot(i, 0), svertex_bot(i, 1), svertex_bot(i, 2));
    }
    glEnd();
}

// 地面
void Ground(void)
{
    if (flag_debug){
        std::cout << "Ground()\n" << std::endl;
    }

    double ground_max_x = WIDTH;
    double ground_max_y = HEIGHT;
    glDisable(GL_LIGHTING);
    glColor3d(1.0, 0.0, 0.0);
    glNormal3d(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    for (double ly = -ground_max_y; ly <= ground_max_y; ly += 10.0)
    {
        glVertex3d(-ground_max_x, ly, 0);
        glVertex3d(ground_max_x, ly, 0);
    }
    for (double lx = -ground_max_x; lx <= ground_max_x; lx += 10.0)
    {
        glVertex3d(lx, ground_max_y, 0);
        glVertex3d(lx, -ground_max_y, 0);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void display()
{
    if (flag_debug){
        std::cout << "display()\n" << std::endl;
    }

    // 色と深さバッファをクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //視野角,アスペクト比(ウィンドウの幅/高さ),描画する範囲(最も近い距離,最も遠い距離)
    gluPerspective(30.0, (double)WIDTH / (double)HEIGHT, 1, 4000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //視点の設定
    gluLookAt(vp_camera(0), vp_camera(1), vp_camera(2), vt_camera(0), vt_camera(1), vt_camera(2), vn_camera(0),vn_camera(1), vn_camera(2));

    // 地面
    Ground();
    // マウス操作を実行
    polarview();

    // 結合
    for (int i=0;i<vbond.size();i++){
        cylinder(MAGNIFICATION * 1.2 / 14 , 0.0, 20, atoms.row(vbond.at(i).at(0)), atoms.row(vbond.at(i).at(1)));
    }
    // 分子
    vector<int> typat = my_structure->typat();
    // 原子種
    int idx_atom = 0;
    int cnt_atom = 0;
    string symbol = my_structure->species.at(idx_atom);
    MaterialColor temp_color = symbol2color(symbol);

    for (int i=0;i<my_structure->natom;i++){
        // printf("symbol: %s\n", symbol.c_str());
        glPushMatrix();
        // std::cout << my_structure->sposition.row(i) << std::endl;
        // atomsはposition
        double sphere_radius = MAGNIFICATION * 6 * periodic.name2atomic_radius(symbol) / 14;
        glTranslated(atoms(i, 0), atoms(i, 1), atoms(i, 2)); //引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
        glMaterialfv(GL_FRONT, GL_SPECULAR, temp_color.specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, temp_color.ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, temp_color.diffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, &temp_color.shininess);
        glutSolidSphere(sphere_radius, 30, 30); 
        glPopMatrix();

        cnt_atom++;
        if(cnt_atom==my_structure->ntypes.at(idx_atom)){
            idx_atom++;
            cnt_atom=0;
            if (idx_atom!=my_structure->species.size()){
                symbol = my_structure->species.at(idx_atom);
            }
            // 原子種で色を変える
            temp_color = symbol2color(symbol);
        }
    }


    glFlush();
    // For save video
    if (mode==CAPTURE){
        glReadBuffer(GL_FRONT);
        glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, video_buf->imageData);
        cvConvertImage(video_buf, video, CV_CVTIMG_FLIP + CV_CVTIMG_SWAP_RB);
        cvWriteFrame(videowriter, video);
    }
    glutSwapBuffers();
}
void idle(void)
{
    // 0.02秒
    usleep(2*std::pow(10, 4));
    glutPostRedisplay();
}


void Init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    // glDepthMask(GL_TRUE);
    // テクスチャの表示順序を正しくするため．
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightambient);
    // カメラの設定
    vp_camera = {0, -500, 200};
    vn_camera = {0, 0, 1.0};
    vt_camera = {0.0, 0.0, 0.0};
    // 原子の設定
    // 結合マトリックスの作成
    for (int i = 0; i < my_structure->natom; i++)
    {
        for (int j = 0; j < i; j++)
        {
            double bond = Vector3d(my_structure->sposition.row(i) - my_structure->sposition.row(j)).norm();
            // std::cout << "原子間距離 " << bond << std::endl;
            if (bond < bond_cutoff)
            {
                vbond.push_back({i, j});
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int opt;
    extern char *optarg;
    extern int optind, opterr;
    int longindex = -1;
    const int ixyz=500, ixyz2=501, oobj=601, mol_z=301, save=302, cam_x=400, cam_y=401, cam_z=402, wwidth=700, wheight=701;
    struct option longopts[] = {
        {"input", required_argument, NULL, 'i'},
        {"ixyz", required_argument, NULL, ixyz},
        {"ixyz2", required_argument, NULL, ixyz2},
        {"oobj", required_argument, NULL, oobj},
        {"scale", required_argument, NULL, 's'},
        {"mol_z", required_argument, NULL, mol_z},
        {"cam_x", required_argument, NULL, cam_x},
        {"cam_y", required_argument, NULL, cam_y},
        {"cam_z", required_argument, NULL, cam_z},
        {"save", required_argument, NULL, save},
        {"debug", required_argument, NULL, 'd'},
        {"width", required_argument, NULL, wwidth},
        {"height", required_argument, NULL, wheight},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0},
    };

    // For read structure
    string filename;
    // For obj
    int stacks = 40, slices = 40;
    double radius = 0.4, bond;
    string fn_obj;
    string symbol_obj;
    vector<int> typat_obj;

    // - で一致したらlongindexは更新されないようだ．
    // opt で場合分けしてから defaultないでlongindexで場合分け．
    while ((opt = getopt_long(argc, argv, "i:s:d:h", longopts, &longindex)) != -1)
    {
        switch (opt)
        {
        case 'i':
            filename = optarg;
            printf("FILENAME: %s\n", filename.c_str());
            // POSCARの読み込み
            my_structure = new structure(filename.c_str());
            break;
        case ixyz:
            filename = optarg;
            printf("XYZ FILENAME: %s\n", filename.c_str());
            my_structure = new structure(filename.c_str(), "xyz");
            break;
        case ixyz2:
            filename = optarg;
            printf("XYZ2 FILENAME: %s\n", filename.c_str());
            my_structure = new structure(filename.c_str(), "xyz2");
            break;
        // Generate mol.obj
        case oobj:
            fn_obj = optarg;
            typat_obj = my_structure->typat();
            
            symbol_obj = my_structure->species.at(0);

            Obj::write_mtl(fn_obj, "test.mtl", symbol2color(symbol_obj), Obj::mode_write);
            Obj::write_sphere(radius, stacks, slices, my_structure->sposition.row(0), fn_obj, Obj::mode_append);
            // Atom sphere
            for (int i=1;i<my_structure->natom;i++){
                // printf("typat_obj: %5d", typat_obj.at(i));
                symbol_obj = my_structure->species.at(typat_obj.at(i)-1);
                Obj::write_mtl(fn_obj, "test.mtl", symbol2color(symbol_obj), Obj::mode_append);
                Obj::write_sphere(radius, stacks, slices, my_structure->sposition.row(i), fn_obj, Obj::mode_append);
            }
            // Atom cylinder
            for (int i = 0; i < my_structure->natom; i++)
            {
                for (int j = 0; j < i; j++)
                {
                    bond = Vector3d(my_structure->sposition.row(i) - my_structure->sposition.row(j)).norm();
                    if (bond < bond_cutoff)
                    {
                        Obj::write_mtl(fn_obj, "test.mtl", MaterialColor::black_rubber, Obj::mode_append);
                        Obj::write_cylinder(radius*0.3, stacks, my_structure->sposition.row(i), my_structure->sposition.row(j), fn_obj, Obj::mode_append);
                    }
                }
            }
            exit(0);
            break;
        case 's':
            MAGNIFICATION = atof(optarg);
            printf("MAGNIFICATION: %f\n", MAGNIFICATION);
            break;
        case 'd':
            printf("Enable Debug\n");
            flag_debug = true;
        case mol_z:
            MOL_Z = atof(optarg);
            printf("MOL_Z: %f\n", MOL_Z);
            break;
        // Generate image.ppm
        case save:
            {
                GLuint fbID, rbID;
                int FB_WIDTH=WIDTH, FB_HEIGHT;
                // https://stackoverflow.com/questions/9097756/converting-data-from-glreadpixels-to-opencvmat/9098883
                string filename = optarg;
                cv::Mat image(HEIGHT, WIDTH, CV_8UC3);
                Vector3d center = my_structure->gravity_center();
                for (int i = 0; i < my_structure->natom; i++)
                {
                    my_structure->sposition.row(i) -= center;
                }
                atoms = my_structure->sposition;
                MatrixXd direct = my_structure->cartesian2direct();
                atoms = MatrixXd(direct * (MAGNIFICATION * my_structure->slattice_vector));
                Vector3d shift = {0.0, 0.0, MOL_Z};
                for (int i = 0; i < atoms.rows(); i++)
                {
                    atoms.row(i) += shift.transpose();
                }
                glutInitWindowPosition(100, 100);
                glutInitWindowSize(WIDTH, HEIGHT);
                glutInit(&argc, argv);
                // GLUT_DEPTHを入れることで深さの表示が正しくなる
                glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
                glutCreateWindow("display POSCAR");
                glutDisplayFunc(display);
                glutIdleFunc(idle);
                glutMouseFunc(myMouse);
                glutMotionFunc(myMotion);
                glutKeyboardFunc(myKbd);
                Init();
                for (int i=0;i<10;i++){
                    display();
                    glFinish();
                }
                // これがないと背景しかない
                //use fast 4-byte alignment (default anyway) if possible
                glReadBuffer(GL_FRONT);
                glPixelStorei(GL_PACK_ALIGNMENT, (image.step & 3) ? 1 : 4);

                //set length of one complete row in destination data (doesn't need to equal img.cols)
                glPixelStorei(GL_PACK_ROW_LENGTH, image.step / image.elemSize());
                glReadPixels(0, 0, WIDTH, HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, image.data);
                cv::flip(image, image, 0); // x軸反転
                cv::imwrite(filename, image);
                // cv::cvtColor(image2, image, cv::COLOR_GRAY2RGB);
                printf("Save Image\n");
                exit(0);
            }
            break;
        case cam_y:
            POSITION_Y = atoi(optarg);
            printf("POSITION Y %d\n", POSITION_Y);
            break;
        case cam_z:
            POSITION_Z = atoi(optarg);
            printf("POSITION Z %d\n", POSITION_Z);
            break;
        case wwidth:
            WIDTH = atoi(optarg);
            break;
        case wheight:
            HEIGHT = atoi(optarg);
            break;
        case 'h':
            printf("Usage: [-i filename --input filename] [-ixyz filename] [--oobj outfilename] [-s scalse --scale scale] [--mol_z position_z] [--save] [-h --help]\n");
            printf("Enter: capture image\n");
            printf("V: capture video\n");
            printf("esc: exit\n");
            exit(0);
            break;
        default:
            exit(0);
            break;
        }
    }

    Vector3d center = my_structure->gravity_center();
    for (int i = 0; i < my_structure->natom; i++)
    {
        my_structure->sposition.row(i) -= center;
    }
    atoms = my_structure->sposition;
    MatrixXd direct = my_structure->cartesian2direct();
    atoms = MatrixXd(direct * (MAGNIFICATION * my_structure->slattice_vector));
    Vector3d shift = {0.0, 0.0, MOL_Z};
    for (int i = 0; i < atoms.rows(); i++)
    {
        atoms.row(i) += shift.transpose();
    }

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInit(&argc, argv);
    // GLUT_DEPTHを入れることで深さの表示が正しくなる
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("display POSCAR");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(myMouse); // callback function of mause button
    glutMotionFunc(myMotion); // callback function of mause drag
    glutKeyboardFunc(myKbd); // callback function of keyboard button

    Init();
    glutMainLoop();

    return 0;
}