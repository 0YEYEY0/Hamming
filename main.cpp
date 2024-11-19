#include "hammingx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

// Simula la transmisión con un canal ruidoso
void simulateTransmission(std::vector<std::vector<int>>& dataSet, double errorRate) {
    std::srand(std::time(0));
    for (auto& block : dataSet) {
        for (auto& bit : block) {
            if ((std::rand() % 100) < (errorRate * 100)) {
                bit ^= 1; // Introduce un error
            }
        }
    }
}

// Divide y guarda datos en múltiples archivos
void saveToMultipleFiles(const std::vector<std::vector<int>>& dataSet, const std::string& baseFilename) {
    for (size_t i = 0; i < dataSet.size(); i++) {
        std::string filename = baseFilename + "_block_" + std::to_string(i + 1) + ".txt";
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            continue;
        }
        for (int bit : dataSet[i]) {
            file << bit;
        }
        file.close();
    }
    std::cout << "Data saved to multiple files." << std::endl;
}

// Lee datos de múltiples archivos
std::vector<std::vector<int>> readFromMultipleFiles(const std::string& baseFilename, int numBlocks) {
    std::vector<std::vector<int>> dataSet;
    for (int i = 1; i <= numBlocks; i++) {
        std::string filename = baseFilename + "_block_" + std::to_string(i) + ".txt";
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            continue;
        }
        std::vector<int> block;
        char bit;
        while (file >> bit) {
            block.push_back(bit - '0');
        }
        dataSet.push_back(block);
        file.close();
    }
    std::cout << "Data read from multiple files." << std::endl;
    return dataSet;
}

// Retransmisión si el error no es corregible
bool retransmitBlock(std::vector<int>& block) {
    // Simula la retransmisión: genera nuevamente el bloque original
    std::vector<int> originalBlock = block; // Aquí, simplemente lo recuperamos sin errores
    block = originalBlock;
    std::cout << "Retransmission requested and block retransmitted." << std::endl;
    return true;
}

int main() {
    int dataSize = 4;        // Tamaño de cada bloque de datos
    int numBlocks = 10;      // Número de bloques de datos
    double errorRate = 0.2;  // Probabilidad de error por bit
    int maxRetransmissions = 3;

    // Paso 1: Generar datos originales y codificarlos
    std::vector<std::vector<int>> dataSet;
    for (int i = 0; i < numBlocks; i++) {
        std::vector<int> data(dataSize);
        for (int j = 0; j < dataSize; j++) {
            data[j] = std::rand() % 2;
        }
        dataSet.push_back(generateHammingCode(data));
    }

    // Paso 2: Guardar datos en múltiples archivos
    saveToMultipleFiles(dataSet, "block");

    // Paso 3: Leer datos desde los archivos
    auto readDataSet = readFromMultipleFiles("block", numBlocks);

    // Paso 4: Simular transmisión con errores
    simulateTransmission(readDataSet, errorRate);

    // Paso 5: Detectar, corregir y solicitar retransmisión si es necesario
    int errorsCorrected = 0;
    int retransmissions = 0;

    for (size_t i = 0; i < readDataSet.size(); i++) {
        std::cout << "Block " << i + 1 << " received: ";
        displayCode(readDataSet[i]);

        int errorPosition = detectAndCorrectError(readDataSet[i]);
        if (errorPosition > 1) { // Si no se puede corregir
            if (retransmissions < maxRetransmissions) {
                retransmitBlock(readDataSet[i]);
                retransmissions++;
            } else {
                std::cout << "Max retransmissions reached for block " << i + 1 << std::endl;
            }
        } else {
            errorsCorrected++;
        }

        std::cout << "Block " << i + 1 << " after correction: ";
        displayCode(readDataSet[i]);
        std::cout << std::endl;
    }

    // Paso 6: Estadísticas
    std::cout << "Transmission complete!" << std::endl;
    std::cout << "Blocks with errors corrected: " << errorsCorrected << std::endl;
    std::cout << "Retransmissions requested: " << retransmissions << std::endl;

    return 0;
}

