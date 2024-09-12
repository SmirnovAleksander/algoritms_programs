#include <iostream>
#include <fstream>
#include <string>

int main()
{
    setlocale(LC_ALL, "RU");

    std::string inputFileName, outputFileName;

    std::cout << "Введите название входного файла: ";
    std::cin >> inputFileName;
    std::cout << "Введите название выходного файла: ";
    std::cin >> outputFileName;

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cout << "Ошибка! Файл не был открыт." << std::endl;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cout << "Ошибка! Файл не был открыт." << std::endl;
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
            currentPage = atoi(str.c_str() + 1); //он atoi принимает не String а char*, а c_str возвращает указатель на символ
            if (lastPage != 0 && currentPage > lastPage + 1) {
                std::cout << "У страницы " << currentPage << " разрывс с " << lastPage << " более чем на одну страницу" << std::endl;
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
        std::cout << "Оглавление создано" << std::endl;
    else
        std::cout << "Содержимое файла пусто. Оглавление не создано" << std::endl;

    inputFile.close();
    outputFile.close();
    return 0;
}