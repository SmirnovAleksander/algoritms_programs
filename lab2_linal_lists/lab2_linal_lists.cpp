#include <iostream>
#include <string>
#include <fstream>

bool checkProgramComments(const std::string& fileName, int& errorLine, int& errorPosition);

int main() {
	setlocale(LC_ALL, "RU");

	std::string fileName;
	int errorLine, errorPosition;

	while (true) {

		std::cout << "Введите имя входного файла или введите 'exit' для выхода: ";
		std::cin >> fileName;

		if (fileName == "exit") {
			std::cout << "Прграмма была завершена" << std::endl;
			break;
		}
//основное тело прграммы 
		if (checkProgramComments(fileName, errorLine, errorPosition)) {
			std::cout << "Пргрмма была завершена успешно. Ошибок в коментариях нет)";
		}
		else {
			std::cout << "Пргрмма была завершена с ошибкой." << std::endl;
			std::cout << "Ошибка в строке " << errorLine << " на позиции " << errorPosition << std::endl;
		}
	}
	return 0;
}

bool checkProgramComments(const std::string &fileName, int &errorLine, int& errorPosition) {
	const int max_size = 100;
	char comType[max_size]; 
	int comLine[max_size]; 
	int comPosition[max_size];
	int top = -1;

	std::ifstream inputFile(fileName);
	if (!inputFile.is_open()) {
		std::cout << "Файл не был открыт!" << std::endl;
		return false;
	}

	std::string line;
	unsigned int lineCount = 0;
	bool inString = false;
	char prevCh = '\0';

	while (std::getline(inputFile, line)) {
		lineCount++;

		if (line.empty()) {
			continue;
		}

		for (std::size_t i = 0; i < line.size(); ++i) {
			char ch = line[i];

			if (ch == '\'') { //в паскале есть строки writeln('''Hello''') -> 'Hello' вот вот 
				if (i + 1 < line.size() && line[i + 1] == '\'') {
					std::cout << "Найден удвоенный апостроф, пропуск...\n";
					i++; //пропускаю след элемент тк там ''
				} else {
					inString = !inString;
					if (inString) {
						std::cout << "Начало строки в апострофах.\n";
					}
					else {
						std::cout << "Конец строки в апострофах.\n";
					}
				}
			}

			if (!inString) {
				if (ch == '{') {
					if (top < max_size - 1) {
						comType[++top] = '{';
						comLine[top] = lineCount;
						comPosition[top] = static_cast<int>(i);
					}
					else {
						std::cout << "Превышена глубина вложенности" << std::endl;
						return false;
					}
				}
				else if (ch == '(' && i + 1 < line.size() && line[i + 1] == '*') {
					if (top < max_size - 1) {
						comType[++top] = '(';
						comLine[top] = lineCount;
						comPosition[top] = static_cast<int>(i); //перевод в тип int тк у нас там был тип size_t
						i++; //пропускаю след элемент тк там (*
					}
					else {
						std::cout << "Превышена глубина вложенности" << std::endl;
						return false;
					}
				}
				else if (ch == '}') {
					if (top == -1 || comType[top] != '{') { //если стек заночился а появился новый элеменнт 
						//или если закр скобка не соответст открывающейся
						errorLine = lineCount;
						errorPosition = i;
						return false; //все плохо)
					}
					top--; //очищаю верхний элемент стека
				}
				else if (ch == '*' && i + 1 < line.size() && line[i + 1] == ')') {
					if (top == -1 || comType[top] != '{') {
						errorLine = lineCount;
						errorPosition = i;
						return false; //так же все плохо)
					}
					top--;
					i++;
				}
			}
			prevCh = ch;
		}
		if (inString) {
			std::cout << "Ошибка: строка не закрыта в строке " << lineCount << ".\n";
			return false;
		}
	}
	inputFile.close();
	if (top != -1) {
		std::cout << "Остались незакрытые коментарии" << std::endl;
		errorLine = comLine[top];
		errorLine = comPosition[top];
		return false;
	}

	return true;
}