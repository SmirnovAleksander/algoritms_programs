 
 
#include <iostream>
#include <string>

int main() {
    setlocale(LC_ALL, "ru");
    std::string line;
    bool instring = false;
    std::cout << "введите строку: ";
    std::getline(std::cin, line);
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '\'') {
            if (i + 1 < line.size() && line[i + 1] == '\'') {
                std::cout << "найден удвоенный апостроф, пропуск...\n";
                i++;  
            }
            else {
                instring = !instring;
                if (instring) {
                    std::cout << "начало строки в апострофах.\n";
                }
                else {
                    std::cout << "конец строки в апострофах.\n";
                }
            }
        }
    }
    if (instring) {
        std::cout << "ошибка: строка не закрыта.\n";
    }
    else {
        std::cout << "строка корректна.\n";
    }

    return 0;
}