#include "gronsfeld.h"
#include "file.h"
#include <stdexcept>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <locale>
#include <codecvt>
#include <vector>

using namespace std;

// Функция для преобразования строки в вектор байтов UTF-8
vector<unsigned char> stringToBytes(const string& str) {
    return vector<unsigned char>(str.begin(), str.end());
}

// Функция для преобразования вектора байтов в строку UTF-8
string bytesToString(const vector<unsigned char>& bytes) {
    return string(bytes.begin(), bytes.end());
}

string gronsfeldEncryptText(const string& text, const string& key, bool verbose) {
    if (key.empty()) throw runtime_error("Ключ не может быть пустым");
    
    // Проверка что ключ состоит только из цифр
    for (char c : key) {
        if (!isdigit(c)) {
            throw runtime_error("Ключ должен содержать только цифры");
        }
    }
    
    if (verbose) {
        cout << "=== ПРОЦЕСС ШИФРОВАНИЯ ГРОНСФЕЛЬДА ===" << endl;
        cout << "Исходный текст: " << text << endl;
        cout << "Ключ: " << key << endl;
        cout << "Длина ключа: " << key.size() << endl;
        cout << "----------------------------------------" << endl;
    }
    
    // Преобразуем текст в байты UTF-8
    vector<unsigned char> textBytes = stringToBytes(text);
    vector<unsigned char> resultBytes;
    int keyIndex = 0;
    
    for (size_t i = 0; i < textBytes.size(); i++) {
        unsigned char c = textBytes[i];
        int shift = key[keyIndex % key.size()] - '0';
        unsigned char encrypted = (c + shift) % 256;
        resultBytes.push_back(encrypted);
        
        if (verbose) {
            cout << "Байт " << i << ": ";
            cout << "Исходный: ";
            if (isprint(c) && c != ' ') cout << c;
            else if (c == ' ') cout << " ";
            else cout << "\\x" << hex << setw(2) << setfill('0') << (int)c << dec;
            cout << " (код: " << (int)c << ")";
            cout << " + Сдвиг: " << shift;
            cout << " = Зашифрованный: ";
            if (isprint(encrypted) && encrypted != ' ') cout << encrypted;
            else if (encrypted == ' ') cout << " ";
            else cout << "\\x" << hex << setw(2) << setfill('0') << (int)encrypted << dec;
            cout << " (код: " << (int)encrypted << ")";
            cout << " [ключ из позиции " << keyIndex % key.size() << "]" << endl;
        }
        
        keyIndex++;
    }
    
    string result = bytesToString(resultBytes);
    
    if (verbose) {
        cout << "----------------------------------------" << endl;
        cout << "Зашифрованный текст: " << result << endl;
        cout << "========================================" << endl;
    }
    
    return result;
}

string gronsfeldDecryptText(const string& ciphtext, const string& key, bool verbose) {
    if (key.empty()) throw runtime_error("Ключ не может быть пустым");
    
    // Проверка что ключ состоит только из цифр
    for (char c : key) {
        if (!isdigit(c)) {
            throw runtime_error("Ключ должен содержать только цифры");
        }
    }
    
    if (verbose) {
        cout << "=== ПРОЦЕСС ДЕШИФРОВАНИЯ ГРОНСФЕЛЬДА ===" << endl;
        cout << "Зашифрованный текст: " << ciphtext << endl;
        cout << "Ключ: " << key << endl;
        cout << "Длина ключа: " << key.size() << endl;
        cout << "----------------------------------------" << endl;
    }
    
    // Преобразуем зашифрованный текст в байты UTF-8
    vector<unsigned char> cipherBytes = stringToBytes(ciphtext);
    vector<unsigned char> resultBytes;
    int keyIndex = 0;
    
    for (size_t i = 0; i < cipherBytes.size(); i++) {
        unsigned char c = cipherBytes[i];
        int shift = key[keyIndex % key.size()] - '0';
        unsigned char decrypted = (c - shift + 256) % 256;
        resultBytes.push_back(decrypted);
        
        if (verbose) {
            cout << "Байт " << i << ": ";
            cout << "Зашифрованный: ";
            if (isprint(c) && c != ' ') cout << c;
            else if (c == ' ') cout << " ";
            else cout << "\\x" << hex << setw(2) << setfill('0') << (int)c << dec;
            cout << " (код: " << (int)c << ")";
            cout << " - Сдвиг: " << shift;
            cout << " = Расшифрованный: ";
            if (isprint(decrypted) && decrypted != ' ') cout << decrypted;
            else if (decrypted == ' ') cout << " ";
            else cout << "\\x" << hex << setw(2) << setfill('0') << (int)decrypted << dec;
            cout << " (код: " << (int)decrypted << ")";
            cout << " [ключ из позиции " << keyIndex % key.size() << "]" << endl;
        }
        
        keyIndex++;
    }
    
    string result = bytesToString(resultBytes);
    
    if (verbose) {
        cout << "----------------------------------------" << endl;
        cout << "Расшифрованный текст: " << result << endl;
        cout << "========================================" << endl;
    }
    
    return result;
}

void gronsfeldEncryptFile(const string& inputFile, const string& outputFile, const string& key) {
    try {
        auto textData = readFile(inputFile);
        string text(textData.begin(), textData.end());
        string encrypted = gronsfeldEncryptText(text, key, false);
        vector<unsigned char> encryptedData(encrypted.begin(), encrypted.end());
        writeFile(outputFile, encryptedData);
    } catch (const exception& e) {
        throw runtime_error("Ошибка при шифровании файла: " + string(e.what()));
    }
}

void gronsfeldDecryptFile(const string& inputFile, const string& key, const string& outputFile) {
    try {
        auto cipherData = readFile(inputFile);
        string cipherText(cipherData.begin(), cipherData.end());
        string decrypted = gronsfeldDecryptText(cipherText, key, false);
        vector<unsigned char> decryptedData(decrypted.begin(), decrypted.end());
        writeFile(outputFile, decryptedData);
    } catch (const exception& e) {
        throw runtime_error("Ошибка при расшифровании файла: " + string(e.what()));
    }
}
