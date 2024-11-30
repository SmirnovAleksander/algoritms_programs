#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

enum NodeType { LEAF, AND, OR };

struct TreeNode {
    string name;
    int mass;
    NodeType type;
    vector<TreeNode*> children;
    int minMass;
    int maxMass;
    int limitMass;

    TreeNode(string n, int m = -1, NodeType t = LEAF) : name(n), mass(m), type(t), minMass(0), maxMass(0), limitMass(0) {}
};

// Загрузка дерева
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

        int adjustedMass = (type == LEAF) ? mass : -1;

        TreeNode* node = new TreeNode(name, adjustedMass, type);

        if (level == 0) {
            nodeStack.clear();
            nodeStack.push_back(node);
        }
        else {
            if (level > nodeStack.size()) {
                cerr << "Ошибка: уровень вложенности нарушен!" << endl;
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

// Снизу вверх: расчёт минимальной и максимальной массы для каждого узла
void calculateMassRanges(TreeNode* node) {
    if (!node) return;

    if (node->type == LEAF) {
        node->minMass = node->mass;
        node->maxMass = node->mass;
        return;
    }

    int minMass = 0;
    int maxMass = 0;

    for (TreeNode* child : node->children) {
        calculateMassRanges(child);

        if (node->type == AND) {
            minMass += child->minMass;
            maxMass += child->maxMass;
        }
        else if (node->type == OR) {
            minMass = (minMass == 0) ? child->minMass : min(minMass, child->minMass);
            maxMass = max(maxMass, child->maxMass);
        }
    }

    node->minMass = minMass;
    node->maxMass = maxMass;
}

// Сверху вниз: обрезка дерева на основе ограничений
TreeNode* trimTree(TreeNode* node, int maxMass) {
    if (!node) return nullptr;

    if (maxMass < node->minMass) return nullptr;

    if (node->type == LEAF) {
        return (node->mass <= maxMass) ? node : nullptr;
    }

    vector<TreeNode*> trimmedChildren;

    if (node->type == AND) {
        int remainingMass = maxMass;

        // Учёт минимальных масс дочерних узлов
        for (TreeNode* child : node->children) {
            remainingMass -= child->minMass;
        }

        for (TreeNode* child : node->children) {
            int childMaxMass = remainingMass + child->minMass;
            TreeNode* trimmedChild = trimTree(child, childMaxMass);
            if (trimmedChild) {
                trimmedChildren.push_back(trimmedChild);
            }
        }

        if (trimmedChildren.size() == node->children.size()) {
            node->children = trimmedChildren;
            node->limitMass = maxMass;
            return node;
        }
        else {
            delete node;
            return nullptr;
        }
    }
    else if (node->type == OR) {
        for (TreeNode* child : node->children) {
            TreeNode* trimmedChild = trimTree(child, maxMass);
            if (trimmedChild) {
                trimmedChildren.push_back(trimmedChild);
            }
        }

        if (!trimmedChildren.empty()) {
            node->children = trimmedChildren;
            node->limitMass = maxMass;
            return node;
        }
        else {
            delete node;
            return nullptr;
        }
    }

    return nullptr;
}

// Удаление дерева
void deleteTree(TreeNode* node) {
    if (!node) return;
    for (TreeNode* child : node->children) {
        deleteTree(child);
    }
    delete node;
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

void printTreeWithLimits(TreeNode* node, int level = 0) {
    if (!node) return;
    for (int i = 0; i < level; ++i) {
        cout << "  ";
    }
    cout << node->name;

    if (node->type == AND) cout << " [AND]";
    else if (node->type == OR) cout << " [OR]";

    if (node->mass != -1) {
        cout << " (Масса: " << node->mass << ")";
    }

    if (node->limitMass > 0) {
        cout << " (Ограничение: <= " << node->limitMass << ")";
    }

    cout << " [" << node->minMass << ", " << node->maxMass << "]" << endl;

    for (TreeNode* child : node->children) {
        printTreeWithLimits(child, level + 1);
    }
}

// простой формат
void printTreeWithoutLimits(TreeNode* node, int level = 0) {
    if (!node) return;
    for (int i = 0; i < level; ++i) {
        cout << ".";
    }
    cout << node->name;

    if (node->type == AND) cout << " AND 0";
    else if (node->type == OR) cout << " OR 0";
    else cout << " LEAF " << node->mass;

    cout << endl;

    for (TreeNode* child : node->children) {
        printTreeWithoutLimits(child, level + 1);
    }
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

    cout << "Введите максимально допустимую массу: ";
    cin >> maxMass;

    cout << "Исходное дерево с ограничениями:" << endl;
    calculateMassRanges(root);
    printTreeWithLimits(root);

    root = trimTree(root, maxMass);

    if (!root) {
        cout << "Дерево пустое после обрезки." << endl;
    }
    else {
        cout << "Обрезанное дерево с ограничениями:" << endl;
        printTreeWithLimits(root);
    }

    cout << "\nОбрезанное дерево без ограничений:" << endl;
    printTreeWithoutLimits(root);

    deleteTree(root);

    return 0;
}
