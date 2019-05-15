#ifndef MyHeader_MaterialColor
#define MyHeader_MaterialColor

#include <string>
#include <GL/freeglut.h>

// For openGL
class MaterialColor
{
    public:
    std::string name;
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
    const static MaterialColor ruby;
    //emerald(エメラルド)
    const static MaterialColor emerald;
    //jade(翡翠)
    const static MaterialColor jade;
    //obsidian(黒曜石)
    const static MaterialColor obsidian;
    // pearl(真珠)
    const static MaterialColor pearl;
    //turquoise(トルコ石)
    const static MaterialColor turquoise;
    //brass(真鍮)
    const static MaterialColor brass;
    //bronze(青銅)
    const static MaterialColor bronze;
    //chrome(クローム)
    const static MaterialColor chrome;
    //copper(銅)
    const static MaterialColor copper;
    //gold(金)
    const static MaterialColor gold;
    //silver(銀)
    const static MaterialColor silver;
    //プラスチック(黒)
    const static MaterialColor black_plastic;
    //プラスチック(シアン)
    const static MaterialColor cyan_plastic;
    //プラスチック(緑)
    const static MaterialColor green_plastic;
    //プラスチック(赤)
    const static MaterialColor red_plastic;
    //プラスチック(白)
    const static MaterialColor white_plastic;
    //プラスチック(黄)
    const static MaterialColor yellow_plastic;
    //ゴム(黒)
    const static MaterialColor black_rubber;
    //ゴム(シアン)
    const static MaterialColor cyan_rubber;
    //ゴム(緑)
    const static MaterialColor green_rubber;
    //ゴム(赤)
    const static MaterialColor red_rubber;
    //ゴム(白)
    const static MaterialColor white_rubber;
    //ゴム(黄)
    const static MaterialColor yellow_rubber;
};


#endif
