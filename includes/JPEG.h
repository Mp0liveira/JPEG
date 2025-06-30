#ifndef JPEG_H
#define JPEG_H

#include <string>

class JPEG {
private:
    std::string fileName;
    void JPEG::processarCanal(cv::Mat canal);

public:
    void compressaoJPEG();
    JPEG(const std::string &inputFile) : fileName(inputFile);

}

#endif //JPEG_H