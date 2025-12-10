#include <iostream>
#include <vector>
#include <algorithm>
#include "array.h"
using namespace std;

int main() {
    Array* arr = createArray(10);

    string line;
    while (getline(cin, line)) {
        pushBack(arr, line);
    }

    // Массив частот символов (0–255)
    int freq[256] = {0};

    // Подсчёт вхождений символов
    for (int i = 0; i < arr->size; i++) {
        string s = getAt(arr, i);
        for (unsigned char c : s) {
            if (c != ' ' && c != '\n') {
                freq[c]++;
            }
        }
    }

    // Собираем только те символы, которые встречались
    vector<unsigned char> symbols;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0)
            symbols.push_back((unsigned char)i);
    }

    // Сортируем символы по возрастанию кода
    sort(symbols.begin(), symbols.end());

    // Находим максимальную частоту
    int maxHeight = 0;
    for (unsigned char c : symbols)
        if (freq[c] > maxHeight)
            maxHeight = freq[c];

    // Печатаем гистограмму сверху вниз
    for (int h = maxHeight; h > 0; h--) {
        for (unsigned char c : symbols) {
            if (freq[c] >= h)
                cout << "#";
            else
                cout << " ";
        }
        cout << endl;
    }

    // Печатаем строку с символами
    for (unsigned char c : symbols)
        cout << c;
    cout << endl;

    freeArray(arr);
    return 0;
}
