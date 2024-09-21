#include <iostream>
#include <fstream>
#include <string>

void handleError(const std::string& message, int& lineNumber, int& position) {
    std::cerr << "Ошибка: " << message << " на строке " << lineNumber << " на позиции " << position << std::endl;
}

bool checkProgramComments(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Ошибка открытия входного файла!" << std::endl;
        return false;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Ошибка открытия выходного файла!" << std::endl;
        return false;
    }

    std::string line;
    int lineCount = 0;
    bool inApostrophe = false; //если внутри апострофа
    int commentSteps = 0;       // Глубина вложенности комментариев
    bool isFigureComment = false; // если фигурные скобки 

    while (std::getline(inputFile, line)) {
        ++lineCount;
        std::string thisLine;

        for (size_t i = 0; i < line.length(); i++) {
            char currChar = line[i];
            int position = i + 1;

            //Если в апосторфах то вот
            if (currChar == '\'') {
                inApostrophe = !inApostrophe;
                thisLine += currChar;
                continue;
            }
            if (inApostrophe) {
                thisLine += currChar;
                continue;
            }

            //откр. коменты
            if (currChar == '{') {
                if (commentSteps == 0) {
                    thisLine += '{';
                }
                commentSteps++;
                isFigureComment = true;
            }
            else if (currChar == '(' && i + 1 < line.length() && line[i + 1] == '*') {
                if (commentSteps == 0) {
                    thisLine += '{';
                }
                commentSteps++;
                isFigureComment = false;
                i++;
            }
            //закр. коменты
            else if (currChar == '}') {
                if (commentSteps > 0 && isFigureComment) {
                    commentSteps--;

                    if (commentSteps == 0) {
                        thisLine += '}';
                    }
                }
                else {
                    handleError("Некорректное закрытие фигурной скобки", lineCount, position);
                }
            }
            else if (currChar == '*' && i + 1 < line.length() && line[i + 1] == ')') {
                if (commentSteps > 0 && !isFigureComment) {
                    commentSteps--;

                    if (commentSteps == 0) {
                        thisLine += '}';
                    }
                }
                else {
                    handleError("Некорректное закрытие комментария '*)'", lineCount, position);
                }
                i++;
            }
            else if (commentSteps > 0) {
                thisLine += currChar; // Copy текста внутри комента            
            }
            else {
                thisLine += currChar; //copy вего текста
            }
        }

        outputFile << thisLine << std::endl;
    }

    if (commentSteps > 0) {
        std::cerr << "Ошибка: Несоответствие открытых комментариев на строке " << lineCount << std::endl;
        return false;
    }
    return true;

    inputFile.close();
    outputFile.close();
}

int main() {
    setlocale(LC_ALL, "RU");
    std::string inputFileName;
    std::string outputFileName = "output.txt";

    while (true) {
        std::cout << "Введите имя входного файла (или 'exit' для выхода): ";
        std::cin >> inputFileName;
        if (inputFileName == "exit") {
            break;
        }
        if (checkProgramComments(inputFileName, outputFileName)) {
            std::cout << "Файл обработан успешно!" << std::endl;
        }
        else {
            std::cout << "Произошла ошибка при обработке файла." << std::endl;
            std::remove(outputFileName.c_str());
        }
    }
    return 0;
}
