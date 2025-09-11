#include "belazo.h"
#include "file.h"
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <locale>
#include <codecvt>
#include <sstream>
 
using namespace std;

// Функция для преобразования широких символов в UTF-8
string wideStringToUTF8(const wstring& wideStr) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wideStr);
}

// Функция для преобразования UTF-8 в широкие символы
wstring utf8ToWideString(const string& utf8Str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(utf8Str);
}

string belazoEncryptText(const string& text, const string& key, bool verbose) {
    if (key.empty()) throw runtime_error("Ключ не может быть пустым");
    
    if (verbose) {
        cout << "=== ПРОЦЕСС ШИФРОВАНИЯ БЕЛАЗО ===" << endl;
        cout << "Исходный текст: " << text << endl;
        cout << "Ключ: " << key << endl;
        cout << "Длина ключа: " << key.size() << endl;
        cout << "----------------------------------" << endl;
    }
    
    // Преобразуем текст и ключ в широкие символы
    wstring wideText = utf8ToWideString(text);
    wstring wideKey = utf8ToWideString(key);
    wstring resultWide;
    int keyIndex = 0;
    
    for (wchar_t c : wideText) {
        wchar_t keyChar = wideKey[keyIndex % wideKey.size()];
        wchar_t encrypted = (c + keyChar) % 65536;
        resultWide += encrypted;
        
        if (verbose) {
            wcout << L"Символ: '" << c << L"' (код: " << (int)c << L")";
            wcout << L" + Ключ: '" << keyChar << L"' (код: " << (int)keyChar << L")";
            wcout << L" = '" << encrypted << L"' (код: " << (int)encrypted << L")";
            wcout << L" [ключ из позиции " << keyIndex % wideKey.size() << L"]" << endl;
        }
        
        keyIndex++;
    }
    
    string result = wideStringToUTF8(resultWide);
    
    if (verbose) {
        cout << "----------------------------------" << endl;
        cout << "Зашифрованный текст: " << result << endl;
        cout << "==================================" << endl;
    }
    
    return result;
}

string belazoDecryptText(const string& ciphtext, const string& key, bool verbose) {
    if (key.empty()) throw runtime_error("Ключ не может быть пустым");
    
    if (verbose) {
        cout << "=== ПРОЦЕСС ДЕШИФРОВАНИЯ БЕЛАЗО ===" << endl;
        cout << "Зашифрованный текст: " << ciphtext << endl;
        cout << "Ключ: " << key << endl;
        cout << "Длина ключа: " << key.size() << endl;
        cout << "----------------------------------" << endl;
    }
    
    // Преобразуем зашифрованный текст и ключ в широкие символы
    wstring wideCipher = utf8ToWideString(ciphtext);
    wstring wideKey = utf8ToWideString(key);
    wstring resultWide;
    int keyIndex = 0;
    
    for (wchar_t c : wideCipher) {
        wchar_t keyChar = wideKey[keyIndex % wideKey.size()];
        wchar_t decrypted = (c - keyChar + 65536) % 65536;
        resultWide += decrypted;
        
        if (verbose) {
            wcout << L"Символ: '" << c << L"' (код: " << (int)c << L")";
            wcout << L" - Ключ: '" << keyChar << L"' (код: " << (int)keyChar << L")";
            wcout << L" = '" << decrypted << L"' (код: " << (int)decrypted << L")";
            wcout << L" [ключ из позиции " << keyIndex % wideKey.size() << L"]" << endl;
        }
        
        keyIndex++;
    }
    
    string result = wideStringToUTF8(resultWide);
    
    if (verbose) {
        cout << "----------------------------------" << endl;
        cout << "Расшифрованный текст: " << result << endl;
        cout << "==================================" << endl;
    }
    
    return result;
}

void belazoEncryptFile(const string& inputFile, const string& outputFile, const string& key) {
    try {
        auto textData = readFile(inputFile);
        string text(textData.begin(), textData.end());
        string encrypted = belazoEncryptText(text, key, false);
        vector<unsigned char> encryptedData(encrypted.begin(), encrypted.end());
        writeFile(outputFile, encryptedData);
    } catch (const exception& e) {
        throw runtime_error("Ошибка при шифровании файла: " + string(e.what()));
    }
}

void belazoDecryptFile(const string& inputFile, const string& key, const string& outputFile) {
    try {
        auto cipherData = readFile(inputFile);
        string cipherText(cipherData.begin(), cipherData.end());
        string decrypted = belazoDecryptText(cipherText, key, false);
        vector<unsigned char> decryptedData(decrypted.begin(), decrypted.end());
        writeFile(outputFile, decryptedData);
    } catch (const exception& e) {
        throw runtime_error("Ошибка при расшифровании файла: " + string(e.what()));
    }
}
