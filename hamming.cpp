#include "hammingx.h"
#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <map>

// Calcular el número de bits de paridad necesarios
int calculateParityBits(int dataBits) {
    int r = 0;
    while ((1 << r) < (dataBits + r + 1)) {
        r++;
    }
    return r;
}

// Generar código Hamming para los datos de entrada
std::vector<int> encodeHamming(const std::vector<int>& data) {
    int m = data.size();
    int r = calculateParityBits(m);
    int n = m + r;

    std::vector<int> hammingCode(n, 0);
    int j = 0;

    // Insertar los bits de datos en las posiciones del código Hamming
    for (int i = 0; i < n; i++) {
        if ((i + 1) && ((i + 1) & i) == 0) {
            continue; // Posición de bit de paridad
        }
        hammingCode[i] = data[j++];
    }

    // Calcular los bits de paridad
    for (int i = 0; i < r; i++) {
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

// Introducir errores aleatorios en un bloque de datos con una probabilidad dada
void introduceError(Block& block, double errorProb) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Introducir múltiples errores si el valor aleatorio es menor que la probabilidad de error
    for (int i = 0; i < block.hamming.size(); ++i) {
        if (dis(gen) < errorProb) {
            block.hamming[i] ^= 1; // Cambiar un bit al azar
            block.hasError = true;
            std::cout << "Error en la posicion: " << i + 1 << "\n";
        }
    }
}

// Visualizar un bloque de datos
void visualizeBlock(const Block& block, int blockNumber) {
    std::cout << "Bloque " << blockNumber << ":\n";
    std::cout << "Datos Originales: ";
    for (int bit : block.data) {
        std::cout << bit;
    }
    std::cout << "\nCódigo Hamming: ";
    for (int bit : block.hamming) {
        std::cout << bit;
    }
    std::cout << "\nDatos Recibidos: ";
    for (int bit : block.received) {
        std::cout << bit;
    }
    std::cout << "\nChecksum: ";
    for (int bit : block.checksum) {
        std::cout << bit;
    }
    std::cout << "\n\n";
}

// Crear un bloque de datos con un tamaño dado
Block createBlock(int dataSize) {
    Block block;
    block.data = generateRandomData(dataSize);
    block.received = block.data;
    return block;
}

// Generar datos aleatorios de un tamaño dado
std::vector<int> generateRandomData(int size) {
    std::vector<int> data(size);
    for (int i = 0; i < size; i++) {
        data[i] = std::rand() % 2; // Generar 0 o 1 aleatoriamente
    }
    return data;
}

// Generar un checksum simple para el bloque de datos
std::vector<int> generateChecksum(const std::vector<int>& data) {
    std::vector<int> checksum;
    int sum = 0;
    for (int bit : data) {
        sum += bit;
    }
    checksum.push_back(sum % 2);  // Simple checksum (paridad)
    return checksum;
}

// Detectar y corregir errores en el código Hamming
int detectAndCorrect(std::vector<int>& hammingCode) {
    int n = hammingCode.size();
    int errorPosition = 0;

    // Verificar los bits de paridad y detectar errores
    for (int i = 0; i < std::log2(n) + 1; i++) {
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

    // Corregir el error si es necesario
    if (errorPosition != 0) {
        hammingCode[errorPosition - 1] ^= 1;
    }

    return errorPosition;
}

// Calcular estadísticas de un conjunto de bloques
std::map<std::string, int> calculateStatistics(const std::vector<Block>& blocks) {
    std::map<std::string, int> stats;
    int totalErrors = 0;
    int totalRetransmitted = 0;

    for (const auto& block : blocks) {
        if (block.hasError) totalErrors++;
        if (block.retransmitted) totalRetransmitted++;
    }



    return stats;
}
