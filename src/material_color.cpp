#include <material_color.h>
using std::string;
using std::to_string;


const MaterialColor MaterialColor::ruby = {
    "ruby",
    {0.1745, 0.01175, 0.01175, 1.0},
    {0.61424, 0.04136, 0.04136, 1.0},
    {0.727811, 0.626959, 0.626959, 1.0},
    76.8};

//emerald(エメラルド)
const MaterialColor MaterialColor::emerald = {
    "emerald",
    {0.0215, 0.1745, 0.0215, 1.0},
    {0.07568, 0.61424, 0.07568, 1.0},
    {0.633, 0.727811, 0.633, 1.0},
    76.8};

//jade(翡翠)
const MaterialColor MaterialColor::jade = {
    "jade",
    {0.135, 0.2225, 0.1575, 1.0},
    {0.54, 0.89, 0.63, 1.0},
    {0.316228, 0.316228, 0.316228, 1.0},
    12.8};
//obsidian(黒曜石)
const MaterialColor MaterialColor::obsidian = {
    "obsidian",
    {0.05375, 0.05, 0.06625, 1.0},
    {0.18275, 0.17, 0.22525, 1.0},
    {0.332741, 0.328634, 0.346435, 1.0},
    38.4};
// pearl(真珠)
const MaterialColor MaterialColor::pearl = {
    "pearl",
    {0.25, 0.20725, 0.20725, 1.0},
    {1, 0.829, 0.829, 1.0},
    {0.296648, 0.296648, 0.296648, 1.0},
    10.24};
//turquoise(トルコ石)
const MaterialColor turquoise = {
    "turquoise",
    {0.1, 0.18725, 0.1745, 1.0},
    {0.396, 0.74151, 0.69102, 1.0},
    {0.297254, 0.30829, 0.306678, 1.0},
    12.8};
//brass(真鍮)
const MaterialColor MaterialColor::brass = {
    "brass",
    {0.329412, 0.223529, 0.027451, 1.0},
    {0.780392, 0.568627, 0.113725, 1.0},
    {0.992157, 0.941176, 0.807843, 1.0},
    27.89743616};
//bronze(青銅)
const MaterialColor MaterialColor::bronze = {
    "bronze",
    {0.2125, 0.1275, 0.054, 1.0},
    {0.714, 0.4284, 0.18144, 1.0},
    {0.393548, 0.271906, 0.166721, 1.0},
    25.6};
//chrome(クローム)
const MaterialColor MaterialColor::chrome = {
    "chrome",
    {0.25, 0.25, 0.25, 1.0},
    {0.4, 0.4, 0.4, 1.0},
    {0.774597, 0.774597, 0.774597, 1.0},
    76.8};
//copper(銅)
const MaterialColor MaterialColor::copper = {
    "copper",
    {0.19125, 0.0735, 0.0225, 1.0},
    {0.7038, 0.27048, 0.0828, 1.0},
    {0.256777, 0.137622, 0.086014, 1.0},
    12.8};
//gold(金)
const MaterialColor MaterialColor::gold = {
    "gold",
    {0.24725, 0.1995, 0.0745, 1.0},
    {0.75164, 0.60648, 0.22648, 1.0},
    {0.628281, 0.555802, 0.366065, 1.0},
    51.2};
//silver(銀)
const MaterialColor MaterialColor::silver = {
    "silver",
    {0.19225, 0.19225, 0.19225, 1.0},
    {0.50754, 0.50754, 0.50754, 1.0},
    {0.508273, 0.508273, 0.508273, 1.0},
    51.2};
//プラスチック(黒)
const MaterialColor MaterialColor::black_plastic = {
    "black_plastic",
    {0.0, 0.0, 0.0, 1.0},
    {0.01, 0.01, 0.01, 1.0},
    {0.50, 0.50, 0.50, 1.0},
    32};
//プラスチック(シアン)
const MaterialColor MaterialColor::cyan_plastic = {
    "cyan_plastic",
    {0.0, 0.1, 0.06, 1.0},
    {0.0, 0.50980392, 0.50980392, 1.0},
    {0.50196078, 0.50196078, 0.50196078, 1.0},
    32};
//プラスチック(緑)
const MaterialColor MaterialColor::green_plastic = {
    "green_plastic",
    {0.0, 0.0, 0.0, 1.0},
    {0.1, 0.35, 0.1, 1.0},
    {0.45, 0.55, 0.45, 1.0},
    32};
//プラスチック(赤)
const MaterialColor MaterialColor::red_plastic = {
    "red_plastic",
    {0.0, 0.0, 0.0, 1.0},
    {0.5, 0.0, 0.0, 1.0},
    {0.7, 0.6, 0.6, 1.0},
    32};
//プラスチック(白)
const MaterialColor MaterialColor::white_plastic = {
    "white_plastic",
    {0.0, 0.0, 0.0, 1.0},
    {0.55, 0.55, 0.55, 1.0},
    {0.70, 0.70, 0.70, 1.0},
    32};
//プラスチック(黄)
const MaterialColor MaterialColor::yellow_plastic = {
    "yellow_plastic",
    {0.0, 0.0, 0.0, 1.0},
    {0.5, 0.5, 0.0, 1.0},
    {0.60, 0.60, 0.50, 1.0},
    32};
//ゴム(黒)
const MaterialColor MaterialColor::black_rubber = {
    "black_rubber",
    {0.02, 0.02, 0.02, 1.0},
    {0.01, 0.01, 0.01, 1.0},
    {0.4, 0.4, 0.4, 1.0},
    10.0};
//ゴム(シアン)
const MaterialColor MaterialColor::cyan_rubber = {
    "cyan_rubber",
    {0.0, 0.05, 0.05, 1.0},
    {0.4, 0.5, 0.5, 1.0},
    {0.04, 0.7, 0.7, 1.0},
    10.0};
//ゴム(緑)
const MaterialColor MaterialColor::green_rubber = {
    "green_rubber",
    {0.0, 0.05, 0.0, 1.0},
    {0.4, 0.5, 0.4, 1.0},
    {0.04, 0.7, 0.04, 1.0},
    10.0};
//ゴム(赤)
const MaterialColor MaterialColor::red_rubber = {
    "red_rubber",
    {0.05, 0.0, 0.0, 1.0},
    {0.5, 0.4, 0.4, 1.0},
    {0.7, 0.04, 0.04, 1.0},
    10.0};
//ゴム(白)
const MaterialColor MaterialColor::white_rubber = {
    "white_rubber",
    {0.05, 0.05, 0.05, 1.0},
    {0.5, 0.5, 0.5, 1.0},
    {0.7, 0.7, 0.7, 1.0},
    10.0};
//ゴム(黄)
const MaterialColor MaterialColor::yellow_rubber = {
    "yellow_rubber",
    {0.05, 0.05, 0.0, 1.0},
    {0.5, 0.5, 0.4, 1.0},
    {0.7, 0.7, 0.04, 1.0},
    10.0};
