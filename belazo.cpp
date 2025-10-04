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

// Функция для преобразования числа в шестнадцатеричную строку
inline string toHex(int number, int width = 2) {
    stringstream ss;
    ss << uppercase << hex << setw(width) << setfill('0') << number;
    return ss.str();
}

// Функция для преобразования шестнадцатеричной строки в число
inline int fromHex(const string& hexStr) {
    int value;
    stringstream ss;
    ss << hex << hexStr;
    if (!(ss >> value)) {
        throw runtime_error("Неверный hex формат: " + hexStr);
    }
    return value;
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
    
    string result;
    
    for (size_t i = 0; i < text.size(); i++) {
        unsigned char c = text[i];
        unsigned char keyChar = key[i % key.size()];
        int encrypted = (c + keyChar) % 256;
        
        // Преобразуем в HEX для читаемости
        string hexCode = toHex(encrypted);
        result += hexCode;
        
        if (verbose) {
            cout << "Позиция " << i << ": ";
            cout << "Символ: '";
            if (isprint(c) && c != ' ') cout << c;
            else if (c == ' ') cout << " ";
            else cout << "\\x" << hex << setw(2) << setfill('0') << (int)c << dec;
            cout << "' (код: " << (int)c << ")";
            cout << " + Ключ: '" << keyChar << "' (код: " << (int)keyChar << ")";
            cout << " = HEX: " << hexCode << " (код: " << encrypted << ")";
            cout << " [ключ из позиции " << i % key.size() << "]" << endl;
        }
        
        // Добавляем пробел между кодами, но не после последнего символа
        if (i < text.size() - 1) {
            result += " ";
        }
    }
    
    if (verbose) {
        cout << "----------------------------------" << endl;
        cout << "Результат шифрования: " << result << endl;
        cout << "==================================" << endl;
    }
    
    return result;
}

string belazoDecryptText(const string& ciphtext, const string& key, bool verbose) {
    if (key.empty()) throw runtime_error("Ключ не может быть пустым");
    
    if (verbose) {
        cout << "=== ПРОЦЕСС ДЕШИФРОВАНИЯ БЕЛАЗО ===" << endl;
        cout << "Результат шифрования: " << ciphtext << endl;
        cout << "Ключ: " << key << endl;
        cout << "Длина ключа: " << key.size() << endl;
        cout << "----------------------------------" << endl;
    }
    
    string result;
    size_t i = 0;
    int charIndex = 0;
    
    while (i < ciphtext.length()) {
        // Пропускаем пробелы
        while (i < ciphtext.length() && isspace(ciphtext[i])) {
            i++;
        }
        
        // Извлекаем две шестнадцатеричные цифры
        if (i + 1 < ciphtext.length() && isxdigit(ciphtext[i]) && isxdigit(ciphtext[i + 1])) {
            string hexStr = ciphtext.substr(i, 2);
            try {
                int code = fromHex(hexStr);
                unsigned char keyChar = key[charIndex % key.size()];
                unsigned char decrypted = (code - keyChar + 256) % 256;
                result += decrypted;
                
                if (verbose) {
                    cout << "HEX: " << hexStr << " (код: " << code << ")";
                    cout << " - Ключ: '" << keyChar << "' (код: " << (int)keyChar << ")";
                    cout << " = Символ: '";
                    if (isprint(decrypted) && decrypted != ' ') cout << decrypted;
                    else if (decrypted == ' ') cout << " ";
                    else cout << "\\x" << hex << setw(2) << setfill('0') << (int)decrypted << dec;
                    cout << "' (код: " << (int)decrypted << ")";
                    cout << " [ключ из позиции " << charIndex % key.size() << "]" << endl;
                }
                
                charIndex++;
            } catch (const exception& e) {
                throw runtime_error("Ошибка при обработке hex-последовательности: " + hexStr);
            }
            
            i += 2;
        } else if (i < ciphtext.length() && !isspace(ciphtext[i])) {
            // Если осталась неполная hex-последовательность
            throw runtime_error("Неполная hex-последовательность в позиции: " + to_string(i));
        }
    }
    
    if (verbose) {
        cout << "----------------------------------" << endl;
        cout << "Результат расшифровки: " << result << endl;
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
