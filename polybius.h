#ifndef POLYBIUS_H
#define POLYBIUS_H
#include <vector>
#include <string>
using namespace std;
extern "C" {
    void polybiusEncryptFile(const string& inputFile, const string& outputFile);
    void polybiusDecryptFile(const string& inputFile, const string& outputFile);
    string polybiusEncryptText(const string& text, bool verbose = false);
    string polybiusDecryptText(const string& ciphtext, bool verbose = false);
} 
#endif
