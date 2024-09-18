 
 
#include <iostream>
#include <string>

int main() {
    setlocale(LC_ALL, "ru");
    std::string line;
    bool instring = false;
    std::cout << "������� ������: ";
    std::getline(std::cin, line);
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '\'') {
            if (i + 1 < line.size() && line[i + 1] == '\'') {
                std::cout << "������ ��������� ��������, �������...\n";
                i++;  
            }
            else {
                instring = !instring;
                if (instring) {
                    std::cout << "������ ������ � ����������.\n";
                }
                else {
                    std::cout << "����� ������ � ����������.\n";
                }
            }
        }
    }
    if (instring) {
        std::cout << "������: ������ �� �������.\n";
    }
    else {
        std::cout << "������ ���������.\n";
    }

    return 0;
}