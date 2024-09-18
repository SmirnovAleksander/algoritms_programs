#include <iostream>
#include <string>
#include <fstream>

bool checkProgramComments(const std::string& fileName, int& errorLine, int& errorPosition);

int main() {
	setlocale(LC_ALL, "RU");

	std::string fileName;
	int errorLine, errorPosition;

	while (true) {

		std::cout << "������� ��� �������� ����� ��� ������� 'exit' ��� ������: ";
		std::cin >> fileName;

		if (fileName == "exit") {
			std::cout << "�������� ���� ���������" << std::endl;
			break;
		}
//�������� ���� �������� 
		if (checkProgramComments(fileName, errorLine, errorPosition)) {
			std::cout << "������� ���� ��������� �������. ������ � ����������� ���)";
		}
		else {
			std::cout << "������� ���� ��������� � �������." << std::endl;
			std::cout << "������ � ������ " << errorLine << " �� ������� " << errorPosition << std::endl;
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
		std::cout << "���� �� ��� ������!" << std::endl;
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

			if (ch == '\'') { //� ������� ���� ������ writeln('''Hello''') -> 'Hello' ��� ��� 
				if (i + 1 < line.size() && line[i + 1] == '\'') {
					std::cout << "������ ��������� ��������, �������...\n";
					i++; //��������� ���� ������� �� ��� ''
				} else {
					inString = !inString;
					if (inString) {
						std::cout << "������ ������ � ����������.\n";
					}
					else {
						std::cout << "����� ������ � ����������.\n";
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
						std::cout << "��������� ������� �����������" << std::endl;
						return false;
					}
				}
				else if (ch == '(' && i + 1 < line.size() && line[i + 1] == '*') {
					if (top < max_size - 1) {
						comType[++top] = '(';
						comLine[top] = lineCount;
						comPosition[top] = static_cast<int>(i); //������� � ��� int �� � ��� ��� ��� ��� size_t
						i++; //��������� ���� ������� �� ��� (*
					}
					else {
						std::cout << "��������� ������� �����������" << std::endl;
						return false;
					}
				}
				else if (ch == '}') {
					if (top == -1 || comType[top] != '{') { //���� ���� ��������� � �������� ����� �������� 
						//��� ���� ���� ������ �� ��������� �������������
						errorLine = lineCount;
						errorPosition = i;
						return false; //��� �����)
					}
					top--; //������ ������� ������� �����
				}
				else if (ch == '*' && i + 1 < line.size() && line[i + 1] == ')') {
					if (top == -1 || comType[top] != '{') {
						errorLine = lineCount;
						errorPosition = i;
						return false; //��� �� ��� �����)
					}
					top--;
					i++;
				}
			}
			prevCh = ch;
		}
		if (inString) {
			std::cout << "������: ������ �� ������� � ������ " << lineCount << ".\n";
			return false;
		}
	}
	inputFile.close();
	if (top != -1) {
		std::cout << "�������� ���������� ����������" << std::endl;
		errorLine = comLine[top];
		errorLine = comPosition[top];
		return false;
	}

	return true;
}