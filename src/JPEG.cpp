#include "../includes/JPEG.h"
#include <vector>
#include <filesystem>

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
    tempLuminancia.convertTo(this->tabelaLuminancia, CV_32F);

    cv::Mat tempCrominancia(8, 8, CV_8UC1, dadosCrominancia);
    tempCrominancia.convertTo(this->tabelaCrominancia, CV_32F);

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
    std::string bitstream = "";
    uint8_t canais = imagemOriginal.channels();
    uint16_t altura = imagemOriginal.rows;
    uint16_t largura = imagemOriginal.cols;

    // Verificar o número de canais (imagem em escala de cinza, RGB ou RGBA)
    if (canais == 1) {
        std::cout << "Imagem em tom de cinza detectada. Iniciando compressão..." << std::endl;
        imagemParaProcessar = imagemOriginal;
        bitstream += processarCanal(imagemParaProcessar, TipoCanal::Luminancia);

    } else if (canais == 3) {
        std::cout << "Imagem em escala RGB. Iniciando compressão..." << std::endl;
        cv::cvtColor(imagemOriginal, imagemParaProcessar, cv::COLOR_BGR2YCrCb);

        // Separa os canais Y, Cr, Cb
        std::vector<cv::Mat> canaisImagem;
        cv::split(imagemParaProcessar, canaisImagem);

        cv::Mat Y = canaisImagem[0];
        cv::Mat Cr = canaisImagem[1];
        cv::Mat Cb = canaisImagem[2];
        
        // Processar cada canal individualmente
        bitstream += processarCanal(Y, TipoCanal::Luminancia);
        bitstream += processarCanal(Cr, TipoCanal::Crominancia);
        bitstream += processarCanal(Cb, TipoCanal::Crominancia);

    } else if (canais == 4) {
        std::cout << "Imagem em escalar RGBA. Iniciando compressão..." << std::endl;
        canais = 3;
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
        bitstream += processarCanal(Y, TipoCanal::Luminancia);
        bitstream += processarCanal(Cr, TipoCanal::Crominancia);
        bitstream += processarCanal(Cb, TipoCanal::Crominancia);
    }
    std::string header = writeHeader(largura, altura, canais);
    bitstream = header + bitstream;
    writeFile(bitstream);
}

