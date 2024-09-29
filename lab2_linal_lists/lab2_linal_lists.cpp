#include <iostream>
#include <string>
#include <fstream>

bool checkprogramcomments(const std::string& filename, int& errorline, int& errorposition);

int main() {
	setlocale(LC_ALL, "ru");

	std::string filename;
	int errorline, errorposition;

	while (true) {

		std::cout << "введите имя входного файла или введите 'exit' для выхода: ";
		std::cin >> filename;

		if (filename == "exit") {
			std::cout << "прграмма была завершена" << std::endl;
			break;
		}
//основное тело прграммы 
		if (checkprogramcomments(filename, errorline, errorposition)) {
			std::cout << "пргрмма была завершена успешно. ошибок в коментариях нет)" << std::endl;
		}
		else {
			std::cout << "прграмма была завершена с ошибкой." << std::endl;
			std::cout << "ошибка в строке " << errorline << " на позиции " << errorposition << std::endl;
		}
	}
	return 0;
}

bool checkprogramcomments(const std::string &filename, int &errorline, int &errorposition) {
	const int max_size = 100;
	char comtype[max_size]; 
	int comline[max_size]; 
	int composition[max_size];
	int top = -1;

	std::ifstream inputfile(filename);
	if (!inputfile.is_open()) {
		std::cout << "файл не был открыт!" << std::endl;
		return false;
	}

	std::string line;
	unsigned int linecount = 0;
	bool instring = false;

	while (std::getline(inputfile, line)) {
		linecount++;

		if (line.empty()) {
			continue;
		}

		for (std::size_t i = 0; i < line.size(); ++i) {
			char ch = line[i];

			if (ch == '\'') { //в паскале есть строки writeln('''hello''') -> 'hello' вот вот 
				if (i + 1 < line.size() && line[i + 1] == '\'') {
					std::cout << "найден удвоенный апостроф, пропуск...\n";
					i++; //пропускаю след элемент тк там ''
				} else {
					instring = !instring;
					/*if (instring) {
						std::cout << "начало строки в апострофах.\n";
					}
					else {
						std::cout << "конец строки в апострофах.\n";
					}*/
				}
			}
			if (!instring) {
				if (ch == '{') {
					if (top < max_size - 1) {
						comtype[++top] = '{';
						comline[top] = linecount;
						composition[top] = static_cast<int>(i + 1);
					}
					else {
						std::cout << "превышена глубина вложенности" << std::endl;
						return false;
					}
				}
				else if (ch == '(' && i + 1 < line.size() && line[i + 1] == '*') {
					if (top < max_size - 1) {
						comtype[++top] = '(';
						comline[top] = linecount;
						composition[top] = static_cast<int>(i + 1); //перевод в тип int тк у нас там был тип size_t
						i++; //пропускаю след элемент тк там (*
					}
					else {
						std::cout << "превышена глубина вложенности" << std::endl;
						return false;
					}
				}
				else if (ch == '}' && (top != -1 && comtype[top] == '{')) {
					top--;  // убираем комментарий из стека
				}
				else if (ch == '*' && i + 1 < line.size() && line[i + 1] == ')' && (top != -1 && comtype[top] == '(')) {
					top--;  // убираем комментарий из стека
					i++;  // пропускаем следующий символ ')'
				}
			}
		}
		if (instring) {
			std::cout << "ошибка: строка не закрыта в строке " << linecount << ".\n";
			return false;
		}
	}
	inputfile.close();
	if (top != -1) {
		std::cout << "остались незакрытые коментарии" << std::endl;
		errorline = comline[top];
		errorline = composition[top];
		return false;
	}

	return true;
}