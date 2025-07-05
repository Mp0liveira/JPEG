#ifndef JPEG_H
#define JPEG_H

#include <string>
#include "opencv2/opencv.hpp"
#include "ac_bitstream.h"

// Definição dos tipos de canal
enum class TipoCanal {
    Luminancia,
    Crominancia
};

class JPEG {
private:
    std::string fileName;
    std::string processarCanal(const cv::Mat &canal, TipoCanal tipo);
    cv::Mat padding(const cv::Mat &matriz);
    cv::Mat quantizarBloco(const cv::Mat &bloco, TipoCanal tipo);
    cv::Mat tabelaLuminancia;
    cv::Mat tabelaCrominancia;
    std::map<int, std::string> gerarTabelaHuffman(const std::vector<int>& bits, const std::vector<unsigned char>& huffval);
    // Tabelas de Huffman (mapa de simbolo -> código binário)
    std::map<int, std::string> tabelaLuminanciaDC;
    std::map<int, std::string> tabelaCrominanciaDC;
    std::map<int, std::string> tabelaLuminanciaAC;
    std::map<int, std::string> tabelaCrominanciaAC;
    int getCategoria(const int &diferenca);
    std::string getValor(const int &diferenca, const int &categoria);
    std::vector<int> zigzagAC(const cv::Mat &blocoQuantizado);
    std::string codificarAC(std::vector<int> coeficientesZigZag, TipoCanal tipo);
    std::string writeHeader(uint16_t largura, uint16_t altura, uint8_t canais);
    void writeFile(const std::string &bitstream);

public:
    void compressaoJPEG();
    JPEG(const std::string &inputFile);

};

enum class TipoDecoder {
    LuminanciaDC,
    LuminanciaAC,
    CrominanciaDC,
    CrominanciaAC
};

class JPEGdecoder {
private:
    std::string fileName;
    void arrayBits(Bitstream &bs);
    std::string inputBuffer = "";
    cv::Mat tabelaLuminancia;
    cv::Mat tabelaCrominancia;
    uint8_t canais;
    uint16_t largura;
    uint16_t altura; 
    // Variável que indica o bit que está sendo processado
    size_t posicaoLeitura = 0;
    std::map<std::string, int> gerarDecoderHuffman(const std::vector<int>& bits, const std::vector<unsigned char>& huffval);
    std::map<std::string, int> tabelaDecodeLuminanciaDC;
    std::map<std::string, int> tabelaDecodeCrominanciaDC;
    std::map<std::string, int> tabelaDecodeLuminanciaAC;
    std::map<std::string, int> tabelaDecodeCrominanciaAC;
    void lerHeader();
    int decodificaSimbolo(TipoDecoder tipo);
    int decodificaValor(const std::string &diferencaString, const int &categoria);
    cv::Mat preencherBloco(const int &DCAtual, const std::vector<int> &coeficientesZigZag);
    cv::Mat desquantizarBloco(const cv::Mat &blocoQuantizado, TipoCanal tipo);

public:
    void descompressaoJPEG();
    JPEGdecoder(const std::string &inputFile);
};


#endif //JPEG_H