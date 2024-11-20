#include "hammingx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include <cstdlib>
#include <ctime>

// Configuración de la simulación
const int DATA_SIZE = 16;         // Tamaño de datos por bloque
const int NUM_BLOCKS = 10;        // Número total de bloques
const double ERROR_PROB = 0.3;    // Probabilidad de error por bloque
const std::string OUTPUT_FILE = "simulation_results.txt";

void saveBlocksToFile(const std::vector<Block>& blocks, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de salida.\n";
        return;
    }

    file << "Simulación de Codificación Hamming\n";
    file << "===================================\n";
    for (size_t i = 0; i < blocks.size(); ++i) {
        file << "\n--- Block " << i + 1 << " ---\n";
        file << "Original Data: ";
        for (int bit : blocks[i].data) file << bit;
        file << "\nCompressed Data: ";
        for (int bit : blocks[i].compressed) file << bit;
        file << "\nHamming Code:    ";
        for (int bit : blocks[i].hamming) file << bit;
        file << "\nReceived Data:   ";
        for (int bit : blocks[i].received) file << bit;
        file << "\nChecksum:        ";
        for (int bit : blocks[i].checksum) file << bit;
        if (blocks[i].hasError) {
            file << "\nError Detected at Position: " << blocks[i].errorPosition;
        } else {
            file << "\nNo Errors Detected.";
        }
        file << "\n";
    }
    file.close();
    std::cout << "Resultados guardados en: " << filename << "\n";
}

int main() {
    // Inicializar el generador de números aleatorios
    std::srand(std::time(0));

    // Crear bloques de datos
    std::vector<Block> blocks;
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        blocks.push_back(createBlock(DATA_SIZE));
    }

    // Introducir errores en los bloques
    for (auto& block : blocks) {
        introduceError(block, ERROR_PROB);
    }

    // Detectar y corregir errores en cada bloque
    for (auto& block : blocks) {
        if (block.hasError) {
            block.errorPosition = detectAndCorrect(block.received);
            if (block.errorPosition != 0) {
                block.retransmitted = true; // Marcar el bloque como retransmitido
            }
        }
    }

    // Mostrar información detallada de cada bloque
    for (size_t i = 0; i < blocks.size(); ++i) {
        visualizeBlock(blocks[i], static_cast<int>(i + 1));
    }

    // Guardar los resultados en un archivo
    saveBlocksToFile(blocks, OUTPUT_FILE);

    // Calcular estadísticas
    std::map<std::string, int> stats = calculateStatistics(blocks);
    std::cout << "\n--- Estadísticas de la Simulación ---\n";
    for (const auto& stat : stats) {
        std::cout << std::setw(20) << std::left << stat.first << ": " << stat.second << "\n";
    }

    return 0;
}
