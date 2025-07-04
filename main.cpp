#include <iostream>
#include "includes/JPEG.h"

int main() {
    JPEG jpeg("kodim03.png");
    jpeg.compressaoJPEG();

    return 0;
}