#ifndef HAMMING_H
#define HAMMING_H

#include <vector>

// Function declarations
int calculateParityBits(int dataBits);
std::vector<int> generateHammingCode(const std::vector<int>& data);
void injectError(std::vector<int>& hammingCode, int position);
void detectAndCorrectError(std::vector<int>& hammingCode);
void displayCode(const std::vector<int>& code);
void analyzeResults(const std::vector<int>& original, const std::vector<int>& corrected);

#endif // HAMMING_H
