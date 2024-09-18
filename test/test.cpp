 
 
#include <iostream>
#include <string>

int main() {
    setlocale(LC_ALL, "ru");
    std::string line;
    bool instring = false;  // флаг для отслеживания нахождения в строке

    // считывание строки с консоли
    std::cout << "введите строку: ";
    std::getline(std::cin, line);

    // обрабатываем строку посимвольно
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        // проверка на апостроф
        if (c == '\'') {
            // проверяем на удвоенный апостроф
            if (i + 1 < line.size() && line[i + 1] == '\'') {
                std::cout << "найден удвоенный апостроф, пропуск...\n";
                i++;  // пропускаем второй апостроф
            }
            else {
                instring = !instring;  // переключаем флаг только если апостроф не удвоен
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