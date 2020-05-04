#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "objects.hpp"

int main(int argc, char **argv) {
// first define the scene , variables , ... 
    int W = 512;
    int H = 512;
    double f = 350;
    int NB_path = 500;
    Scene scene(W, H, f);

// then scan all pixels
    uint8_t* img = scene.scan(NB_path).pixels;
// save image and return 0
    
    stbi_write_png("image.png", W, H, 3, &img[0], 0);
    free(scene.image.pixels);
    return 0;
}