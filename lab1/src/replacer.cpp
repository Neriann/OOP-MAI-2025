#include "../include/replacer.hpp"

std::string replace_symbol(std::string text, int n, char old_value, char new_value) {
    if (n <= 0) {
        return text;
    }
    size_t sz = text.size();
    size_t cnt = 0;
    for (size_t i = 0; i < sz; ++i) {
        if (text[i] == old_value) {
            if (++cnt % n == 0) {
                text[i] = new_value;
            }
        }
    }
    return text;
}