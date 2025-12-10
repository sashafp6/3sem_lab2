#include <iostream>
#include <vector>
#include <algorithm>

struct Node {
    int value;
    Node* left;
    Node* right;
    Node(int v) : value(v), left(nullptr), right(nullptr) {}
};

// Вставка в дерево поиска
void insert(Node*& root, int val) {
    if (!root) {
        root = new Node(val);
        return;
    }
    if (val < root->value)
        insert(root->left, val);
    else if (val > root->value)
        insert(root->right, val);
}

// Обход дерева и поиск вершин с двумя детьми
void findTwoChildren(Node* root, std::vector<int>& result) {
    if (!root) return;
    if (root->left && root->right)
        result.push_back(root->value);
    findTwoChildren(root->left, result);
    findTwoChildren(root->right, result);
}

// Очистка памяти
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    Node* root = nullptr;
    int x;

    // Читаем числа до нуля
    while (std::cin >> x && x != 0) {
        insert(root, x);
    }

    std::vector<int> nodes;
    findTwoChildren(root, nodes);

    std::sort(nodes.begin(), nodes.end()); // сортировка по возрастанию

    for (int v : nodes)
        std::cout << v << " ";
    std::cout << "\n";

    freeTree(root);
    return 0;
}
