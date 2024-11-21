#ifndef HAMMINGX_H
#define HAMMINGX_H

#include <vector>
#include <string>
#include <map>

// Estructura para representar un bloque de datos
struct Block {
    std::vector<int> data;         // Datos originales
    std::vector<int> compressed;  // Datos comprimidos
    std::vector<int> hamming;     // Código Hamming generado
    std::vector<int> received;    // Datos recibidos (con posibles errores)
    std::vector<int> checksum;    // Checksum adicional
    bool hasError = false;        // Indica si el bloque tiene errores
    int errorPosition = 0;        // Posición del error detectado (si aplica)
    bool retransmitted = false;   // Indica si el bloque fue retransmitido
};

// Prototipos de funciones
int calculateParityBits(int dataBits);
std::vector<int> compressData(const std::vector<int>& data);
std::vector<int> generateRandomData(int size);
std::vector<int> encodeHamming(const std::vector<int>& data);
std::vector<int> generateChecksum(const std::vector<int>& data);
int detectAndCorrect(std::vector<int>& hammingCode);
Block createBlock(int dataSize);
void introduceError(Block& block, double errorProb);
void visualizeBlock(const Block& block, int blockNumber);
std::map<std::string, int> calculateStatistics(const std::vector<Block>& blocks);

// Funcionalidades nuevas
std::map<std::string, double> analyzePerformance(const std::vector<Block>& blocks);
void processBlockWithVisualization(Block& block, int blockNumber, double errorProb);
void runTests();

#endif // HAMMINGX_H
