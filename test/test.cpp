#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int max_size = 1000;
char stack[max_size];
int stackPosition[max_size];
int stackLine[max_size];
int top = -1; 

bool isEmpty() {
    return top == -1;
}
void push(char symbol, int position, int lineNumber) {
    if (top < max_size - 1) {
        top++;
        stack[top] = symbol;
        stackPosition[top] = position;
        stackLine[top] = lineNumber;
    }
    else {
        cout << "������: ���� ����������." << endl;
    }
}
void pop() {
    if (!isEmpty())
        top--;
    else
        cout << "������: ���� ����." << endl;
}
char topEl() {
    if (!isEmpty()) {
        return stack[top];
    }
    return '\0';
}
int topElPosition() {
    if (!isEmpty()) {
        return stackPosition[top];
    }
    return -1;
}
int topElLine() {
    if (!isEmpty()) {
        return stackLine[top];
    }
    return -1;
}

void checkComments(const string& filename, string& outputFileName) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "�� ������� ������� ����." << endl;
    }
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cout << "�� ������� ������� ���� ��� ������." << endl;
    }

    top = -1;
    bool inQuotes = false;
    string line;
    bool hasErrors = false;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] == '\'') {
                inQuotes = !inQuotes;
            }

            if (!inQuotes) {
                if (line[i] == '{') {
                    push('{', i, lineNumber);
                    if (top == 0) {
                        outputFile << '{';
                    }
                }
                else if (line[i] == '(' && i + 1 < line.length() && line[i + 1] == '*') {
                    push('(', i, lineNumber);
                    if (top == 0) {
                        outputFile << '{';
                    }
                    i++;
                }
                else if (line[i] == '}') {
                    if (isEmpty() || topEl() != '{') {
                        if (isEmpty()) {
                            cout << "������: ������ ����������� ������ '}' �� ������ " << lineNumber << ", ������� " << i << endl;
                        }
                        else {
                            cout << "������: ����������������� ����������� ������ '}' �� ������ " << lineNumber << ", ������� " << i << endl;
                            cout << "����������� ������ ��� ������ �� ������ " << topElLine() << ", ������� " << topElPosition() + 1 << endl;
                        }
                        hasErrors = true;
                        break;
                    }
                    else {
                        pop();   
                        if (isEmpty() || top == -1) {
                            outputFile << '}';
                        }
                    }
                    
                }
                else if (line[i] == '*' && i + 1 < line.length() && line[i + 1] == ')') {
                    if (isEmpty() || topEl() != '(') {
                        if (isEmpty()) {
                            cout << "������: ������ ����������� ������ '*)' �� ������ " << lineNumber << ", ������� " << i << endl;
                        }
                        else {
                            cout << "������: ����������������� ����������� ������ '*)' �� ������ " << lineNumber << ", ������� " << i << endl;
                            cout << "����������� ������ ��� ������ �� ������ " << topElLine() << ", ������� " << topElPosition() + 1 << endl;
                        }
                        hasErrors = true;
                        break;
                    } else {
                        pop();
                        if (isEmpty() || top == -1) {
                            outputFile << '}';
                        }
                        i++;
                    }
                } 
                else {
                    outputFile << line[i];
                }
            }
            else {
                outputFile << line[i];
            }
        }
        outputFile << endl;
    }

    if (!isEmpty()) {
        cout << "������: ���������� �����������. ����������� ������ '" << topEl() << "' ��� ������ �� ������ " << topElLine() << ", ������� " << topElPosition() + 1 << endl;
        hasErrors = true;
    }

    file.close();
    outputFile.close();

    if (hasErrors) {
        remove(outputFileName.c_str());
        cout << "���� �� ������ ��-�� ������" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    string filename;
    string outputFileName;
    while (true) {
        cout << "������� ��� �������� �����: ";
        cin >> filename;
        cout << "������� ��� ��������� �����: ";
        cin >> outputFileName;
        cout << "����� ����� �������� 'exit'" << endl;
        if (filename == "exit") {
            break;
        }
        checkComments(filename, outputFileName);
    }
    return 0;
}
