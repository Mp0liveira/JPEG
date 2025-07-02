#include "../includes/JPEG.h"
#include <vector>

JPEG::JPEG(const std::string &inputFile) {

    this->fileName = inputFile;
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

    // --- 2. Inicialização das Tabelas de Huffman ---

    // Dados brutos (bits e huffval)
    std::vector<int> bitsLuminanciaDC = {0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
    std::vector<unsigned char> huffvalLuminanciaDC = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    
    std::vector<int> bitsCrominanciaDC = {0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
    std::vector<unsigned char> huffvalCrominanciaDC = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    
    std::vector<int> bitsLuminanciaAC = {0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 125};
    std::vector<unsigned char> huffvalLuminanciaAC = {
        0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 
        0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0, 
        0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28, 
        0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 
        0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
        0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 
        0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 
        0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5, 
        0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2, 
        0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 
        0xF9, 0xFA
    };
    std::vector<int> bitsCrominanciaAC = {0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 119};
    std::vector<unsigned char> huffvalCrominanciaAC = {
        0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 
        0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0, 
        0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26, 
        0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 
        0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 
        0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 
        0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 
        0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 
        0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 
        0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 
        0xF9, 0xFA
    };
    
    // Gerando e armazenando as tabelas de Huffman
    this->tabelaLuminanciaDC = gerarTabelaHuffman(bitsLuminanciaDC, huffvalLuminanciaDC);
    this->tabelaCrominanciaDC = gerarTabelaHuffman(bitsCrominanciaDC, huffvalCrominanciaDC);
    this->tabelaLuminanciaAC = gerarTabelaHuffman(bitsLuminanciaAC, huffvalLuminanciaAC);
    this->tabelaCrominanciaAC = gerarTabelaHuffman(bitsCrominanciaAC, huffvalCrominanciaAC);

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
    int diferenca = 0;
    int DCAnterior = 0;

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

        // Codificação DC
        int DCAtual = blocoQuantizado.at<int>(0, 0);
        diferenca = DCAtual - DCAnterior;
        DCAnterior = DCAtual;

        int categoria = getCategoriaDC(diferenca);
        std::string codigoHuffmanDC;

        if (tipo == TipoCanal::Luminancia) {
            codigoHuffmanDC = this->tabelaLuminanciaDC[categoria];
        } else if (tipo == TipoCanal::Crominancia) {
            codigoHuffmanDC = this->tabelaCrominanciaDC[categoria];
        }

        std::string valorDC = getValorDC(diferenca, categoria);
        std::string finalDC = codigoHuffmanDC + valorDC;

        // Codificação AC

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

cv::Mat JPEG::quantizarBloco(const cv::Mat &bloco, TipoCanal tipo) {
    cv::Mat temp;

    // Dividindo cada elemento de Aij pelo correspondente Bij
    // A tabela de quantização correta é escolhida de acordo com seu tipo
    if (tipo == TipoCanal::Luminancia) {
        temp = bloco / this->tabelaLuminancia;
    } else if (tipo == TipoCanal::Crominancia) {
        temp = bloco / this->tabelaCrominancia;
    }

    cv::Mat blocoQuantizadoFloat = temp.clone();

    for (int i = 0; i < blocoQuantizadoFloat.rows; ++i) {
        // Pega um ponteiro para o início da linha 'i'
        float* ptr_linha = blocoQuantizadoFloat.ptr<float>(i);

        // Itera sobre cada coluna da linha atual usando o ponteiro
        for (int j = 0; j < blocoQuantizadoFloat.cols; ++j) {
            // Aplica a função de arredondamento diretamente no elemento
            ptr_linha[j] = std::round(ptr_linha[j]);
        }
    }

    cv::Mat blocoQuantizado;
    blocoQuantizadoFloat.convertTo(blocoQuantizado, CV_32S);

    return blocoQuantizado;
}

std::map<int, std::string> JPEG::gerarTabelaHuffman(const std::vector<int>& bits, const std::vector<unsigned char>& huffval) {
    std::map<int, std::string> tabela;
    int codigoAtual = 0;
    int ponteiroHuffval = 0;

    for (int i = 0; i < 16; ++i) {
        int comprimentoBits = i + 1;
        int numCodigos = bits[i];

        for (int j = 0; j < numCodigos; ++j) {
            unsigned char simbolo = huffval[ponteiroHuffval];
            
            // Converter inteiro 'codigoAtual' para uma string binária
            std::string codigoBinario = "";
            int tempCodigo = codigoAtual;
            for(int k = 0; k < comprimentoBits; ++k) {
                codigoBinario = ((tempCodigo & 1) ? "1" : "0") + codigoBinario;
                tempCodigo >>= 1;
            }
            
            tabela[simbolo] = codigoBinario;
            ponteiroHuffval++;
            codigoAtual++;
        }
        codigoAtual <<= 1;
    }
    return tabela;
}

int JPEG::getCategoriaDC(int diferenca) {

    if (diferenca == 0) {
        return 0;
    }

    int moduloDiferenca = std::abs(diferenca);

    return static_cast<int>(std::floor(std::log2(moduloDiferenca))) + 1;
}

std::string getValorDC(int diferenca, int categoria) {
    if (categoria == 0) {
        return "";
    }

    int valorCodificar;
    if (diferenca > 0) {
        valorCodificar = diferenca;
    } else {
        // Se for negativo, faz complemento de 1
        valorCodificar = diferenca + (1 << categoria) - 1;
    }

    std::string valorDC = "";
    for (int i = 0; i < categoria; ++i) {
        valorDC = ((valorCodificar & 1) ? "1" : "0") + valorDC;
        valorCodificar >>= 1;
    }

    return valorDC;
}