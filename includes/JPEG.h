#ifndef JPEG_H
#define JPEG_H

#include <string>
#include "opencv2/opencv.hpp"

// Definição dos tipos de canal
enum class TipoCanal {
    Luminancia,
    Crominancia
};

class JPEG {
private:
    std::string fileName;
    void processarCanal(const cv::Mat &canal, TipoCanal tipo);
    cv::Mat padding(const cv::Mat &matriz);
    cv::Mat quantizarBloco(const cv::Mat &bloco, TipoCanal tipo);
    cv::Mat tabelaLuminancia;
    cv::Mat tabelaCrominancia;

public:
    void compressaoJPEG();
    JPEG(const std::string &inputFile);

};

#endif //JPEG_H