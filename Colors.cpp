// Colors.cpp
#include "Colors.h"
#include <iostream>

void GameColors::printColored(const std::string& text, const std::string& color) {
    std::cout << color << text << RESET << std::endl;
}