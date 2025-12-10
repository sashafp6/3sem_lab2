
#include "lfu.h"
#include <iostream>
#include <sstream>
#include <vector>

int main() {
    int capacity, numRequests;
    
    std::cout << "Введите емкость кэша: ";
    std::cin >> capacity;
    std::cout << "Введите количество запросов: ";
    std::cin >> numRequests;
    std::cin.ignore(); // очищаем буфер
    
    LFUCache cache(capacity);
    std::vector<std::string> requests;
    
    std::cout << "Введите запросы (SET x y или GET x):" << std::endl;
    
    // Читаем все запросы
    for (int i = 0; i < numRequests; i++) {
        std::string request;
        std::getline(std::cin, request);
        requests.push_back(request);
    }
    
    // Обрабатываем запросы
    for (const auto& request : requests) {
        if (request.find("SET ") == 0) {
            std::istringstream iss(request.substr(4));
            std::string key, value;
            if (iss >> key >> value) {
                cache.SET(key, value);
            }
        }
        else if (request.find("GET ") == 0) {
            std::string key = request.substr(4);
            std::string result = cache.GET(key);
            std::cout << result << std::endl;
        }
    }
    
    std::cout << "Состояние кэша:" << std::endl;
    cache.printCache();
    
    return 0;
}
