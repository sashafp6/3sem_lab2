#ifndef STACK_STRING_H
#define STACK_STRING_H

#include <string>

struct StackNodeStr {
    std::string value;
    StackNodeStr* next;
};

// Сам стек
struct StackStr {
    StackNodeStr* top;
};

// Создание стека
StackStr* createStackStr();

// Добавление (push)
void pushStr(StackStr* stack, const std::string& value);

// Удаление (pop)
void popStr(StackStr* stack);

// Чтение (peek)
std::string peekStr(StackStr* stack);

// Печать
void printStackStr(StackStr* stack);

// Очистка памяти
void freeStackStr(StackStr* stack);

// Сохранение стека в файл
void saveStackToFile(StackStr* stack, const std::string& filename);

// Загрузка стека из файла
void loadStackFromFile(StackStr* stack, const std::string& filename);

#endif
