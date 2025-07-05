#include <iostream>
#include "includes/JPEG.h"

int main() {
    JPEG jpeg("kodim03.png");
    jpeg.compressaoJPEG();

    JPEGdecoder jpegd("kodim03.png.bin");
    jpegd.descompressaoJPEG();

    return 0;
}