std::string JPEG::processarCanal(const cv::Mat &canal, TipoCanal tipo) {
    cv::Mat matrizComPadding = padding(canal);
    int diferenca = 0;
    int DCAnterior = 0;
    std::string bitstreamCanal = "";

    // Percorrendo os blocos de 8x8
    for (int i = 0; i < matrizComPadding.rows; i += 8) {
        for (int j = 0; j < matrizComPadding.cols; j += 8) {
        
        // Extraindo o bloco 8x8
        cv::Rect regiaoDoBloco(j, i, 8, 8);
        cv::Mat bloco8x8 = matrizComPadding(regiaoDoBloco).clone();
        //std::cout << "bloco 8x8" << std::endl;
        
        // Level shift
        cv::Mat blocoShift;
        bloco8x8.convertTo(blocoShift, CV_32F); // Converter para float pois a matriz original é uint8_t
        blocoShift -= 128;
        //std::cout << "level shift" << std::endl;

        // DCT
        cv::Mat blocoDCT;
        cv::dct(blocoShift, blocoDCT); // Aplica a DCT
        //std::cout << "cdt" << std::endl;

        // Quantização
        cv::Mat blocoQuantizado;
        blocoQuantizado = quantizarBloco(blocoDCT, tipo);
        //std::cout << "quantizacao" << std::endl;

        // Codificação DC
        int DCAtual = blocoQuantizado.at<int>(0, 0);
        diferenca = DCAtual - DCAnterior;
        DCAnterior = DCAtual;

        int categoria = getCategoria(diferenca);
        std::string codigoHuffmanDC;

        if (tipo == TipoCanal::Luminancia) {
            codigoHuffmanDC = this->tabelaLuminanciaDC[categoria];
        } else if (tipo == TipoCanal::Crominancia) {
            codigoHuffmanDC = this->tabelaCrominanciaDC[categoria];
        }

        std::string valorDC = getValor(diferenca, categoria);
        std::string finalDC = codigoHuffmanDC + valorDC;

        // Codificação AC
        std::vector<int> coeficientesZigZag = zigzagAC(blocoQuantizado);
        //std::cout << "zigzag" << std::endl;
        std::string finalAC = codificarAC(coeficientesZigZag, tipo);

        bitstreamCanal += finalDC;
        bitstreamCanal += finalAC;
        //std::cout << "ac" << std::endl;
        }
    }

    return bitstreamCanal;
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
        float* ptr_linha = blocoQuantizadoFloat.ptr<float>(i);

        for (int j = 0; j < blocoQuantizadoFloat.cols; ++j) {
            // Arredondar pro inteiro mais próximo é a mesma coisa que a operação de +0.5 e fazer um floor
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

int JPEG::getCategoria(const int &diferenca) {

    if (diferenca == 0) {
        return 0;
    }

    int moduloDiferenca = std::abs(diferenca);

    return static_cast<int>(std::floor(std::log2(moduloDiferenca))) + 1;
}

std::string JPEG::getValor(const int &diferenca, const int &categoria) {
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

    std::string valor = "";
    for (int i = 0; i < categoria; ++i) {
        valor = ((valorCodificar & 1) ? "1" : "0") + valor;
        valorCodificar >>= 1;
    }

    return valor;
}

std::vector<int> JPEG::zigzagAC(const cv::Mat &blocoQuantizado) {
    const int coordenadasZigZag[63][2] = {
        {0,1}, {1,0}, {2,0}, {1,1}, {0,2}, {0,3}, {1,2}, {2,1},
        {3,0}, {4,0}, {3,1}, {2,2}, {1,3}, {0,4}, {0,5}, {1,4},
        {2,3}, {3,2}, {4,1}, {5,0}, {6,0}, {5,1}, {4,2}, {3,3},
        {2,4}, {1,5}, {0,6}, {0,7}, {1,6}, {2,5}, {3,4}, {4,3},
        {5,2}, {6,1}, {7,0}, {7,1}, {6,2}, {5,3}, {4,4}, {3,5},
        {2,6}, {1,7}, {2,7}, {3,6}, {4,5}, {5,4}, {6,3}, {7,2},
        {7,3}, {6,4}, {5,5}, {4,6}, {3,7}, {4,7}, {5,6}, {6,5},
        {7,4}, {7,5}, {6,6}, {5,7}, {6,7}, {7,6}, {7,7}
    };

    std::vector<int> coeficientesZigZag;
    coeficientesZigZag.reserve(63);

    for (int i = 0; i < 63; i++) {
        int linha = coordenadasZigZag[i][0];
        int coluna = coordenadasZigZag[i][1];
        coeficientesZigZag.push_back(blocoQuantizado.at<int>(linha, coluna));
    }

    return coeficientesZigZag;
}

std::string JPEG::codificarAC(std::vector<int> coeficientesZigZag, TipoCanal tipo) {
    std::string finalAC = "";

    int qtdZeros = 0;

    for (int i = 0; i < 63; i++) {
        if (coeficientesZigZag[i] == 0) {
            qtdZeros++;
        } else {
            while (qtdZeros >= 16) {
                // Símbolo ZRL é 0xF0
                if (tipo == TipoCanal::Luminancia) {
                    finalAC += this->tabelaLuminanciaAC[0xF0];
                } else {
                    finalAC += this->tabelaCrominanciaAC[0xF0];
                }
                qtdZeros -= 16;
            }

            // Já codifica o coeficiente
            int categoria = getCategoria(coeficientesZigZag[i]);
            // Forma o símbolo Z/C (zeros restantes, categoria)
            uint8_t simbolo = (qtdZeros << 4) | categoria;

            // Código de Huffman para o símbolo
            if (tipo == TipoCanal::Luminancia) {
                finalAC += this->tabelaLuminanciaAC[simbolo];
            } else if (tipo == TipoCanal::Crominancia) {
                finalAC += this->tabelaCrominanciaAC[simbolo];
            }
            
            finalAC += getValor(coeficientesZigZag[i], categoria);

            qtdZeros = 0;
        }
    }

    // Símbolo EOB (0X00)
    if (qtdZeros > 0) {
        if (tipo == TipoCanal::Luminancia) {
            finalAC += this->tabelaLuminanciaAC[0x00];
        } else {
            finalAC += this->tabelaCrominanciaAC[0x00];
        }
    }
    
    return finalAC;
}

std::string JPEG::writeHeader(uint16_t largura, uint16_t altura, uint8_t canais) {
    // Header = canais + altura + largura
    std::string header = "";

    for (int i = 0; i < 16; i++) {
        header = ((largura & 1) ? "1" : "0") + header;
        largura >>= 1;
    }

    for (int i = 0; i < 16; i++) {
        header = ((altura & 1) ? "1" : "0") + header;
        altura >>= 1;
    }

    for (int i = 0; i < 8; i++) {
        header = ((canais & 1) ? "1" : "0") + header;
        canais >>= 1;
    }

    return header;
}

void JPEG::writeFile(const std::string &bitstream) {
    Bitstream bs;

    // Escrevendo o conteúdo codificado
    for (char c : bitstream) {
        if (c == '0') {
            bs.writeBit(false); // Escreve 0
        } else {
            bs.writeBit(true); // Escreve 1
        }
    }

    std::filesystem::path imagem(this->fileName);
    std::string caminhoParaSalvar = "../ArquivosComprimidos/" + imagem.stem().string() + ".bin";

    bs.flushesToFile(caminhoParaSalvar);
}

/// -------------------------------------------------------------------------------------------------- ///

JPEGdecoder::JPEGdecoder(const std::string &inputFile) {

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
    tempLuminancia.convertTo(this->tabelaLuminancia, CV_32F);

    cv::Mat tempCrominancia(8, 8, CV_8UC1, dadosCrominancia);
    tempCrominancia.convertTo(this->tabelaCrominancia, CV_32F);
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
    this->tabelaDecodeLuminanciaDC = gerarDecoderHuffman(bitsLuminanciaDC, huffvalLuminanciaDC);
    this->tabelaDecodeCrominanciaDC = gerarDecoderHuffman(bitsCrominanciaDC, huffvalCrominanciaDC);
    this->tabelaDecodeLuminanciaAC = gerarDecoderHuffman(bitsLuminanciaAC, huffvalLuminanciaAC);
    this->tabelaDecodeCrominanciaAC = gerarDecoderHuffman(bitsCrominanciaAC, huffvalCrominanciaAC);
}

void JPEGdecoder::arrayBits(Bitstream& bs) {
    uint64_t size = bs.totalSize();

    for (uint64_t i = 0; i < size; i++) {
        bool bit = bs.readBit();
        this->inputBuffer += bit ? '1' : '0';
    }
}

std::map<std::string, int> JPEGdecoder::gerarDecoderHuffman(const std::vector<int>& bits, const std::vector<unsigned char>& huffval) {
    std::map<std::string, int> tabela;
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
            
            tabela[codigoBinario] = simbolo;
            ponteiroHuffval++;
            codigoAtual++;
        }
        codigoAtual <<= 1;
    }
    return tabela;
}

