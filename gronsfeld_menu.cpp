#include "gronsfeld_menu.h"
#include "gronsfeld.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <iomanip>

using namespace std;
 
enum class CiphAction { 
    ENCRYPT = 1, 
    DECRYPT = 2, 
    BACK = 3 
};

enum class InCiph { 
    CONSOLE_INPUT = 1, 
    FILE_INPUT = 2, 
    BACK = 3 
};

void gronsfeldCiph() {
    bool repeat = true;
    while (repeat) {
        try {
            system("clear");
            cout << "====== GRONSFELD CIPHER ======" << endl;
            cout << "1. Зашифровать данные" << endl;
            cout << "2. Расшифровать данные" << endl;
            cout << "3. Вернуться в главное меню" << endl;
            cout << "Выберите действие: ";
            
            int action;
            if (!(cin >> action) || action < 1 || action > 3) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Неверный выбор действия. Введите число от 1 до 3.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            CiphAction ciphAction = static_cast<CiphAction>(action);
            
            switch (ciphAction) {
                case CiphAction::ENCRYPT: {
                    system("clear");
                    cout << "1. Ввести текст с консоли" << endl;
                    cout << "2. Зашифровать файл" << endl;
                    cout << "3. Назад" << endl;
                    cout << "Выберите источник данных: ";
                    
                    int choice;
                    if (!(cin >> choice) || choice < 1 || choice > 3) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw runtime_error("Неверный выбор. Введите число от 1 до 3.");
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    InCiph inChoice = static_cast<InCiph>(choice);
                    
                    switch (inChoice) {
                        case InCiph::CONSOLE_INPUT: {
                            string text, key;
                            cout << "Введите текст для шифрования: ";
                            getline(cin, text);
                            cout << "Введите ключ (цифры): ";
                            getline(cin, key);
                            
                            string ciphtext = gronsfeldEncryptText(text, key, true);
                            cout << "Результат шифрования: ";
                            for (auto c : ciphtext) {
                                if (isprint(c) && c != ' ') cout << c;
                                else if (c == ' ') cout << " ";
                                else cout << "\\x" << hex << setw(2) << setfill('0') << (int)c << dec;
                            }
                            cout << endl;
                            break;
                        }
                        case InCiph::FILE_INPUT: {
                            string inputFile, outputFile, key;
                            cout << "Введите имя файла для шифрования: ";
                            getline(cin, inputFile);
                            cout << "Введите имя файла для сохранения: ";
                            getline(cin, outputFile);
                            cout << "Введите ключ (цифры): ";
                            getline(cin, key);
                            
                            gronsfeldEncryptFile(inputFile, outputFile, key);
                            cout << "Файл успешно зашифрован!" << endl;
                            break;
                        }
                        case InCiph::BACK:
                            continue;
                    }
                    
                    cout << "Нажмите Enter для продолжения...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    repeat = false;
                    break;
                }
                case CiphAction::DECRYPT: {
                    system("clear");
                    cout << "1. Ввести текст с консоли" << endl;
                    cout << "2. Расшифровать файл" << endl;
                    cout << "3. Назад" << endl;
                    cout << "Выберите источник данных: ";
                    
                    int source;
                    if (!(cin >> source) || source < 1 || source > 3) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw runtime_error("Неверный выбор. Введите число от 1 до 3.");
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    InCiph decryptSource = static_cast<InCiph>(source);
                    
                    switch (decryptSource) {
                        case InCiph::CONSOLE_INPUT: {
                            string ciphtext, key;
                            cout << "Введите зашифрованный текст: ";
                            getline(cin, ciphtext);
                            cout << "Введите ключ (цифры): ";
                            getline(cin, key);
                            
                            string result = gronsfeldDecryptText(ciphtext, key, true);
                            cout << "Результат расшифровки: " << result << endl;
                            break;
                        }
                        case InCiph::FILE_INPUT: {
                            string inputFile, outputFile, key;
                            cout << "Введите имя зашифрованного файла: ";
                            getline(cin, inputFile);
                            cout << "Введите имя файла для сохранения: ";
                            getline(cin, outputFile);
                            cout << "Введите ключ (цифры): ";
                            getline(cin, key);
                            
                            gronsfeldDecryptFile(inputFile, key, outputFile);
                            cout << "Файл успешно расшифрован!" << endl;
                            break;
                        }
                        case InCiph::BACK:
                            break;
                    }
                    
                    if (decryptSource != InCiph::BACK) {
                        cout << "Нажмите Enter для продолжения...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    repeat = false;
                    break;
                }
                case CiphAction::BACK:
                    repeat = false;
                    break;
            }
        }
        catch (const exception& e) {
            system("clear");
            cerr << e.what() << endl;
            cout << "Нажмите Enter для продолжения...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}
