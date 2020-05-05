#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./include/include.h"
#include <chrono>
#include <iostream>
using namespace chrono;

int main(int argc, char **argv) {
// first define the scene , variables , ... 
    auto start = system_clock::now();

    int W = 512;
    int H = 512;
    double f = 350;
    int NB_path = atoi(argv[1]);
    Scene scene(W, H, f);
// then scan all pixels
    vector<uint8_t> img = scene.scan(NB_path, 3).pixels;

// save image and return 0
    std::string name_str = "image/";
    name_str.append(to_string(W));
    name_str.append("_");
    name_str.append(to_string(H));
    name_str.append("_NBpath=");
    name_str.append(to_string(NB_path));
    name_str.append(".png");

    stbi_write_png(name_str.c_str(), W, H, 3, &img[0], 0);

    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    std::cout << "渲染这狗图花费了爷整整" 
    << double(duration.count()) * microseconds::period::num / microseconds::period::den 
    << "秒" << std::endl;
    
    return 0;
}