void JPEGdecoder::lerHeader() {
    // Header = canais + altura + largura
    const int TAMANHO_HEADER = 8 + 16 + 16; // 40 bits

    std::string canais = inputBuffer.substr(0, 8);
    std::string altura = inputBuffer.substr(8, 16);
    std::string largura = inputBuffer.substr(24, 16);

    this->canais = static_cast<uint8_t>(std::stoi(canais, nullptr, 2));
    this->altura = static_cast<uint16_t>(std::stoi(altura, nullptr, 2));
    this->largura = static_cast<uint16_t>(std::stoi(largura, nullptr, 2));

    this->posicaoLeitura = TAMANHO_HEADER;

}

int JPEGdecoder::decodificaSimbolo(TipoDecoder tipo) {
    std::string codigoAtual = "";
    std::map<std::string, int> tabelaDecode;
    int tipoPRint;

    switch (tipo) {
    case TipoDecoder::LuminanciaDC:
        tabelaDecode = tabelaDecodeLuminanciaDC;
        tipoPRint = 0;
        break;
    case TipoDecoder::LuminanciaAC:
        tabelaDecode = tabelaDecodeLuminanciaAC;
        tipoPRint = 1;
        break;
    case TipoDecoder::CrominanciaDC:
        tabelaDecode = tabelaDecodeCrominanciaDC;
        tipoPRint = 2;
        break;
    case TipoDecoder::CrominanciaAC:
        tabelaDecode = tabelaDecodeCrominanciaAC;
        tipoPRint = 3;
        break;
    }

    while (this->posicaoLeitura < inputBuffer.length()) {
        codigoAtual += inputBuffer[this->posicaoLeitura];
        this->posicaoLeitura++;
        int categoria;

        auto it = tabelaDecode.find(codigoAtual);
        if (it != tabelaDecode.end()) {
            categoria = it->second;
            return categoria;
        }  
    }
    //std::cout << "STRING NAO ENCOTNRADA NA TABELA: " << codigoAtual << std::endl;
    // Se retornar -1 tem alguma coisa muito errada...
    return -1;
}

