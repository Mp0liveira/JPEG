#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    // Use o caminho para uma imagem de teste.
    std::string caminho_imagem = "../images/kodim03.png";
    cv::Mat imagem = cv::imread(caminho_imagem);

    if (imagem.empty()) {
        std::cout << "Erro: Imagem não encontrada." << std::endl;
        return -1;
    }

    cv::imshow("Imagem no Ubuntu!", imagem);
    cv::waitKey(0);

    return 0;
}