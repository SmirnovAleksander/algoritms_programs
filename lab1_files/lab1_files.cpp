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
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка! Файл не был открыт." << std::endl;
    }

    std::string str;
    std::string header;
    int lastPage = 0;
    bool isNewParagraph = true; //если есть пустая строка то true, после чтенея заголовка сбрасываю флаг
    bool isContent = false; //Если ничего не записано то выходной файл удаляется

    //для перой страници и не толлко
    bool singleLineOnPage = false; //изначально true пока не встретит еще одну строку на cтранице
    bool isFirstPage = true; //для определения первой стрроки
    bool onlyOneStrOnFirstPage = false; //если ошибка то фапйл е 

    while (std::getline(inputFile, str)) {
        if (str.empty()) {
            if (!header.empty()) {
                if (isFirstPage && singleLineOnPage) {
                    onlyOneStrOnFirstPage = true;
                }
                else {
                    outputFile << header << " .......... " << lastPage << std::endl;
                    isContent = true;
                }
                header = "";
                isFirstPage = false;
            }
            isNewParagraph = true;
            singleLineOnPage = false;
            continue;
        }

        if (str.length() > 2 && str[0] == '-' && str[str.length() - 1] == '-') {
            int pageNumber = 0;
            for (int i = 1; i < str.length() - 1; i++) {
                if (str[i] < '0' || str[i] > '9') {
                    std::cout << "Ошибка! Некорректный номер страницы: " << str << std::endl;
                }
                pageNumber = pageNumber * 10 + (str[i] - '0'); //строка - строка = число тк в таблице ASCII у них коды сопадают с самим числоом
            }

            if (lastPage != 0 && pageNumber == lastPage) {
                std::cerr << "Ошибка! Повторяющиеся страницы: " << pageNumber << std::endl;
            }

            if (lastPage != 0 && pageNumber > lastPage + 1) {
                std::cerr << "Ошибка! Разрыв между страницами: " << lastPage << " и " << pageNumber << std::endl;
            }

            if (!header.empty() && !singleLineOnPage) {
                outputFile << header << " .......... " << lastPage << std::endl;
                isContent = true;
                header = "";
            }

            lastPage = pageNumber;
            isNewParagraph = true;
            singleLineOnPage = false;
            continue;
        }

        if (isNewParagraph) {
            header = str;
            isNewParagraph = false;
            singleLineOnPage = true;
        }
        else {
            singleLineOnPage = false;
        }
    }
    //тк header только передался а строки уже закончились то так
    //из-за этого если поседняя страице содержит одну строку то она не будет учитываться как продолжение предыдущей страницы
    if (!header.empty() && lastPage > 0) {
        if (isFirstPage && singleLineOnPage) {
            onlyOneStrOnFirstPage = true;
        }
        else {
            outputFile << header << " .......... " << lastPage << std::endl;
            isContent = true;
        }
    }

    if (onlyOneStrOnFirstPage) {
        std::cerr << "Ошибка! На первой странице только одна строка." << std::endl;
        outputFile.close(); 
        std::remove(outputFileName.c_str()); 
    }
    else if (isContent) {
        std::cout << "Оглавление создано" << std::endl;
    }
    else {
        std::cout << "Содержимое файла пусто. Оглавление не создано" << std::endl;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}