int JPEGdecoder::decodificaValor(const std::string &valorString, const int &categoria) {
    if (categoria == 0) {
        return 0;
    }

    int valor = std::stoi(valorString, nullptr, 2);

    // 1° bit = 1 -> próprio valor, caso contrário é o complemento de 1
    if (valorString[0] == '0') {
        int range = 1 << categoria;
        valor = valor - (range - 1);
    }

    this->posicaoLeitura += categoria;

    return valor;
}

cv::Mat JPEGdecoder::preencherBloco(const int &DCAtual, const std::vector<int> &coeficientesZigZag) {
    cv::Mat blocoQuantizado(8, 8, CV_32S);
    const int coordenadasZigZag[63][2] = {
        {0,1}, {1,0}, {2,0}, {1,1}, {0,2}, {0,3}, {1,2}, {2,1},
        {3,0}, {4,0}, {3,1}, {2,2}, {1,3}, {0,4}, {0,5}, {1,4},
        {2,3}, {3,2}, {4,1}, {5,0}, {6,0}, {5,1}, {4,2}, {3,3},
        {2,4}, {1,5}, {0,6}, {0,7}, {1,6}, {2,5}, {3,4}, {4,3},
        {5,2}, {6,1}, {7,0}, {7,1}, {6,2}, {5,3}, {4,4}, {3,5},
        {2,6}, {1,7}, {2,7}, {3,6}, {4,5}, {5,4}, {6,3}, {7,2},
        {7,3}, {6,4}, {5,5}, {4,6}, {3,7}, {4,7}, {5,6}, {6,5},
        {7,4}, {7,5}, {6,6}, {5,7}, {6,7}, {7,6}, {7,7}
    };

    blocoQuantizado.at<int>(0,0) = DCAtual;

    for (int i = 0; i < 63; i++) {
        int linha = coordenadasZigZag[i][0];
        int coluna = coordenadasZigZag[i][1];
        blocoQuantizado.at<int>(linha, coluna) = coeficientesZigZag[i];
    }

    return blocoQuantizado;
}

