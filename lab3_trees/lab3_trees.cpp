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

// Функция для загрузки дерева из файла (рекурсивная)
TreeNode* loadTree(ifstream& file) {
    string name;
    int mass;

    // Чтение имени узла
    if (!(file >> name)) {
        cerr << "Ошибка чтения имени узла!" << endl;
        return nullptr;
    }

    // Чтение массы узла
    if (!(file >> mass)) {
        cerr << "Ошибка чтения массы узла!" << endl;
        return nullptr;
    }

    TreeNode* node = new TreeNode(name, mass); // Создаем узел

    int numChildren;
    if (!(file >> numChildren)) {
        cerr << "Ошибка чтения количества потомков!" << endl;
        return nullptr;
    }

    for (int i = 0; i < numChildren; ++i) {
        TreeNode* child = loadTree(file); // Рекурсивно загружаем потомков
        if (child) {
            node->children.push_back(child);
        }
        else {
            cerr << "Ошибка загрузки потомка узла " << name << endl;
            return nullptr;
        }
    }

    return node;
}

// Функция для загрузки дерева из файла
TreeNode* loadTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return nullptr;
    }

    TreeNode* root = loadTree(file); // Загружаем дерево рекурсивно
    file.close();
    return root;
}

// Функция для вывода дерева на экран
void printTree(TreeNode* node, int level = 0) {
    if (node == nullptr) return;

    // Отступы для визуализации дерева
    for (int i = 0; i < level; ++i) {
        cout << "  ";
    }
    cout << node->name;

    if (node->mass != -1) { // Если это лист, выводим массу
        cout << " (Масса: " << node->mass << ")";
    }
    cout << endl;

    // Рекурсивно выводим дочерние узлы
    for (TreeNode* child : node->children) {
        printTree(child, level + 1);
    }
}

// Функция для обрезки дерева по массе
TreeNode* trimTree(TreeNode* node, int maxMass) {
    if (node == nullptr) return nullptr;

    // Если это листовой узел, проверяем его массу
    if (node->children.empty()) {
        return (node->mass <= maxMass) ? node : nullptr;
    }

    // Рекурсивно обрезаем дочерние узлы
    vector<TreeNode*> trimmedChildren;
    for (TreeNode* child : node->children) {
        TreeNode* trimmedChild = trimTree(child, maxMass);
        if (trimmedChild) {
            trimmedChildren.push_back(trimmedChild);
        }
    }

    node->children = trimmedChildren;

    // Если узел не имеет допустимых детей и не является листом, он удаляется
    return (!node->children.empty() || node->mass != -1) ? node : nullptr;
}

// Функция для освобождения памяти дерева
void deleteTree(TreeNode* node) {
    if (node == nullptr) return;
    for (TreeNode* child : node->children) {
        deleteTree(child);
    }
    delete node;
}

int main() {
    setlocale(LC_ALL, "RU");
    string filename = "tree.txt"; // Имя файла с деревом
    int maxMass;

    // Загружаем дерево из файла
    TreeNode* root = loadTreeFromFile(filename);
    if (!root) {
        cerr << "Ошибка загрузки дерева!" << endl;
        return 1;
    }

    cout << "Введите максимально допустимую массу: ";
    cin >> maxMass;

    cout << "Исходное дерево:" << endl;
    printTree(root);

    // Обрезаем дерево
    root = trimTree(root, maxMass);

    cout << "Обрезанное дерево:" << endl;
    printTree(root);

    // Освобождаем память
    deleteTree(root);

    return 0;
}
