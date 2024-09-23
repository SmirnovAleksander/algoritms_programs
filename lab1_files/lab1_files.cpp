//13. �������   �����   ���������, ���������  ��  ����������
//����������.������ �������� ���������� � ��������� �  �������
//������  �������  ��  ����������� ���������.����� �������� ��
//��������.����� �������� ���������� � �� ������ �  �������  �
//�����  ������  ������  '-'.������������  ����  � �����������
//���������(7).
//
//������� ��������� ��-21 
//����������: C++
//����� ���������: Visual studio 2022

#include <iostream>
#include <fstream>
#include <string>

int main()
{
    setlocale(LC_ALL, "RU");

    std::string inputFileName, outputFileName;

    std::cout << "������� �������� �������� �����: ";
    std::cin >> inputFileName;
    std::cout << "������� �������� ��������� �����: ";
    std::cin >> outputFileName;

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "������! ���� �� ��� ������." << std::endl;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "������! ���� �� ��� ������." << std::endl;
    }

    std::string str;
    std::string header;
    int lastPage = 0;
    bool isNewParagraph = true; //���� ���� ������ ������ �� true, ����� ������ ��������� ��������� ����
    bool isContent = false; //���� ������ �� �������� �� �������� ���� ���������

    //��� ����� �������� � �� ������
    bool singleLineOnPage = false; //���������� true ���� �� �������� ��� ���� ������ �� c�������
    bool isFirstPage = true; //��� ����������� ������ �������
    bool onlyOneStrOnFirstPage = false; //���� ������ �� ����� � 

    while (std::getline(inputFile, str)) {
        if (str.empty()) {
            if (!header.empty()) {
                if (isFirstPage && singleLineOnPage) {
                    onlyOneStrOnFirstPage = true;
                }
                else {
                    outputFile << header << " .......... " << lastPage << std::endl;
                    isContent = true;
                }
                header = "";
                isFirstPage = false;
            }
            isNewParagraph = true;
            singleLineOnPage = false;
            continue;
        }

        if (str.length() > 2 && str[0] == '-' && str[str.length() - 1] == '-') {
            int pageNumber = 0;
            for (int i = 1; i < str.length() - 1; i++) {
                if (str[i] < '0' || str[i] > '9') {
                    std::cout << "������! ������������ ����� ��������: " << str << std::endl;
                }
                pageNumber = pageNumber * 10 + (str[i] - '0'); //������ - ������ = ����� �� � ������� ASCII � ��� ���� �������� � ����� �������
            }

            if (lastPage != 0 && pageNumber == lastPage) {
                std::cerr << "������! ������������� ��������: " << pageNumber << std::endl;
            }

            if (lastPage != 0 && pageNumber > lastPage + 1) {
                std::cerr << "������! ������ ����� ����������: " << lastPage << " � " << pageNumber << std::endl;
            }

            if (!header.empty() && !singleLineOnPage) {
                outputFile << header << " .......... " << lastPage << std::endl;
                isContent = true;
                header = "";
            }

            lastPage = pageNumber;
            isNewParagraph = true;
            singleLineOnPage = false;
            continue;
        }

        if (isNewParagraph) {
            header = str;
            isNewParagraph = false;
            singleLineOnPage = true;
        }
        else {
            singleLineOnPage = false;
        }
    }
    //�� header ������ ��������� � ������ ��� ����������� �� ���
    //��-�� ����� ���� �������� ������� �������� ���� ������ �� ��� �� ����� ����������� ��� ����������� ���������� ��������
    if (!header.empty() && lastPage > 0) {
        if (isFirstPage && singleLineOnPage) {
            onlyOneStrOnFirstPage = true;
        }
        else {
            outputFile << header << " .......... " << lastPage << std::endl;
            isContent = true;
        }
    }

    if (onlyOneStrOnFirstPage) {
        std::cerr << "������! �� ������ �������� ������ ���� ������." << std::endl;
        outputFile.close(); 
        std::remove(outputFileName.c_str()); 
    }
    else if (isContent) {
        std::cout << "���������� �������" << std::endl;
    }
    else {
        std::cout << "���������� ����� �����. ���������� �� �������" << std::endl;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}
