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
//		std::cout << "������� ��� �������� ����� ��� ������� 'exit' ��� ������: ";
//		std::cin >> fileName;
//
//		if (fileName == "exit") {
//			std::cout << "�������� ���� ���������" << std::endl;
//			break;
//		}
////�������� ���� �������� 
//		if (checkProgramComments(fileName, errorLine, errorPosition)) {
//			std::cout << "������� ���� ��������� �������. ������ � ����������� ���)" << std::endl;
//		}
//		else {
//			std::cout << "�������� ���� ��������� � �������." << std::endl;
//			std::cout << "������ � ������ " << errorLine << " �� ������� " << errorPosition << std::endl;
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
//		std::cout << "���� �� ��� ������!" << std::endl;
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
//			if (ch == '\'') { //� ������� ���� ������ writeln('''Hello''') -> 'Hello' ��� ��� 
//				if (i + 1 < line.size() && line[i + 1] == '\'') {
//					std::cout << "������ ��������� ��������, �������...\n";
//					i++; //��������� ���� ������� �� ��� ''
//				} else {
//					inString = !inString;
//					/*if (inString) {
//						std::cout << "������ ������ � ����������.\n";
//					}
//					else {
//						std::cout << "����� ������ � ����������.\n";
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
//						std::cout << "��������� ������� �����������" << std::endl;
//						return false;
//					}
//				}
//				else if (ch == '(' && i + 1 < line.size() && line[i + 1] == '*') {
//					if (top < max_size - 1) {
//						comType[++top] = '(';
//						comLine[top] = lineCount;
//						comPosition[top] = static_cast<int>(i + 1); //������� � ��� int �� � ��� ��� ��� ��� size_t
//						i++; //��������� ���� ������� �� ��� (*
//					}
//					else {
//						std::cout << "��������� ������� �����������" << std::endl;
//						return false;
//					}
//				}
//				else if (ch == '}' && (top != -1 && comType[top] == '{')) {
//					top--;  // ������� ����������� �� �����
//				}
//				else if (ch == '*' && i + 1 < line.size() && line[i + 1] == ')' && (top != -1 && comType[top] == '(')) {
//					top--;  // ������� ����������� �� �����
//					i++;  // ���������� ��������� ������ ')'
//				}
//			}
//		}
//		if (inString) {
//			std::cout << "������: ������ �� ������� � ������ " << lineCount << ".\n";
//			return false;
//		}
//	}
//	inputFile.close();
//	if (top != -1) {
//		std::cout << "�������� ���������� ����������" << std::endl;
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
    std::cerr << "������: " << message << " �� ������ " << lineNumber << " �� ������� " << position << std::endl;
}

bool checkProgramComments(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "������ �������� �������� �����!" << std::endl;
        return false;
    }

    if (!outputFile.is_open()) {
        std::cerr << "������ �������� ��������� �����!" << std::endl;
        return false;
    }

    std::string line;
    int lineCount = 0;
    bool inApostrophe = false; //���� ������ ���������
    int commentSteps = 0;       // ������� ����������� ������������
    bool isFigureComment = false; // ���� �������� ������ 

    while (std::getline(inputFile, line)) {
        ++lineCount;
        std::string thisLine;

        for (size_t i = 0; i < line.length(); i++) {
            char currChar = line[i];
            int position = i + 1;

            //���� � ���������� �� ���
            if (currChar == '\'') {
                inApostrophe = !inApostrophe;
                thisLine += currChar;
                continue;
            }
            if (inApostrophe) {
                thisLine += currChar;
                continue;
            }

            //����. �������
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
            //����. �������
            else if (currChar == '}') {
                if (commentSteps > 0 && isFigureComment) {
                    commentSteps--;

                    if (commentSteps == 0) {
                        thisLine += '}';
                    }
                }
                else {
                    handleError("������������ �������� �������� ������", lineCount, position);
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
                    handleError("������������ �������� ����������� '*)'", lineCount, position);
                }
                i++;
            }
            else if (commentSteps > 0) {
                thisLine += currChar; // Copy ������ ������ �������            
            }
            else {
                thisLine += currChar; //copy ���� ������
            }
        }

        outputFile << thisLine << std::endl;
    }

    if (commentSteps > 0) {
        std::cerr << "������: �������������� �������� ������������ �� ������ " << lineCount << std::endl;
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
        std::cout << "������� ��� �������� ����� (��� 'exit' ��� ������): ";
        std::cin >> inputFileName;
        if (inputFileName == "exit") {
            break;
        }
        if (checkProgramComments(inputFileName, outputFileName)) {
            std::cout << "���� ��������� �������!" << std::endl;
        }
        else {
            std::cout << "��������� ������ ��� ��������� �����." << std::endl;
            std::remove(outputFileName.c_str());
        }
    }
    return 0;
}
