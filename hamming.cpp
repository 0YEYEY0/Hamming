#include "hammingx.h"
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <cmath>
#include <iomanip>

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

    for (int i = 0; i < block.hamming.size(); ++i) {
        if (dis(gen) < errorProb) {
            block.hamming[i] ^= 1; // Cambiar un bit al azar
            block.hasError = true;
            std::cout << "Error en la posicion: " << i + 1 << "\n";
        }
    }
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

// Visualizar un bloque de datos con comparaciones claras
void visualizeBlock(const Block& block, int blockNumber) {
    std::cout << "===== BLOQUE " << blockNumber << " =====\n";
    std::cout << "Datos Originales:     ";
    for (int bit : block.data) {
        std::cout << bit;
    }
    std::cout << "\nCodigo Hamming:       ";
    for (int bit : block.hamming) {
        std::cout << bit;
    }
    std::cout << "\nDatos con Errores:    ";
    for (int bit : block.received) {
        std::cout << bit;
    }
    std::cout << "\nCodigo Corregido:     ";
    for (int bit : block.hamming) {
        std::cout << bit;
    }
    std::cout << "\nChecksum:             ";
    for (int bit : block.checksum) {
        std::cout << bit;
    }
    std::cout << "\n========================\n\n";
}

// Calcular la tasa de corrección y probabilidad de error residual
std::map<std::string, double> analyzePerformance(const std::vector<Block>& blocks) {
    int totalErrors = 0;
    int correctedErrors = 0;
    int residualErrors = 0;

    for (const auto& block : blocks) {
        if (block.hasError) {
            totalErrors++;
            if (block.errorPosition != 0) {
                correctedErrors++;
            } else {
                residualErrors++;
            }
        }
    }

    double correctionRate = (totalErrors > 0) ? (correctedErrors * 100.0 / totalErrors) : 100.0;
    double residualErrorRate = (blocks.size() > 0) ? (residualErrors * 100.0 / blocks.size()) : 0.0;

    return {
            {"Total Errors Detected", totalErrors},
            {"Correction Rate (%)", correctionRate},
            {"Residual Error Rate (%)", residualErrorRate}
    };
}

// Crear y procesar un bloque individual
void processBlockWithVisualization(Block& block, int blockNumber, double errorProb) {
    block.hamming = encodeHamming(block.data);
    block.received = block.hamming;
    introduceError(block, errorProb);
    block.errorPosition = detectAndCorrect(block.hamming);
    block.checksum = generateChecksum(block.hamming);
    visualizeBlock(block, blockNumber);
}

// Realizar pruebas con diferentes tamaños y tasas de error
void runTests() {
    std::vector<int> dataSizes = {16, 32, 64};  // Tamaños de datos en bits
    std::vector<double> errorRates = {0.1, 0.3, 0.5}; // Tasas de error

    for (int size : dataSizes) {
        for (double rate : errorRates) {
            std::cout << "== Prueba: Tamano de Datos = " << size
                      << ", Tasa de Error = " << (rate * 100) << "% ==\n";

            std::vector<Block> testBlocks;
            int numBlocks = 5;

            for (int i = 0; i < numBlocks; ++i) {
                Block block = createBlock(size);
                processBlockWithVisualization(block, i + 1, rate);
                testBlocks.push_back(block);
            }

            // Asegurar que el encabezado se imprime independientemente de los bloques
            if (testBlocks.empty()) {
                std::cout << "No se generaron bloques para esta configuracion.\n";
            }

            // Analizar el rendimiento
            std::map<std::string, double> performance = analyzePerformance(testBlocks);
            std::cout << "=== Resultados de la Prueba ===\n";
            for (const auto& stat : performance) {
                std::cout << stat.first << ": " << std::fixed << std::setprecision(2)
                          << stat.second << "\n";
            }
            std::cout << "==============================\n\n";
        }
    }
}

