#include <iostream>
#include <string>
#include <fstream>

bool checkprogramcomments(const std::string& filename, int& errorline, int& errorposition);

int main() {
	setlocale(LC_ALL, "ru");

	std::string filename;
	int errorline, errorposition;

	while (true) {

		std::cout << "������� ��� �������� ����� ��� ������� 'exit' ��� ������: ";
		std::cin >> filename;

		if (filename == "exit") {
			std::cout << "�������� ���� ���������" << std::endl;
			break;
		}
//�������� ���� �������� 
		if (checkprogramcomments(filename, errorline, errorposition)) {
			std::cout << "������� ���� ��������� �������. ������ � ����������� ���)" << std::endl;
		}
		else {
			std::cout << "�������� ���� ��������� � �������." << std::endl;
			std::cout << "������ � ������ " << errorline << " �� ������� " << errorposition << std::endl;
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
		std::cout << "���� �� ��� ������!" << std::endl;
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

			if (ch == '\'') { //� ������� ���� ������ writeln('''hello''') -> 'hello' ��� ��� 
				if (i + 1 < line.size() && line[i + 1] == '\'') {
					std::cout << "������ ��������� ��������, �������...\n";
					i++; //��������� ���� ������� �� ��� ''
				} else {
					instring = !instring;
					/*if (instring) {
						std::cout << "������ ������ � ����������.\n";
					}
					else {
						std::cout << "����� ������ � ����������.\n";
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
						std::cout << "��������� ������� �����������" << std::endl;
						return false;
					}
				}
				else if (ch == '(' && i + 1 < line.size() && line[i + 1] == '*') {
					if (top < max_size - 1) {
						comtype[++top] = '(';
						comline[top] = linecount;
						composition[top] = static_cast<int>(i + 1); //������� � ��� int �� � ��� ��� ��� ��� size_t
						i++; //��������� ���� ������� �� ��� (*
					}
					else {
						std::cout << "��������� ������� �����������" << std::endl;
						return false;
					}
				}
				else if (ch == '}' && (top != -1 && comtype[top] == '{')) {
					top--;  // ������� ����������� �� �����
				}
				else if (ch == '*' && i + 1 < line.size() && line[i + 1] == ')' && (top != -1 && comtype[top] == '(')) {
					top--;  // ������� ����������� �� �����
					i++;  // ���������� ��������� ������ ')'
				}
			}
		}
		if (instring) {
			std::cout << "������: ������ �� ������� � ������ " << linecount << ".\n";
			return false;
		}
	}
	inputfile.close();
	if (top != -1) {
		std::cout << "�������� ���������� ����������" << std::endl;
		errorline = comline[top];
		errorline = composition[top];
		return false;
	}

	return true;
}