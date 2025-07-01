#include "../includes/JPEG.h"
#include <vector>

JPEG::JPEG(const std::string &inputFile) {

    fileName = inputFile;
    // Tabela de quantização para Y 
    unsigned char dadosLuminancia[64] = {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
    };
    // Tabela de quantização para Cr e Cb
    unsigned char dadosCrominancia[64] = {
    17, 18, 24, 47, 99, 99, 99, 99,
    18, 21, 26, 66, 99, 99, 99, 99,
    24, 26, 56, 99, 99, 99, 99, 99,
    47, 66, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99
    };

    cv::Mat tempLuminancia(8, 8, CV_8UC1, dadosLuminancia);
    tempLuminancia.convertTo(tabelaLuminancia, CV_32F);

    cv::Mat tempCrominancia(8, 8, CV_8UC1, dadosCrominancia);
    tempCrominancia.convertTo(tabelaCrominancia, CV_32F);

}

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
        processarCanal(imagemParaProcessar, TipoCanal::Luminancia);

    } else if (imagemOriginal.channels() == 3) {
        std::cout << "Imagem em escala RGB. Iniciando compressão..." << std::endl;
        cv::cvtColor(imagemOriginal, imagemParaProcessar, cv::COLOR_BGR2YCrCb);

        // Separa os canais Y, Cr, Cb
        std::vector<cv::Mat> canaisImagem;
        cv::split(imagemParaProcessar, canaisImagem);

        cv::Mat Y = canaisImagem[0];
        cv::Mat Cr = canaisImagem[1];
        cv::Mat Cb = canaisImagem[2];
        
        // Processar cada canal individualmente
        processarCanal(Y, TipoCanal::Luminancia);
        processarCanal(Cr, TipoCanal::Crominancia);
        processarCanal(Cb, TipoCanal::Crominancia);

    } else if (imagemOriginal.channels() == 4) {
        std::cout << "Imagem em escalar RGBA. Iniciando compressão..." << std::endl;
        cv::Mat temp;
        // Converte para 3 canais (OpenCV usa BGR ao invés de RGB)
        cv::cvtColor(imagemOriginal, temp, cv::COLOR_BGRA2BGR);
        cv::cvtColor(temp, imagemParaProcessar, cv::COLOR_BGR2YCrCb);
        
        // Separa os canais Y, Cr, Cb
        std::vector<cv::Mat> canaisImagem;
        cv::split(imagemParaProcessar, canaisImagem);

        cv::Mat Y = canaisImagem[0];
        cv::Mat Cr = canaisImagem[1];
        cv::Mat Cb = canaisImagem[2];

        // Processar cada canal individualmente
        processarCanal(Y, TipoCanal::Luminancia);
        processarCanal(Cr, TipoCanal::Crominancia);
        processarCanal(Cb, TipoCanal::Crominancia);
    }
}

void JPEG::processarCanal(const cv::Mat &canal, TipoCanal tipo) {
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
        blocoQuantizado = quantizarBloco(blocoDCT, tipo);
        
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

cv::Mat JPEG::quantizarBloco(cv::Mat bloco, TipoCanal tipo) {
    cv::Mat blocoQuantizado;

    // Dividindo cada elemento de Aij pelo correspondente Bij
    // A tabela de quantização correta é escolhida de acordo com seu tipo
    if (tipo == TipoCanal::Luminancia) {
        blocoQuantizado = bloco / this->tabelaLuminancia;
    } else if (tipo == TipoCanal::Crominancia) {
        blocoQuantizado = bloco / this->tabelaCrominancia;
    }

    return blocoQuantizado;
}
