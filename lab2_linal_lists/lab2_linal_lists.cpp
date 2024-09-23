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

//#include <iostream>
//#include <fstream>
//#include <string>
//
//void handleError(const std::string& message, int& lineNumber, int& position) {
//    std::cerr << "������: " << message << " �� ������ " << lineNumber << " �� ������� " << position << std::endl;
//}
//
//bool checkProgramComments(const std::string& inputFileName, const std::string& outputFileName) {
//    std::ifstream inputFile(inputFileName);
//    std::ofstream outputFile(outputFileName);
//
//    if (!inputFile.is_open()) {
//        std::cerr << "������ �������� �������� �����!" << std::endl;
//        return false;
//    }
//
//    if (!outputFile.is_open()) {
//        std::cerr << "������ �������� ��������� �����!" << std::endl;
//        return false;
//    }
//
//    std::string line;
//    int lineCount = 0;
//    bool inApostrophe = false; //���� ������ ���������
//    int commentSteps = 0;       // ������� ����������� ������������
//    bool isFigureComment = false; // ���� �������� ������ 
//
//    while (std::getline(inputFile, line)) {
//        ++lineCount;
//        std::string thisLine;
//
//        for (size_t i = 0; i < line.length(); i++) {
//            char currChar = line[i];
//            int position = i + 1;
//
//            //���� � ���������� �� ���
//            if (currChar == '\'') {
//                inApostrophe = !inApostrophe;
//                thisLine += currChar;
//                continue;
//            }
//            if (inApostrophe) {
//                thisLine += currChar;
//                continue;
//            }
//
//            //����. �������
//            if (currChar == '{') {
//                if (commentSteps == 0) {
//                    thisLine += '{';
//                }
//                commentSteps++;
//                isFigureComment = true;
//            }
//            else if (currChar == '(' && i + 1 < line.length() && line[i + 1] == '*') {
//                if (commentSteps == 0) {
//                    thisLine += '{';
//                }
//                commentSteps++;
//                isFigureComment = false;
//                i++;
//            }
//            //����. �������
//            else if (currChar == '}') {
//                if (commentSteps > 0 && isFigureComment) {
//                    commentSteps--;
//
//                    if (commentSteps == 0) {
//                        thisLine += '}';
//                    }
//                }
//                else {
//                    handleError("������������ �������� �������� ������", lineCount, position);
//                }
//            }
//            else if (currChar == '*' && i + 1 < line.length() && line[i + 1] == ')') {
//                if (commentSteps > 0 && !isFigureComment) {
//                    commentSteps--;
//
//                    if (commentSteps == 0) {
//                        thisLine += '}';
//                    }
//                }
//                else {
//                    handleError("������������ �������� ����������� '*)'", lineCount, position);
//                }
//                i++;
//            }
//            else if (commentSteps > 0) {
//                thisLine += currChar; // Copy ������ ������ �������            
//            }
//            else {
//                thisLine += currChar; //copy ���� ������
//            }
//        }
//
//        outputFile << thisLine << std::endl;
//    }
//
//    if (commentSteps > 0) {
//        std::cerr << "������: �������������� �������� ������������ �� ������ " << lineCount << std::endl;
//        return false;
//    }
//    return true;
//
//    inputFile.close();
//    outputFile.close();
//}
//
//int main() {
//    setlocale(LC_ALL, "RU");
//    std::string inputFileName;
//    std::string outputFileName = "output.txt";
//
//    while (true) {
//        std::cout << "������� ��� �������� ����� (��� 'exit' ��� ������): ";
//        std::cin >> inputFileName;
//        if (inputFileName == "exit") {
//            break;
//        }
//        if (checkProgramComments(inputFileName, outputFileName)) {
//            std::cout << "���� ��������� �������!" << std::endl;
//        }
//        else {
//            std::cout << "��������� ������ ��� ��������� �����." << std::endl;
//            std::remove(outputFileName.c_str());
//        }
//    }
//    return 0;
//}
