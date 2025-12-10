#include "hashtable.h"
#include <iostream>
#include <fstream>
#include <string>

void printHelp() {
    std::cout << "Использование:\n"
              << "./set --file <путь_к_файлу> --query <операция> <значение> [--mode <double|cuckoo|hybrid>]\n\n"
              << "Операции:\n"
              << "  SETADD <элемент>   - добавить элемент во множество\n"
              << "  SETDEL <элемент>   - удалить элемент из множества\n"
              << "  SET_AT <элемент>   - проверить наличие элемента\n"
              << "Режимы:\n"
              << "  double   - только двойное хэширование\n"
              << "  cuckoo   - только кукушкино хэширование\n"
              << "  hybrid   - сначала кукушка, при коллизии двойное хэширование\n";
}

// Гибридные операции

void insertHybrid(HashTable& doubleHT, CuckooHashTable& cuckooHT, const std::string& key) {
    try {
        insertCuckoo(cuckooHT, key, key);  // Используем ключ и как значение
    } catch (...) {
        insertDouble(doubleHT, key, key); 
    }
}

bool searchHybrid(const HashTable& doubleHT, const CuckooHashTable& cuckooHT, const std::string& key) {
    return !searchCuckoo(cuckooHT, key).empty() || !searchDouble(doubleHT, key).empty();
}

void removeHybrid(HashTable& doubleHT, CuckooHashTable& cuckooHT, const std::string& key) {
    if (!searchCuckoo(cuckooHT, key).empty()) {
        removeCuckoo(cuckooHT, key);
    } else {
        removeDouble(doubleHT, key);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printHelp();
        return 1;
    }

    std::string filePath, queryType, element, mode = "double";

    // Разбор аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) filePath = argv[++i];
        else if (arg == "--query" && i + 2 < argc) {
            queryType = argv[++i];
            element = argv[++i];
        }
        else if (arg == "--mode" && i + 1 < argc) mode = argv[++i];
    }

    if (filePath.empty() || queryType.empty() || element.empty()) {
        printHelp();
        return 1;
    }

    // Инициализация таблиц
    HashTable doubleHT;
    CuckooHashTable cuckooHT;
    initHashTableDouble(doubleHT, 50);
    initCuckoo(cuckooHT, 50);

    // Загружаем данные из файла
    if (mode == "double") loadFromFileDouble(doubleHT, filePath);
    else if (mode == "cuckoo") loadFromFileCuckoo(cuckooHT, filePath);
    else {  // hybrid
        loadFromFileCuckoo(cuckooHT, filePath);
        loadFromFileDouble(doubleHT, filePath);
    }

    // Обработка операций
    if (queryType == "SETADD") {
        if (mode == "double") insertDouble(doubleHT, element, element);  // ключ = значение
        else if (mode == "cuckoo") insertCuckoo(cuckooHT, element, element);  // ключ = значение
        else insertHybrid(doubleHT, cuckooHT, element);
        std::cout << "Добавлен элемент: " << element << "\n";
    }
    else if (queryType == "SETDEL") {
        if (mode == "double") removeDouble(doubleHT, element);
        else if (mode == "cuckoo") removeCuckoo(cuckooHT, element);
        else removeHybrid(doubleHT, cuckooHT, element);
        std::cout << "Удален элемент: " << element << "\n";
    }
    else if (queryType == "SET_AT") {
        bool found = false;
        if (mode == "double") found = !searchDouble(doubleHT, element).empty();
        else if (mode == "cuckoo") found = !searchCuckoo(cuckooHT, element).empty();
        else found = searchHybrid(doubleHT, cuckooHT, element);

        if (found) std::cout << "Элемент \"" << element << "\" присутствует\n";
        else std::cout << "Элемент \"" << element << "\" отсутствует\n";
    }
    else {
        std::cout << "Неизвестная операция\n";
        printHelp();
        return 1;
    }

    // Сохраняем изменения обратно в файл
    if (mode == "double") saveToFileDouble(doubleHT, filePath);
    else if (mode == "cuckoo")
    saveToFileCuckoo(cuckooHT, filePath);
    else { 
        saveToFileDouble(doubleHT, filePath);
        saveToFileCuckoo(cuckooHT, filePath);
    }

    return 0;
}
