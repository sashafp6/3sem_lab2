#include "hashtable.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

int main() {
    int N;
    std::cin >> N;

    // Используем хэш-таблицу для хранения строк вида "ai bi"
    HashTable ht;
    initHashTableDouble(ht, 20);

    std::vector<std::pair<int, int>> turtles(N);
    for (int i = 0; i < N; i++) {
        int a, b;
        std::cin >> a >> b;
        turtles[i] = {a, b};

        std::ostringstream oss;
        oss << a << " " << b;
        insertDouble(ht, oss.str(), oss.str());
    }

    int max_truth = 0;

    // Перебираем возможное количество "правдивых" черепах
    for (int possible = 0; possible <= N; possible++) {
        int count = 0;
        for (int i = 0; i < N; i++) {
            int a = turtles[i].first;
            int b = turtles[i].second;
            if (a + b == N - 1) count++;
        }
        max_truth = std::max(max_truth, count);
    }

    std::cout << max_truth << std::endl;

    std::cout << "\nСодержимое хэш-таблицы:\n";
    printDouble(ht);

    return 0;
}
