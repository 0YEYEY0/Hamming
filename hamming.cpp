#include "hammingx.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include <iomanip>

// Calcular el número de bits de paridad necesarios para el código Hamming
int calculateParityBits(int dataBits) {
    int r = 0;
    while ((1 << r) < (dataBits + r + 1)) {
        r++;
    }
    return r;
}

// Comprimir datos simulando compresión básica (reducir tamaño)
std::vector<int> compressData(const std::vector<int>& data) {
    std::vector<int> compressed;
    for (size_t i = 0; i < data.size(); i += 2) {
        int compressedBit = (data[i] + (i + 1 < data.size() ? data[i + 1] : 0)) % 2;
        compressed.push_back(compressedBit);
    }
    return compressed;
}

// Generar un conjunto de datos aleatorios
std::vector<int> generateRandomData(int size) {
    std::vector<int> data(size);
    for (int& bit : data) {
        bit = std::rand() % 2;
    }
    return data;
}

// Codificar datos usando el código Hamming
std::vector<int> encodeHamming(const std::vector<int>& data) {
    int m = data.size();
    int r = calculateParityBits(m);
    int n = m + r;
    std::vector<int> hammingCode(n, 0);

    int j = 0;
    for (int i = 0; i < n; i++) {
        if ((i + 1) & (i)) { // Evitar posiciones de paridad
            hammingCode[i] = data[j++];
        }
    }

    for (int i = 0; (1 << i) <= n; i++) { // Calcular bits de paridad
        int position = (1 << i);
        int parity = 0;
        for (int j = 0; j < n; j++) {
            if ((j + 1) & position) {
                parity ^= hammingCode[j];
            }
        }
        hammingCode[position - 1] = parity;
    }

    return hammingCode;
}

// Generar checksum adicional para validar datos
std::vector<int> generateChecksum(const std::vector<int>& data) {
    std::vector<int> checksum(2, 0); // Checksum de 2 bits
    for (size_t i = 0; i < data.size(); i++) {
        checksum[0] ^= (i % 2 == 0) ? data[i] : 0;
        checksum[1] ^= (i % 2 != 0) ? data[i] : 0;
    }
    return checksum;
}

// Detectar y corregir errores en el código Hamming
int detectAndCorrect(std::vector<int>& hammingCode) {
    int n = hammingCode.size();
    int errorPosition = 0;

    for (int i = 0; (1 << i) <= n; i++) {
        int position = (1 << i);
        int parity = 0;
        for (int j = 0; j < n; j++) {
            if ((j + 1) & position) {
                parity ^= hammingCode[j];
            }
        }
        if (parity != 0) {
            errorPosition += position;
        }
    }

    if (errorPosition != 0) {
        hammingCode[errorPosition - 1] ^= 1; // Corregir el error
    }

    return errorPosition;
}

// Crear un bloque de datos codificado
Block createBlock(int dataSize) {
    Block block;
    block.data = generateRandomData(dataSize);
    block.compressed = compressData(block.data);
    block.hamming = encodeHamming(block.compressed);
    block.checksum = generateChecksum(block.compressed);
    block.received = block.hamming;
    return block;
}

// Introducir errores aleatorios en un bloque
void introduceError(Block& block, double errorProb) {
    block.hasError = (std::rand() / static_cast<double>(RAND_MAX)) < errorProb;
    if (block.hasError) {
        int errorPosition = std::rand() % block.received.size();
        block.received[errorPosition] ^= 1; // Flip del bit
        block.errorPosition = errorPosition + 1; // Guardar posición de error
    }
}

// Mostrar información detallada de un bloque
void visualizeBlock(const Block& block, int blockNumber) {
    std::cout << "\n--- Block " << blockNumber << " ---\n";
    std::cout << "Original Data: ";
    for (int bit : block.data) std::cout << bit;
    std::cout << "\nCompressed Data: ";
    for (int bit : block.compressed) std::cout << bit;
    std::cout << "\nHamming Code:    ";
    for (int bit : block.hamming) std::cout << bit;
    std::cout << "\nReceived Data:   ";
    for (int bit : block.received) std::cout << bit;
    std::cout << "\nChecksum:        ";
    for (int bit : block.checksum) std::cout << bit;
    if (block.hasError) {
        std::cout << "\nError Detected at Position: " << block.errorPosition;
    } else {
        std::cout << "\nNo Errors Detected.";
    }
    std::cout << "\n";
}

// Calcular estadísticas del proceso
std::map<std::string, int> calculateStatistics(const std::vector<Block>& blocks) {
    std::map<std::string, int> stats = {
            {"Total Blocks", static_cast<int>(blocks.size())},
            {"Errors Detected", 0},
            {"Blocks Retransmitted", 0}
    };

    for (const auto& block : blocks) {
        if (block.hasError) stats["Errors Detected"]++;
        if (block.retransmitted) stats["Blocks Retransmitted"]++;
    }

    return stats;
}
