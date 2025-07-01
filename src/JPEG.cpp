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

void JPEG::processarCanal(const cv::Mat &canal) {
    cv::Mat matrizComPadding = padding(canal);

    // Percorrendo os blocos de 8x8
    for (int i = 0; i < matrizComPadding.rows; i += 8) {
        for (int j = 0; j < matrizComPadding.cols; j += 8) {
        
        // Extraindo o bloco 8x8
        cv::Rect regiaoDoBloco(j, i, 8, 8);
        cv::Mat bloco8x8 = matrizComPadding(regiaoDoBloco).clone();
        
        // Level shift
        cv::Mat blocoShift;
        bloco8x8.convertTo(blocoShift, CV_32F); // Converter para float pois a matriz original é uint8_t
        blocoShift -= 128;

        // DCT
        cv::Mat blocoDCT;
        cv::dct(blocoShift, blocoDCT); // Aplica a DCT

        // Quantização
        cv::Mat blocoQuantizado;
        blocoQuantizado = quantizarBloco(blocoDCT);
        
    }
}
}

cv::Mat JPEG::padding(const cv::Mat &matriz) {
    size_t linhas = matriz.rows;
    size_t colunas = matriz.cols;

    // Calcula o padding para cada linha (pode ser 0)
    size_t padding_linhas = (8 - (linhas % 8)) % 8;
    size_t padding_colunas = (8 - (colunas % 8)) % 8;

    cv::Mat matrizComPadding;

    // Copia a matriz antiga para a nova realizando o padding (para direita e para baixo, quando necessário)
    // replicando a última linha/coluna
    cv::copyMakeBorder(matriz, matrizComPadding, 0, padding_linhas, 0, padding_colunas, cv::BORDER_REPLICATE);

    return matrizComPadding;
}

cv::Mat JPEG::quantizarBloco(cv::Mat bloco, cv::Mat tabelaQuantizacao) {
    cv::Mat blocoQuantizado;


}
