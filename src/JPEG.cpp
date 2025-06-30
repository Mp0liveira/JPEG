#include "../includes/JPEG.h"
#include "opencv2/opencv.hpp"
#include <vector>

JPEG::JPEG(const std::string &inputFile) : fileName(inputFile) {}

void JPEG::compressaoJPEG() {

    //Abrir a imagem 
    std::string caminhoImagem = "../images/" + fileName;
    cv::Mat imagemOriginal = cv::imread(caminhoImagem);

    // Verificar se a imagem foi carregada corretamente
    if (imagemOriginal.empty()) {
        std::cerr << "Erro ao carregar a imagem" << std::endl;
        return;
    }

    cv::Mat imagemParaProcessar;

    // Verificar o número de canais (imagem em escala de cinza, RGB ou RGBA)
    if (imagemOriginal.channels() == 1) {
        std::cout << "Imagem em tom de cinza detectada. Iniciando compressão..." << std::endl;
        imagemParaProcessar = imagemOriginal;
        // processarCanal(imagemParaProcessar)

    } else if (imagemOriginal.channels() == 3) {
        std::cout << "Imagem em escala RGB. Iniciando compressão..." << std::endl;
        cv::cvtColor(imagemOriginal, imagemParaProcessar, cv::COLOR_BGR2YCrCb);
        // processarCanal(imagemParaProcessar)

    } else if (imagemOriginal.channels() == 4) {
        std::cout << "Imagem em escalar RGBA. Iniciando compressão..." << std::endl;
        cv::Mat temp;
        cv::cvtColor(imagemOriginal, temp, cv::COLOR_BGRA2BGR);
        cv::cvtColor(temp, imagemParaProcessar, cv::COLOR_BGR2YCrCb);
        
        // Separa os canais Y, Cr, Cb
        std::vector<cv::Mat> canaisImagem;
        cv::split(imagemParaProcessar, canaisImagem);

        cv::Mat Y = canaisImagem[0];
        cv::Mat Cr = canaisImagem[1];
        cv::Mat Cb = canaisImagem[2];

        // Processar cada canal individualmente
        // processarCanal(Y)
        // processarCanal(Cr)
        // processarCanal(Cb)
    }
}

void JPEG::processarCanal(cv::Mat canal) {
    
}