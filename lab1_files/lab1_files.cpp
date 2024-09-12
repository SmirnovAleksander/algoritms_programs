#include <iostream>
#include <fstream>
#include <string>

int main()
{
    setlocale(LC_ALL, "RU");

    std::string inputFileName, outputFileName;

    std::cout << "������� �������� �������� �����: ";
    std::cin >> inputFileName;
    std::cout << "������� �������� ��������� �����: ";
    std::cin >> outputFileName;

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cout << "������! ���� �� ��� ������." << std::endl;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cout << "������! ���� �� ��� ������." << std::endl;
    }

    std::string str;

    int currentPage = 0;
    int lastPage = 0;
    bool isNewParagraph = true;
    bool isContent = false;

    while (std::getline(inputFile, str)) {
        if (str.empty()) {
            isNewParagraph = true;
            continue;
        }
        if (str.length() > 2 && str[0] == '-' && str[str.length() - 1] == '-') {
            currentPage = atoi(str.c_str() + 1); //�� atoi ��������� �� String � char*, � c_str ���������� ��������� �� ������
            if (lastPage != 0 && currentPage > lastPage + 1) {
                std::cout << "� �������� " << currentPage << " ������� � " << lastPage << " ����� ��� �� ���� ��������" << std::endl;
            }
            lastPage = currentPage;
            continue;
        }
        if (isNewParagraph) {
            outputFile << str << " .......... " << currentPage << std::endl;
            isNewParagraph = false;
            isContent = true;
        }

    }
    if (isContent)
        std::cout << "���������� �������" << std::endl;
    else
        std::cout << "���������� ����� �����. ���������� �� �������" << std::endl;

    inputFile.close();
    outputFile.close();
    return 0;
}