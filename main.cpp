#include <iostream>
#include "includes/JPEG.h"

int main() {
    JPEG jpeg("kodim05.png");
    jpeg.compressaoJPEG();

    JPEGdecoder jpegd("kodim05.bin");
    jpegd.descompressaoJPEG();

    return 0;
}