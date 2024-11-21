#include "hammingx.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// Simula el proceso de recibir bloques de datos, verificando errores y estadísticas
void processBlock(Block& block, int blockNumber) {
    // Generar código Hamming para los datos
    block.hamming = encodeHamming(block.data);

    // Visualiza el bloque antes de procesarlo
    std::cout << "Datos Originales del Bloque " << blockNumber << ": ";
    for (int bit : block.data) {
        std::cout << bit;
    }
    std::cout << "\nCodigo Hamming Original: ";
    for (int bit : block.hamming) {
        std::cout << bit;
    }
    std::cout << "\n";

    // Introducir errores aleatorios con una probabilidad
    introduceError(block, 0.3); // Probabilidad de 30% de error por bit

    // Visualizar el bloque con errores introducidos
    std::cout << "Bloque con Errores Introducidos: ";
    for (int bit : block.hamming) {
        std::cout << bit;
    }
    std::cout << "\n";

    // Detectar y corregir errores en el código Hamming
    std::vector<int> originalHamming = block.hamming; // Guardamos la versión original para comparaciones
    block.errorPosition = detectAndCorrect(block.hamming);

    if (block.errorPosition != 0) {
        block.hasError = true;
        std::cout << "Errores detectados y corregidos en las siguientes posiciones: ";
        for (int i = 0; i < block.hamming.size(); ++i) {
            if (originalHamming[i] != block.hamming[i]) {
                std::cout << i + 1 << " "; // Imprimir las posiciones corregidas
            }
        }
        std::cout << "\n";
    } else {
        std::cout << "No se detectaron errores.\n";
    }

    // Visualizar el bloque después de la corrección
    std::cout << "Bloque despues de correccion: ";
    for (int bit : block.hamming) {
        std::cout << bit;
    }
    std::cout << "\n";

    // Generar un checksum para el bloque
    block.checksum = generateChecksum(block.hamming);

    // Mostrar las estadísticas del bloque
    std::map<std::string, int> stats = calculateStatistics({block});
    std::cout << "Estadisticas del Bloque " << blockNumber << ":\n";
    for (const auto& stat : stats) {
        std::cout << stat.first << ": " << stat.second << "\n";
    }
    std::cout << "\n";
}

int main() {
    std::srand(std::time(0)); // Semilla para la generación de números aleatorios

    int numBlocks = 5; // Número de bloques a procesar

    // Crear y procesar bloques de datos
    for (int i = 1; i <= numBlocks; ++i) {
        Block block = createBlock(64); // Crear bloque de datos con tamaño de 64 bits
        processBlock(block, i);        // Procesar el bloque
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Espera para simular el tiempo de procesamiento
    }

    return 0;
}
