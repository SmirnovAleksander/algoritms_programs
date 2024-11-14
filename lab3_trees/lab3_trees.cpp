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

enum NodeType { LEAF, AND, OR };

struct TreeNode {
    string name;
    int mass;
    NodeType type;
    vector<TreeNode*> children;

    TreeNode(string n, int m = -1, NodeType t = LEAF) : name(n), mass(m), type(t) {}
};

TreeNode* loadTree(ifstream& file) {
    vector<TreeNode*> nodeStack;
    string line;

    while (getline(file, line)) {
        int level = 0;
        while (level < line.size() && line[level] == '.') {
            level++;
        }

        line = line.substr(level);

        istringstream ss(line);
        string name;
        int mass;
        string typeStr;
        ss >> name >> typeStr >> mass;

        NodeType type = LEAF;
        if (typeStr == "AND") type = AND;
        else if (typeStr == "OR") type = OR;

        TreeNode* node = new TreeNode(name, mass, type);

        if (level == 0) {
            nodeStack.clear();
            nodeStack.push_back(node);
        }
        else {
            if (level > nodeStack.size()) {
                cerr << "������: ������� ����������� �������!" << endl;
                delete node;
                return nullptr;
            }

            TreeNode* parent = nodeStack[level - 1];
            parent->children.push_back(node);

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

    if (node->type == AND) cout << " [AND]";
    else if (node->type == OR) cout << " [OR]";

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

    if (node->mass > maxMass) {
        return nullptr;
    }

    vector<TreeNode*> trimmedChildren;
    for (TreeNode* child : node->children) {
        TreeNode* trimmedChild = trimTree(child, maxMass);
        if (trimmedChild) {
            trimmedChildren.push_back(trimmedChild);
        }
    }

    if (node->type == AND) {
        if (trimmedChildren.size() == node->children.size()) {
            node->children = trimmedChildren;
            return node;
        }
        delete node;
        return nullptr;
    }
    else if (node->type == OR) {
        if (!trimmedChildren.empty()) {
            node->children = trimmedChildren;
            return node;
        }
        delete node;
        return nullptr;
    }

    return node;
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

    cout << "������� ����������� ���������� �����: ";
    cin >> maxMass;

    cout << "�������� ������:" << endl;
    printTree(root);

    root = trimTree(root, maxMass);

    cout << "���������� ������:" << endl;
    printTree(root);

    deleteTree(root);

    return 0;
}
