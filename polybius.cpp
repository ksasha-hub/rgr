#include "polybius.h"
#include "file.h"
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;

// Функция для преобразования числа в шестнадцатеричную строку
string toHex(int number, int width = 2) {
    stringstream ss;
    ss << uppercase << hex << setw(width) << setfill('0') << number;
    return ss.str();
}

// Функция для преобразования шестнадцатеричной строки в число
int fromHex(const string& hexStr) {
    int value;
    stringstream ss;
    ss << hex << hexStr;
    if (!(ss >> value)) {
        throw runtime_error("Неверный hex формат: " + hexStr);
    }
    return value;
}

string polybiusEncryptText(const string& text, bool verbose) {
    if (verbose) {
        cout << "=== ПРОЦЕСС ШИФРОВАНИЯ ПОЛИБИЯ ===" << endl;
        cout << "Исходный текст: " << text << endl;
        cout << "----------------------------------" << endl;
    }
    
    string result;
    
    for (size_t i = 0; i < text.size(); i++) {
        unsigned char c = text[i];
        int code = static_cast<int>(c);
        
        // Преобразуем код символа в шестнадцатеричное представление
        string hexCode = toHex(code);
        result += hexCode;
        
        if (verbose) {
            cout << "Позиция " << i << ": ";
            cout << "Символ: '";
            if (isprint(c) && c != ' ') cout << c;
            else if (c == ' ') cout << " ";
            else cout << "\\x" << hex << setw(2) << setfill('0') << code << dec;
            cout << "' (код: " << code << ") -> HEX: " << hexCode << endl;
        }
        
        // Добавляем пробел между кодами, но не после последнего символа
        if (i < text.size() - 1) {
            result += " ";
        }
    }
    
    if (verbose) {
        cout << "----------------------------------" << endl;
        cout << "Зашифрованный текст: " << result << endl;
        cout << "==================================" << endl;
    }
    
    return result;
}

string polybiusDecryptText(const string& ciphtext, bool verbose) {
    if (verbose) {
        cout << "=== ПРОЦЕСС ДЕШИФРОВАНИЯ ПОЛИБИЯ ===" << endl;
        cout << "Зашифрованный текст: " << ciphtext << endl;
        cout << "----------------------------------" << endl;
    }
    
    string result;
    size_t i = 0;
    
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
                char decrypted = static_cast<char>(code);
                result += decrypted;
                
                if (verbose) {
                    cout << "HEX: " << hexStr << " -> Код: " << code << " -> Символ: '";
                    if (isprint(decrypted) && decrypted != ' ') cout << decrypted;
                    else if (decrypted == ' ') cout << " ";
                    else cout << "\\x" << hex << setw(2) << setfill('0') << code << dec;
                    cout << "'" << endl;
                }
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
        cout << "Расшифрованный текст: " << result << endl;
        cout << "==================================" << endl;
    }
    
    return result;
}

void polybiusEncryptFile(const string& inputFile, const string& outputFile) {
    try {
        auto textData = readFile(inputFile);
        string text(textData.begin(), textData.end());
        string encrypted = polybiusEncryptText(text, false);
        vector<unsigned char> encryptedData(encrypted.begin(), encrypted.end());
        writeFile(outputFile, encryptedData);
    } catch (const exception& e) {
        throw runtime_error("Ошибка при шифровании файла: " + string(e.what()));
    }
}

void polybiusDecryptFile(const string& inputFile, const string& outputFile) {
    try {
        auto cipherData = readFile(inputFile);
        string cipherText(cipherData.begin(), cipherData.end());
        string decrypted = polybiusDecryptText(cipherText, false);
        vector<unsigned char> decryptedData(decrypted.begin(), decrypted.end());
        writeFile(outputFile, decryptedData);
    } catch (const exception& e) {
        throw runtime_error("Ошибка при расшифровании файла: " + string(e.what()));
    }
}
