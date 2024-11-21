#include "hammingx.h"
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(std::time(0)); // Inicializar la semilla aleatoria

    // Procesar bloques individuales
    int numBlocks = 20;
    for (int i = 1; i <= numBlocks; ++i) {
        Block block = createBlock(64);
        processBlockWithVisualization(block, i, 0.3);
    }

    // Ejecutar pruebas automáticas
    runTests();

    return 0;
}
