#ifndef BELAZO_H
#define BELAZO_H
#include <vector>
#include <string>
using namespace std;
extern "C" {
    void belazoEncryptFile(const string& inputFile, const string& outputFile, const string& key);
    void belazoDecryptFile(const string& inputFile, const string& key, const string& outputFile);
    string belazoEncryptText(const string& text, const string& key, bool verbose = false);
    string belazoDecryptText(const string& ciphtext, const string& key, bool verbose = false);
}
#endif
