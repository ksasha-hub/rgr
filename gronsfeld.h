#ifndef GRONSFELD_H
#define GRONSFELD_H
#include <vector>
#include <string>
using namespace std;
extern "C" {
    void gronsfeldEncryptFile(const string& inputFile, const string& outputFile, const string& key);
    void gronsfeldDecryptFile(const string& inputFile, const string& key, const string& outputFile);
    string gronsfeldEncryptText(const string& text, const string& key, bool verbose = false);
    string gronsfeldDecryptText(const string& ciphtext, const string& key, bool verbose = false);
}
#endif
 
