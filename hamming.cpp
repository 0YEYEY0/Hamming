#include "hammingx.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Function to calculate the number of parity bits needed
int calculateParityBits(int dataBits) {
    int r = 0;
    while ((1 << r) < (dataBits + r + 1)) {
        r++;
    }
    return r;
}

// Function to generate the Hamming code
std::vector<int> generateHammingCode(const std::vector<int>& data) {
    int m = data.size();
    int r = calculateParityBits(m);
    int n = m + r;

    std::vector<int> hammingCode(n, 0);

    // Insert data bits into the hamming code
    int j = 0;
    for (int i = 0; i < n; i++) {
        if ((i + 1) && ((i + 1) & i) == 0) { // Check if it's a parity bit position
            continue;
        }
        hammingCode[i] = data[j++];
    }

    // Calculate parity bits
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

// Function to inject random errors into some data sets
void injectRandomErrors(std::vector<std::vector<int>>& dataSet, int numErrors) {
    std::srand(std::time(0)); // Seed for randomness
    for (int i = 0; i < numErrors; i++) {
        int setIndex = std::rand() % dataSet.size();
        int bitIndex = std::rand() % dataSet[setIndex].size();
        dataSet[setIndex][bitIndex] ^= 1; // Flip the bit to create an error
    }
}

// Function to detect and correct errors in the Hamming code
int detectAndCorrectError(std::vector<int>& hammingCode) {
    int n = hammingCode.size();
    int errorPosition = 0;

    // Calcula el síndrome
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

    // Corrige el error si se detecta
    if (errorPosition != 0) {
        std::cout << "Error detected at position: " << errorPosition << std::endl;
        hammingCode[errorPosition - 1] ^= 1;
        std::cout << "Error corrected.\n";
    } else {
        std::cout << "No error detected.\n";
    }

    return errorPosition; // Devuelve la posición del error (o 0 si no hay error)
}


// Function to display the code
void displayCode(const std::vector<int>& code) {
    for (int bit : code) {
        std::cout << bit << " ";
    }
    std::cout << std::endl;
}
