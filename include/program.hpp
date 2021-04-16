#pragma once
#include <iostream>
#include <mutex>

class Program {
public:

void start();
static Program& getInstance();

private:
static std::mutex singletonMutex; 
};