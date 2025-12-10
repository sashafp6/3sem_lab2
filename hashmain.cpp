#include "hashtable.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// Функция для разделения строки на токены по пробелу
std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(str);
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Использование:\n";
        std::cerr << "  ./dbms --file file.data --query 'КОМАНДА ПАРАМЕТРЫ'\n";
        std::cerr << "Примеры:\n";
        std::cerr << "  ./dbms --file file.data --query 'HINSERT_D Рэкс Овчарка'\n";
        std::cerr << "  ./dbms --file file.data --query 'HPRINT_D'\n";
        std::cerr << "  ./dbms --file file.data --query 'HSEARCH_C Тоби'\n";
        return 1;
    }

    std::string filename, query;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) filename = argv[++i];
        else if (arg == "--query" && i + 1 < argc) query = argv[++i];
    }

    if (filename.empty() || query.empty()) {
        std::cerr << "Ошибка: нужно указать --file и --query.\n";
        return 1;
    }

    std::vector<std::string> tokens = split(query);
    if (tokens.empty()) {
        std::cerr << "Ошибка: пустой запрос.\n";
        return 1;
    }

    std::string command = tokens[0];

    // ------------------------------
    // ДВОЙНОЕ ХЭШИРОВАНИЕ
    // ------------------------------
    if (command == "HINSERT_D" || command == "HSEARCH_D" ||
        command == "HDEL_D"    || command == "HPRINT_D" ||
        command == "HRESET_D")
    {
        HashTable ht;
        initHashTableDouble(ht, 10);
        loadFromFileDouble(ht, filename);

        if (command == "HINSERT_D") {
            if (tokens.size() < 3) {
                std::cerr << "Использование: HINSERT_D ключ значение\n";
                return 1;
            }
            std::string key = tokens[1];
            std::string value = tokens[2];
            insertDouble(ht, key, value);
            saveToFileDouble(ht, filename);
            
        }
        else if (command == "HSEARCH_D") {
            if (tokens.size() < 2) {
                std::cerr << "Использование: HSEARCH_D ключ\n";
                return 1;
            }
            std::string key = tokens[1];
            std::string value = searchDouble(ht, key);
            if (!value.empty())
                std::cout << "Элемент найден: " << key << " : " << value << "\n";
            else
                std::cout << "Элемент не найден: " << key << "\n";
        }
        else if (command == "HDEL_D") {
            if (tokens.size() < 2) {
                std::cerr << "Использование: HDEL_D ключ\n";
                return 1;
            }
            std::string key = tokens[1];
            removeDouble(ht, key);
            saveToFileDouble(ht, filename);
            std::cout << "Удалено: " << key << "\n";
        }
        else if (command == "HPRINT_D") {
            printDouble(ht);
        }
        else if (command == "HRESET_D") {
            initHashTableDouble(ht, 10);
            saveToFileDouble(ht, filename);
            std::cout << "Таблица (двойное хэширование) очищена.\n";
        }
    }

    // ------------------------------
    // КУКУШКИНО ХЭШИРОВАНИЕ
    // ------------------------------
    else if (command == "HINSERT_C" || command == "HSEARCH_C" ||
             command == "HDEL_C"    || command == "HPRINT_C" ||
             command == "HRESET_C")
    {
        CuckooHashTable ht;
        initCuckoo(ht, 10);
        loadFromFileCuckoo(ht, filename);

        if (command == "HINSERT_C") {
            if (tokens.size() < 3) {
                std::cerr << "Использование: HINSERT_C ключ значение\n";
                return 1;
            }
            std::string key = tokens[1];
            std::string value = tokens[2];
	    insertCuckoo(ht, key, value);
            saveToFileCuckoo(ht, filename);
            std::cout << "Добавлено: " << key << " : " << value << "\n";
        }
        else if (command == "HSEARCH_C") {
            if (tokens.size() < 2) {
                std::cerr << "Использование: HSEARCH_C ключ\n";
                return 1;
            }
            std::string key = tokens[1];
            std::string value = searchCuckoo(ht, key);
            if (!value.empty())
                std::cout << "Элемент найден: " << key << " : " << value << "\n";
            else
                std::cout << "Элемент не найден: " << key << "\n";
        }
        else if (command == "HDEL_C") {
            if (tokens.size() < 2) {
                std::cerr << "Использование: HDEL_C ключ\n";
                return 1;
            }
            std::string key = tokens[1];
            removeCuckoo(ht, key);
            saveToFileCuckoo(ht, filename);
            std::cout << "Удалено: " << key << "\n";
        }
        else if (command == "HPRINT_C") {
            printCuckoo(ht);
        }
        else if (command == "HRESET_C") {
            initCuckoo(ht, 10);
            saveToFileCuckoo(ht, filename);
            std::cout << "Таблица (кукушкина) очищена.\n";
        }
    }

    else {
        std::cerr << "Неизвестная команда: " << command << "\n";
        std::cerr << "Доступные команды:\n";
        std::cerr << "  --- Двойное хэширование ---\n";
        std::cerr << "    HINSERT_D ключ значение  — добавить элемент\n";
        std::cerr << "    HSEARCH_D ключ           — поиск элемента\n";
        std::cerr << "    HDEL_D ключ              — удалить элемент\n";
        std::cerr << "    HPRINT_D                 — вывести таблицу\n";
        std::cerr << "    HRESET_D                 — очистить таблицу\n\n";
        std::cerr << "  --- Кукушкино хэширование ---\n";
        std::cerr << "    HINSERT_C ключ значение  — добавить элемент\n";
        std::cerr << "    HSEARCH_C ключ           — поиск элемента\n";
        std::cerr << "    HDEL_C ключ              — удалить элемент\n";
        std::cerr << "    HPRINT_C                 — вывести таблицу\n";
        std::cerr << "    HRESET_C                 — очистить таблицу\n";
    }

    return 0;
}