cv::Mat JPEGdecoder::desquantizarBloco(const cv::Mat &blocoQuantizado, TipoCanal tipo) {
    cv::Mat tabelaQuantizacao;
    cv::Mat blocoDesquantizado;
    cv::Mat blocoQuantizadoFloat;
    blocoQuantizado.convertTo(blocoQuantizadoFloat, CV_32F);

    if (tipo == TipoCanal::Luminancia) {
        tabelaQuantizacao = this->tabelaLuminancia;
    } else if (tipo == TipoCanal::Crominancia) {
        tabelaQuantizacao = this->tabelaCrominancia;
    }

    blocoDesquantizado = blocoQuantizadoFloat.mul(tabelaQuantizacao);

    return blocoDesquantizado;
}

void JPEGdecoder::descompressaoJPEG() {
    std::string caminhoImagem = "../ArquivosComprimidos/" + this->fileName;
    // Biblioteca auxiliar para ler os bits
    Bitstream bs(caminhoImagem);
    // Preencher inputBuffer
    arrayBits(bs);
    // Ler as informações do header e posicionar a variável auxiliar de ponteiro para o início do conteúdo "útil" do bitstream
    lerHeader();
    // Armazena as camadas reconstruídas na ordem Y, Cr e Cb, se existirem
    std::vector<cv::Mat> camadasReconstruidas;

    int DCAnterior = 0;
    // Iterar sobre cada quantidade de camada
    for (int i = 0; i < canais; i++) {

        int alturaComPadding = (this->altura + 7) & -8;
        int larguraComPadding = (this->largura + 7) & -8;
        cv::Mat canalReconstruido(alturaComPadding, larguraComPadding, CV_32F);
        // Zera para o cálculo de cada canal i
        DCAnterior = 0;

        for (int y = 0; y < alturaComPadding; y += 8) {
            for (int x = 0; x < larguraComPadding; x += 8) {
                //std::cout << "Processando o bloco: " << y << ", " << x << " do canal: " << (int)i << std::endl;
                // Decodificar um bloco -> 64 coeficientes (1DC e 63 AC)
                // Primeira coeficiente-> Tabela DC
                int categoriaDC;
                TipoCanal tipo;
                // i = 0 significa luminância
                if (i == 0) {
                    categoriaDC = decodificaSimbolo(TipoDecoder::LuminanciaDC);
                    tipo = TipoCanal::Luminancia;
                } else {
                    categoriaDC = decodificaSimbolo(TipoDecoder::CrominanciaDC);
                    tipo = TipoCanal::Crominancia;
                }

                // Decodifica a diferença
                std::string diferencaString = inputBuffer.substr(this->posicaoLeitura, categoriaDC);
                int diferencaDC = decodificaValor(diferencaString, categoriaDC);
                int DCAtual = DCAnterior + diferencaDC;
                DCAnterior = DCAtual;

                // Próximos 63 coeficientes -> Tabela AC
                std::vector<int> coeficientesZigZag;

                while(coeficientesZigZag.size() < 63) {
                    // Lembrando: símbolo = Z/C -> 4 bits / 4 bits
                    uint8_t simbolo; 
                    if (i == 0) {
                        simbolo = decodificaSimbolo(TipoDecoder::LuminanciaAC);
                    } else {
                        simbolo = decodificaSimbolo(TipoDecoder::CrominanciaAC);
                    }

                    // 4 bits superiores
                    int qtdZeros = (0xF0 & simbolo) >> 4;
                    // 4 bits inferiores
                    int categoriaAC = (0x0F & simbolo);
                    
                    // Verificar condições especiais (ZRL e EOB)
                    if (simbolo == 0xF0) { // ZRL
                        coeficientesZigZag.insert(coeficientesZigZag.end(), 16, 0);
                    } else if (simbolo == 0x00) { // EOB
                        int posicoesRestantes = 63 - coeficientesZigZag.size();
                        coeficientesZigZag.insert(coeficientesZigZag.end(), posicoesRestantes, 0);
                    } else {
                        // Indica o próximo valor que vem depois dos 0s inseridos
                        std::string valorString = inputBuffer.substr(this->posicaoLeitura, categoriaAC);
                        int valorAC = decodificaValor(valorString, categoriaAC);
                        // Adicionando a quantidade de 0s
                        coeficientesZigZag.insert(coeficientesZigZag.end(), qtdZeros, 0);
                        coeficientesZigZag.push_back(valorAC);
                    }

                }

                // Preencher o bloco 8x8
                cv::Mat blocoQuantizado = preencherBloco(DCAtual, coeficientesZigZag);

                // Desquantização
                cv::Mat blocoDesquantizado;
                blocoDesquantizado = desquantizarBloco(blocoQuantizado, tipo);

                // DCT⁻¹ e Shift Inverso (bloco 8x8 reconstruído)
                cv::Mat blocoShift;
                cv::dct(blocoDesquantizado, blocoShift, cv::DCT_INVERSE);
                blocoShift += 128;
                
                // Preenchendo a matriz do canal com o bloco 8x8
                cv::Rect regiaoDoBloco(x, y, 8, 8);
                blocoShift.copyTo(canalReconstruido(regiaoDoBloco));

            }
        }

        camadasReconstruidas.push_back(canalReconstruido);
    }
    /*
    if (camadasReconstruidas.size() == 3) {
    std::cout << "Iniciando a visualizacao dos canais individuais..." << std::endl;

    // --- Canal Y (Luminancia) ---
    cv::Mat canal_Y_vis;
    // Converte de float para uchar (0-255) para poder salvar.
    camadasReconstruidas[0].convertTo(canal_Y_vis, CV_8U);
    cv::imwrite("../ArquivosDescomprimidos/" + this->fileName + "_1_Canal_Y.png", canal_Y_vis);
    
    // --- Canal Cr (Croma Vermelho) ---
    cv::Mat canal_Cr_vis;
    camadasReconstruidas[1].convertTo(canal_Cr_vis, CV_8U);
    cv::imwrite("../ArquivosDescomprimidos/" + this->fileName + "_2_Canal_Cr.png", canal_Cr_vis);

    // --- Canal Cb (Croma Azul) ---
    cv::Mat canal_Cb_vis;
    camadasReconstruidas[2].convertTo(canal_Cb_vis, CV_8U);
    cv::imwrite("../ArquivosDescomprimidos/" + this->fileName + "_3_Canal_Cb.png", canal_Cb_vis);

    std::cout << "Canais individuais salvos para depuracao." << std::endl;
    }
    */
    cv::Mat imagemComPadding;

    if (camadasReconstruidas.size() > 1) {
        // Juntando os canais novamente
        cv::Mat ycrcbFloat;
        cv::merge(camadasReconstruidas, ycrcbFloat);
        
        // Converter para 8U antes de chamar cvtColor (sem isso, a imagem estava ficando rosa)
        cv::Mat ycrcb8U;
        ycrcbFloat.convertTo(ycrcb8U, CV_8U);

        // Convertendo do formato YCrCb para BGR (OpenCV trabalha com ordem diferente do RGB)
        cv::cvtColor(ycrcb8U, imagemComPadding, cv::COLOR_YCrCb2BGR);
    } else {
        cv::Mat Y = camadasReconstruidas[0];
        Y.convertTo(imagemComPadding, CV_8U);
    }

    cv::Rect blocoOriginal(0, 0, this->largura, this->altura);
    cv::Mat imagemReconstruida = imagemComPadding(blocoOriginal).clone();

    std::filesystem::path imagem(this->fileName);
    std::string caminhoParaSalvar = "../ArquivosDescomprimidos/" + imagem.stem().string() + ".png";

    if (!imagemReconstruida.empty()) {
        cv::imwrite(caminhoParaSalvar, imagemReconstruida);
        std::cout << "Imagem reconstruída com sucesso." << std::endl;
    }
}
