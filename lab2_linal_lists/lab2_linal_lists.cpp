//#include <iostream>
//#include <string>
//#include <fstream>
//
//bool checkProgramComments(const std::string& fileName, int& errorLine, int& errorPosition);
//
//int main() {
//	setlocale(LC_ALL, "RU");
//
//	std::string fileName;
//	int errorLine, errorPosition;
//
//	while (true) {
//
//		std::cout << "Введите имя входного файла или введите 'exit' для выхода: ";
//		std::cin >> fileName;
//
//		if (fileName == "exit") {
//			std::cout << "Прграмма была завершена" << std::endl;
//			break;
//		}
////основное тело прграммы 
//		if (checkProgramComments(fileName, errorLine, errorPosition)) {
//			std::cout << "Пргрмма была завершена успешно. Ошибок в коментариях нет)" << std::endl;
//		}
//		else {
//			std::cout << "Прграмма была завершена с ошибкой." << std::endl;
//			std::cout << "Ошибка в строке " << errorLine << " на позиции " << errorPosition << std::endl;
//		}
//	}
//	return 0;
//}
//
//bool checkProgramComments(const std::string &fileName, int &errorLine, int &errorPosition) {
//	const int max_size = 100;
//	char comType[max_size]; 
//	int comLine[max_size]; 
//	int comPosition[max_size];
//	int top = -1;
//
//	std::ifstream inputFile(fileName);
//	if (!inputFile.is_open()) {
//		std::cout << "Файл не был открыт!" << std::endl;
//		return false;
//	}
//
//	std::string line;
//	unsigned int lineCount = 0;
//	bool inString = false;
//
//	while (std::getline(inputFile, line)) {
//		lineCount++;
//
//		if (line.empty()) {
//			continue;
//		}
//
//		for (std::size_t i = 0; i < line.size(); ++i) {
//			char ch = line[i];
//
//			if (ch == '\'') { //в паскале есть строки writeln('''Hello''') -> 'Hello' вот вот 
//				if (i + 1 < line.size() && line[i + 1] == '\'') {
//					std::cout << "Найден удвоенный апостроф, пропуск...\n";
//					i++; //пропускаю след элемент тк там ''
//				} else {
//					inString = !inString;
//					/*if (inString) {
//						std::cout << "Начало строки в апострофах.\n";
//					}
//					else {
//						std::cout << "Конец строки в апострофах.\n";
//					}*/
//				}
//			}
//			if (!inString) {
//				if (ch == '{') {
//					if (top < max_size - 1) {
//						comType[++top] = '{';
//						comLine[top] = lineCount;
//						comPosition[top] = static_cast<int>(i + 1);
//					}
//					else {
//						std::cout << "Превышена глубина вложенности" << std::endl;
//						return false;
//					}
//				}
//				else if (ch == '(' && i + 1 < line.size() && line[i + 1] == '*') {
//					if (top < max_size - 1) {
//						comType[++top] = '(';
//						comLine[top] = lineCount;
//						comPosition[top] = static_cast<int>(i + 1); //перевод в тип int тк у нас там был тип size_t
//						i++; //пропускаю след элемент тк там (*
//					}
//					else {
//						std::cout << "Превышена глубина вложенности" << std::endl;
//						return false;
//					}
//				}
//				else if (ch == '}' && (top != -1 && comType[top] == '{')) {
//					top--;  // Убираем комментарий из стека
//				}
//				else if (ch == '*' && i + 1 < line.size() && line[i + 1] == ')' && (top != -1 && comType[top] == '(')) {
//					top--;  // Убираем комментарий из стека
//					i++;  // Пропускаем следующий символ ')'
//				}
//			}
//		}
//		if (inString) {
//			std::cout << "Ошибка: строка не закрыта в строке " << lineCount << ".\n";
//			return false;
//		}
//	}
//	inputFile.close();
//	if (top != -1) {
//		std::cout << "Остались незакрытые коментарии" << std::endl;
//		errorLine = comLine[top];
//		errorLine = comPosition[top];
//		return false;
//	}
//
//	return true;
//}

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
