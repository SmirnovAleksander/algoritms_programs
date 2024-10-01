//21. В   некотором   компиляторе   ПАСКАЛя  текст  программы
//включает примечания, выделенные  фигурными  скобками  '{', '}'
//либо  парами  символов  '(*'  и  '*)'.Примечания  могут быть
//вложенными друг в друга.Если примечание открыто знаком  '{',
//то оно должно быть закрыто знаком '}'.Аналогично примечание,
//начинающееся с символов '(*'  должно  заканчиваться  символами
//'*)'.Требуется:
//1) проверить правильность вложенности примечаний;
//2) переписать   файл   с   исходным   текстом   так, чтобы
//отсутствовала  вложенность  комментариев  при  сохранении   их
//содержания  и  в  качестве  ограничивающих  символов  остались
//только  фигурные  скобки.Учесть   случай, когда   символы
//примечаний находятся в апострофах.При некорректности  указать
//номера строки и позиции первой ошибки(10).
//
//Смирнов Александр ПС-21 
//Компилятор: C++
//Среда выполнеия: Visual studio 2022

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
        cout << "Ошибка: стек переполнен." << endl;
    }
}
void pop() {
    if (!isEmpty())
        top--;
    else
        cout << "Ошибка: стек пуст." << endl;
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
        cout << "Не удалось открыть файл." << endl;
    }
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cout << "Не удалось создать файл для записи." << endl;
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
                            cout << "Ошибка: лишний закрывающий символ '}' на строке " << lineNumber << ", позиция " << i << endl;
                        }
                        else {
                            cout << "Ошибка: несоответствующий закрывающий символ '}' на строке " << lineNumber << ", позиция " << i << endl;
                            cout << "Открывающий символ был найден на строке " << topElLine() << ", позиция " << topElPosition() + 1 << endl;
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
                            cout << "Ошибка: лишний закрывающий символ '*)' на строке " << lineNumber << ", позиция " << i << endl;
                        }
                        else {
                            cout << "Ошибка: несоответствующий закрывающий символ '*)' на строке " << lineNumber << ", позиция " << i << endl;
                            cout << "Открывающий символ был найден на строке " << topElLine() << ", позиция " << topElPosition() + 1 << endl;
                        }
                        hasErrors = true;
                        break;
                    }
                    else {
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
        cout << "Ошибка: незакрытый комментарий. Открывающий символ '" << topEl() << "' был найден на строке " << topElLine() << ", позиция " << topElPosition() + 1 << endl;
        hasErrors = true;
    }

    file.close();
    outputFile.close();

    if (hasErrors) {
        remove(outputFileName.c_str());
        cout << "Файл не создан из-за ошибок" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    string filename;
    string outputFileName;
    while (true) {
        cout << "Введите имя входного файла: ";
        cin >> filename;
        cout << "Введите имя выходного файла: ";
        cin >> outputFileName;
        cout << "Чтобы выйти напишите 'exit'" << endl;
        if (filename == "exit") {
            break;
        }
        checkComments(filename, outputFileName);
    }
    return 0;
}
