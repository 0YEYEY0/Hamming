#include "hammingx.h"
#include <iostream>
#include <cmath>

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

// Function to inject an error at a given position
void injectError(std::vector<int>& hammingCode, int position) {
    if (position > 0 && position <= hammingCode.size()) {
        hammingCode[position - 1] ^= 1; // Flip the bit
        std::cout << "Injected error at position " << position << "." << std::endl;
    }
}

// Function to detect and correct errors in the Hamming code
void detectAndCorrectError(std::vector<int>& hammingCode) {
    int n = hammingCode.size();
    int errorPosition = 0;

    // Calculate the syndrome
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

    // Correct the error if found
    if (errorPosition != 0) {
        std::cout << "Error detected at position: " << errorPosition << std::endl;
        hammingCode[errorPosition - 1] ^= 1;
        std::cout << "Error corrected.\n";
    } else {
        std::cout << "No error detected.\n";
    }
}

// Function to display the code
void displayCode(const std::vector<int>& code) {
    for (int bit : code) {
        std::cout << bit << " ";
    }
    std::cout << std::endl;
}

// Function to analyze results and compare original and corrected code
void analyzeResults(const std::vector<int>& original, const std::vector<int>& corrected) {
    std::cout << "Original code: ";
    displayCode(original);
    std::cout << "Corrected code: ";
    displayCode(corrected);
}

