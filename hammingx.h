#ifndef HAMMING_H
#define HAMMING_H

#include <vector>

// Function declarations
int calculateParityBits(int dataBits);
std::vector<int> generateHammingCode(const std::vector<int>& data);
void injectRandomErrors(std::vector<std::vector<int>>& dataSet, int numErrors);
int detectAndCorrectError(std::vector<int>& hammingCode);
void displayCode(const std::vector<int>& code);

#endif // HAMMING_H

