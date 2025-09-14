#include <iostream>
#include "include/replacer.hpp"


int main(int argc, char** argv) {
    std::string text;
    int n;
    char old_value, new_value;
    
    std::getline(std::cin, text);
    std::cin >> n >> old_value >> new_value;

    text = replace_symbol(text, n, old_value, new_value);

    std::cout << text << "\n";
}