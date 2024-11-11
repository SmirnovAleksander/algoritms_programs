//24. �  �������  � - ���  ������, ���������������� ����������
//���������  �����������, ������   ��������   �����.��������
//����������� ���������� �������� ����� �������.��������� �����
//������   ���, �����   ������    ��������    ���    ��������,
//���������������  ����������  ��������� �����, �� �� ���������
//"������" ������.�������� ������ ������ �� ����� �  ���������
//����(12).
//
//������� ��������� ��-21 
//����������: C++
//����� ���������: Visual studio 2022

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct TreeNode {
    string name;
    int mass;
    vector<TreeNode*> children;

    TreeNode(string n, int m = -1) : name(n), mass(m) {}
};

TreeNode* loadTree(ifstream& file) {
    vector<TreeNode*> nodeStack;
    string line;

    while (getline(file, line)) {
        // ���������� ������� ����������� �� ���������� �����
        int level = 0;
        while (level < line.size() && line[level] == '.') {
            level++;
        }

        // ������� ����� �� ������
        line = line.substr(level);

        // ��������� ��� � �����
        istringstream ss(line);
        string name;
        int mass;
        ss >> name >> mass;

        // ������� ����� ����
        TreeNode* node = new TreeNode(name, mass);

        // ������������� �������� ��� ���� � ������������ � �������
        if (level == 0) {
            // �������� ����
            nodeStack.clear();
            nodeStack.push_back(node);
        }
        else {
            // �������� �������� � ����������� ������
            if (level > nodeStack.size()) {
                cerr << "������: ������� ����������� �������!" << endl;
                delete node;
                return nullptr;
            }

            TreeNode* parent = nodeStack[level - 1];
            parent->children.push_back(node);

            // ��������� ���� �����
            if (level < nodeStack.size()) {
                nodeStack[level] = node;
            }
            else {
                nodeStack.push_back(node);
            }
        }
    }

    return nodeStack.empty() ? nullptr : nodeStack[0];
}
TreeNode* loadTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "������ �������� �����!" << endl;
        return nullptr;
    }

    TreeNode* root = loadTree(file);
    file.close();
    return root;
}
void printTree(TreeNode* node, int level = 0) {
    if (node == nullptr) return;
    for (int i = 0; i < level; ++i) {
        cout << "  ";
    }
    cout << node->name;

    if (node->mass != -1) {
        cout << " (�����: " << node->mass << ")";
    }
    cout << endl;

    for (TreeNode* child : node->children) {
        printTree(child, level + 1);
    }
}
TreeNode* trimTree(TreeNode* node, int maxMass) {
    if (node == nullptr) return nullptr;
    if (node->children.empty()) {
        return (node->mass <= maxMass) ? node : nullptr;
    }
    vector<TreeNode*> trimmedChildren;
    for (TreeNode* child : node->children) {
        TreeNode* trimmedChild = trimTree(child, maxMass);
        if (trimmedChild) {
            trimmedChildren.push_back(trimmedChild);
        }
    }

    node->children = trimmedChildren;
    return (!node->children.empty() || node->mass != -1) ? node : nullptr;
}
void deleteTree(TreeNode* node) {
    if (node == nullptr) return;
    for (TreeNode* child : node->children) {
        deleteTree(child);
    }
    delete node;
}

int main() {
    setlocale(LC_ALL, "RU");
    string filename = "tree.txt";
    int maxMass;
    TreeNode* root = loadTreeFromFile(filename);
    if (!root) {
        cerr << "������ �������� ������!" << endl;
        return 1;
    }

    cout << "������� ����������� �����: ";
    cin >> maxMass;

    cout << "�������� ������:" << endl;
    printTree(root);

    root = trimTree(root, maxMass);

    cout << "���������� ������:" << endl;
    printTree(root);

    deleteTree(root);

    return 0;
}
