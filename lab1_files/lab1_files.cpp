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
        std::cerr << "Ошибка! Файл не был открыт." << std::endl;
        return 1;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка! Файл не был открыт." << std::endl;
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
            // Пустая строка указывает на конец параграфа
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
            // Переход к следующей странице
            int pageNumber = 0;
            for (size_t i = 1; i < str.length() - 1; ++i) {
                if (str[i] < '0' || str[i] > '9') {
                    std::cerr << "Ошибка! Некорректный номер страницы: " << str << std::endl;
                    return 1;
                }
                pageNumber = pageNumber * 10 + (str[i] - '0');
            }

            if (lastPage != 0 && pageNumber == lastPage) {
                std::cerr << "Ошибка! Повторяющиеся страницы: " << pageNumber << std::endl;
                return 1;
            }

            if (lastPage != 0 && pageNumber > lastPage + 1) {
                std::cerr << "Ошибка! Разрыв между страницами: " << lastPage << " и " << pageNumber << std::endl;
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
            // Сохраняем заголовок параграфа
            header = str;
            isNewParagraph = false;
            singleLineOnPage = true; // Предполагаем, что это единственная строка на странице
        }
        else {
            // Если на странице есть текст и это не первый заголовок
            singleLineOnPage = false;
        }
    }

    // Записываем последний заголовок и номер страницы
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
        std::cerr << "Ошибка! На первой странице только одна строка." << std::endl;
        outputFile.close();  // Закрываем выходной файл, если была ошибка
        std::remove(outputFileName.c_str());  // Удаляем выходной файл, чтобы не оставлять пустой файл
    }
    else if (isContent) {
        std::cout << "Оглавление создано" << std::endl;
    }
    else {
        std::cout << "Содержимое файла пусто. Оглавление не создано" << std::endl;
    }

    inputFile.close();
    return 0;
}
