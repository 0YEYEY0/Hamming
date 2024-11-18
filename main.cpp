#include "hammingx.h"
#include <iostream>
#include <vector>

int main() {
    int dataSize;
    std::cout << "Ingrese numero de bits: ";
    std::cin >> dataSize;

    std::vector<int> data(dataSize);
    std::cout << "Ingrese la informacion de los bits: ";
    for (int i = 0; i < dataSize; i++) {
        std::cin >> data[i];
    }

    std::vector<int> hammingCode = generateHammingCode(data);
    std::cout << "Generando Hamming Code: ";
    displayCode(hammingCode);

    // Simulate error injection
    int errorPosition;
    std::cout << "Ingrese la posicion del error (1-based, 0 para no error): ";
    std::cin >> errorPosition;

    if (errorPosition > 0) {
        injectError(hammingCode, errorPosition);
        std::cout << "Hamming Code con error: ";
        displayCode(hammingCode);
    }

    // Detect and correct the error
    std::vector<int> originalHammingCode = hammingCode; // Store original for comparison
    detectAndCorrectError(hammingCode);

    // Analyze and display results
    analyzeResults(originalHammingCode, hammingCode);

    return 0;
}
