//24. В  листьях  И - ИЛИ  дерева, соответствующего некоторому
//множеству  конструкций, заданы   значения   массы.Известно
//максимально допустимое значение массы изделия.Требуется усечь
//дерево   так, чтобы   дерево    включало    все    элементы,
//соответствующие  допустимым  значениям массы, но не содержало
//"лишних" вершин.Конечное дерево выдать на экран в  наглядном
//виде(12).
//
//Смирнов Александр ПС-21 
//Компилятор: C++
//Среда выполнеия: Visual studio 2022

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct TreeNode {
    string name;
    int mass;
    vector<TreeNode*> children;

    TreeNode(string n, int m = -1) : name(n), mass(m) {}
};

TreeNode* loadTree(ifstream& file) {
    string name;
    int mass;

    if (!(file >> name)) {
        cerr << "Нет имени узла!" << endl;
        return nullptr;
    }
    if (!(file >> mass)) {
        cerr << "Нет массы узла!" << endl;
        return nullptr;
    }

    TreeNode* node = new TreeNode(name, mass);

    int numChildren;
    if (!(file >> numChildren)) {
        cerr << "Ошибка чтения количества потомков!" << endl;
        return nullptr;
    }

    for (int i = 0; i < numChildren; ++i) {
        TreeNode* child = loadTree(file);
        if (child) {
            node->children.push_back(child);
        }
        else {
            cerr << "Ошибка загрузки ребенка узла " << name << endl;
            return nullptr;
        }
    }

    return node;
}
TreeNode* loadTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
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
        cout << " (Масса: " << node->mass << ")";
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
        cerr << "Ошибка загрузки дерева!" << endl;
        return 1;
    }

    cout << "Введите максимально массу: ";
    cin >> maxMass;

    cout << "Исходное дерево:" << endl;
    printTree(root);

    root = trimTree(root, maxMass);

    cout << "Обрезанное дерево:" << endl;
    printTree(root);

    deleteTree(root);

    return 0;
}
