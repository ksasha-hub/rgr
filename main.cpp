#include <iostream>
#include <limits>
#include <cstdlib>
#include "gronsfeld_menu.h"
#include "belazo_menu.h"
#include "polybius_menu.h"

using namespace std;

enum class CiphType {
    GRONSFELD = 1,
    BELAZO = 2,
    POLYBIUS = 3,
    EXIT = 4
};

int main() {
    while (true) {
        try {
            system("clear");
            cout << "=== ENCRYPTION ALGORITHMS ===" << endl;
            cout << "     1. GRONSFELD CIPHER     " << endl;
            cout << "     2. BELAZO CIPHER        " << endl;
            cout << "     3. POLYBIUS SQUARE      " << endl;
            cout << "     4. EXIT                 " << endl;
            cout << "=============================" << endl;
            cout << "Выберите алгоритм: ";
            
            int input;
            if (!(cin >> input) || input < 1 || input > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Ошибка: Введите число от 1 до 4!");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            CiphType choice = static_cast<CiphType>(input);
            
            switch (choice) {
                case CiphType::GRONSFELD:
                    gronsfeldCiph();
                    break;
                case CiphType::BELAZO:
                    belazoCiph(); 
                    break;
                case CiphType::POLYBIUS:
                    polybiusCiph();
                    break;
                case CiphType::EXIT:
                    cout << "Выход из программы. До свидания!" << endl;
                    return 0;
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
