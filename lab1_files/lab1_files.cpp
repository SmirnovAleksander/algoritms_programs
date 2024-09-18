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
        std::cerr << "������! ���� �� ��� ������." << std::endl;
        return 1;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "������! ���� �� ��� ������." << std::endl;
        return 1;
    }

    std::string str;
    std::string header;
    int currentPage = 0;
    int lastPage = 0;
    bool isNewParagraph = true;
    bool isContent = false;
    bool singleLineOnPage = false;
    bool isFirstPage = true;
    bool firstPageSingleLineError = false;

    while (std::getline(inputFile, str)) {
        if (str.empty()) {
            // ������ ������ ��������� �� ����� ���������
            if (!header.empty() && lastPage > 0) {
                if (isFirstPage && singleLineOnPage) {
                    firstPageSingleLineError = true;
                }
                else {
                    outputFile << header << " .......... " << lastPage << std::endl;
                    isContent = true;
                }
                header.clear();
                isFirstPage = false;
            }
            isNewParagraph = true;
            singleLineOnPage = false;
            continue;
        }

        if (str.length() > 2 && str[0] == '-' && str[str.length() - 1] == '-') {
            // ������� � ��������� ��������
            int pageNumber = 0;
            for (size_t i = 1; i < str.length() - 1; ++i) {
                if (str[i] < '0' || str[i] > '9') {
                    std::cerr << "������! ������������ ����� ��������: " << str << std::endl;
                    return 1;
                }
                pageNumber = pageNumber * 10 + (str[i] - '0');
            }

            if (lastPage != 0 && pageNumber == lastPage) {
                std::cerr << "������! ������������� ��������: " << pageNumber << std::endl;
                return 1;
            }

            if (lastPage != 0 && pageNumber > lastPage + 1) {
                std::cerr << "������! ������ ����� ����������: " << lastPage << " � " << pageNumber << std::endl;
                return 1;
            }

            if (!header.empty() && !singleLineOnPage) {
                outputFile << header << " .......... " << lastPage << std::endl;
                isContent = true;
                header.clear();
            }

            lastPage = pageNumber;
            isNewParagraph = true;
            singleLineOnPage = false;
            continue;
        }

        if (isNewParagraph) {
            // ��������� ��������� ���������
            header = str;
            isNewParagraph = false;
            singleLineOnPage = true; // ������������, ��� ��� ������������ ������ �� ��������
        }
        else {
            // ���� �� �������� ���� ����� � ��� �� ������ ���������
            singleLineOnPage = false;
        }
    }

    // ���������� ��������� ��������� � ����� ��������
    if (!header.empty() && lastPage > 0) {
        if (isFirstPage && singleLineOnPage) {
            firstPageSingleLineError = true;
        }
        else {
            outputFile << header << " .......... " << lastPage << std::endl;
            isContent = true;
        }
    }

    if (firstPageSingleLineError) {
        std::cerr << "������! �� ������ �������� ������ ���� ������." << std::endl;
        outputFile.close();  // ��������� �������� ����, ���� ���� ������
        std::remove(outputFileName.c_str());  // ������� �������� ����, ����� �� ��������� ������ ����
    }
    else if (isContent) {
        std::cout << "���������� �������" << std::endl;
    }
    else {
        std::cout << "���������� ����� �����. ���������� �� �������" << std::endl;
    }

    inputFile.close();
    return 0;
}
