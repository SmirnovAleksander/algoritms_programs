//13. Имеется   текст   документа, состоящий  из  нескольких
//параграфов.Каждый параграф начинается с заголовка и  отделен
//пустой  строкой  от  предыдущего параграфа.Текст разделен на
//страницы.Номер страницы проставлен в ее начале и  выделен  с
//обеих  сторон  знаком  '-'.Сформировать  файл  с оглавлением
//документа(7).
//
//Смирнов Александр ПС-21 
//Компилятор: C++
//Среда выполнеия: Visual studio 2022
#include <iostream>
#include <fstream>
#include <string>

int main() {
    setlocale(LC_ALL, "RU");

    std::string inputFileName, outputFileName;

    std::cout << "Введите название входного файла: ";
    std::cin >> inputFileName;
    std::cout << "Введите название выходного файла: ";
    std::cin >> outputFileName;
    //inputFileName = "input.txt";
    //outputFileName = "output.txt";

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка! Файл не был открыт." << std::endl;
        return 1;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка! Файл не был открыт." << std::endl;
        return 1;
    }

    std::string str, header, lastHeader;
    int lastPage = 0;
    bool isNewParagraph = false;
    bool isFirstParagraph = true;
    int currentPage = 0;

    while (std::getline(inputFile, str)) {
        if (str.empty()) {
            isNewParagraph = true;
            continue;
        }

        if (str.length() > 2 && str[0] == '-' && str[str.length() - 1] == '-') {
            int pageNumber = 0;
            for (int i = 1; i < str.length() - 1; ++i) {
                if (str[i] < '0' || str[i] > '9') {
                    std::cerr << "Ошибка! Некорректный номер страницы: " << str << std::endl;
                    return 1;
                }
                pageNumber = pageNumber * 10 + (str[i] - '0');
            }

            if (pageNumber == lastPage) {
                std::cerr << "Ошибка! Повторяющиеся страницы: " << pageNumber << std::endl;
                return 1;
            }
            if (pageNumber > lastPage + 1) {
                std::cerr << "Ошибка! Пропуск страниц между " << lastPage << " и " << pageNumber << std::endl;
                return 1;
            }

            lastPage = pageNumber;
            continue;
        }

        if (isFirstParagraph) {
            header = str; 
            currentPage = lastPage;
            isFirstParagraph = false;
            isNewParagraph = false;
        }
        else {
            if (isNewParagraph) {
                if (!header.empty()) {
                    outputFile << header << " .......... " << currentPage << std::endl;
                }
                header = str;
                currentPage = lastPage;
                isNewParagraph = false;
            }
            //else {
            //    header += " " + str;
            //}
        }
    }
    if (!header.empty()) {
        outputFile << header << " .......... " << currentPage << std::endl;
    }

    inputFile.close();
    outputFile.close();
    std::cout << "Оглавление создано" << std::endl;
    return 0;
}
