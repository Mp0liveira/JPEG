#ifndef JPEG_H
#define JPEG_H

#include <string>

class JPEG {
private:
    std::string fileName;
    void JPEG::processarCanal(const cv::Mat &canal);
    cv::Mat JPEG::padding(const cv::Mat &matriz);
    cv::Mat JPEG::quantizarBloco(cv::Mat bloco, cv::Mat tabelaQuantizacao);

public:
    void compressaoJPEG();
    JPEG(const std::string &inputFile) : fileName(inputFile);

};

#endif //JPEG_H