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
