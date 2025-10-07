#include "money.hpp"

#include <algorithm>
#include <stdexcept>
#include <sstream>

Money::Money(): data(nullptr), size(0) {}

Money::Money(size_t n, unsigned char t) {
    if (!isdigit(t)) {
        throw std::invalid_argument("Money::Money(): t must be an integer");
    }
    size = n;
    data = new unsigned char[size];

    for (size_t i = 0; i < size; i++) {
        data[i] = t;
    }
}

Money::Money(const Money& other) {
    size = other.size;
    data = new unsigned char[size];
    for (size_t i = 0; i < size; i++) {
        data[i] = other.data[i];
    }
}

void Money::swap(Money& other) noexcept {
    std::swap(size, other.size);
    std::swap(data, other.data);
}

Money& Money::operator=(const Money& other) {
    if (this == &other) {
        return *this;
    }
    // idiom copy and swap
    Money tmp = other;
    swap(tmp);
    return *this;
}

Money& Money::operator=(Money&& other) noexcept {
    size = other.size;
    data = other.data;
    other.size = 0;
    other.data = nullptr;
    return *this;
}

Money::Money(const std::initializer_list<unsigned char>& other) {
    size_t i = 0;
    size = other.size();
    data = new unsigned char[size];
    for (unsigned char ch : other) {
        if (!isdigit(ch)) {
            delete[] data;
            throw std::invalid_argument("Money::Money(): std::initializer_list must be a number");
        }
        data[i++] = ch;
    }
}


Money::Money(const std::string& other) {
    if (other.empty()) throw std::invalid_argument("Empty string");

    size = other.size();
    data = new unsigned char[size];
    for (size_t i = 0; i < size; i++) {
        if (!isdigit(other[i]) && i != 0 && other[i] != '-') {
            delete[] data;
            throw std::invalid_argument("Money::Money(): std::string must be a number");
        }
        data[i] = static_cast<unsigned char>(other[i]);
    }
}

Money::Money(Money&& other) noexcept {
    data = other.data;
    size = other.size;
    other.data = nullptr;
    other.size = 0;
}

bool Money::operator==(const Money& other) const {
    return stoi(data, size) == stoi(other.data, other.size);
}

bool Money::operator!=(const Money& other) const {
    return !(*this == other);
}


bool Money::operator>(const Money& other) const {
    return stoi(data, size) > stoi(other.data, other.size);
}

bool Money::operator>=(const Money& other) const {
    return stoi(data, size) >= stoi(other.data, other.size);
}

bool Money::operator<(const Money& other) const {
    return !(*this >= other);
}

bool Money::operator<=(const Money& other) const {
    return !(*this > other);
}

const unsigned char* Money::GetMoney() const {
    return data;
}

size_t Money::GetLength() const {
    return size;
}


Money::~Money() noexcept {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
    size = 0;
}

Money operator+(const Money& lhs, const Money& rhs) {
    std::string result;

    const unsigned char* lhs_data = lhs.GetMoney();
    const unsigned char* rhs_data = rhs.GetMoney();

    size_t lhs_sz = lhs.GetLength();
    size_t rhs_sz = rhs.GetLength();

    int32_t i = lhs_sz - 1;
    int32_t j = rhs_sz - 1;

    int32_t rem = 0;
    while (i >= 0 || j >= 0 || rem) {
        int32_t sum = rem;
        if (i >= 0) sum += lhs_data[i--] - '0';
        if (j >= 0) sum += rhs_data[j--] - '0';

        result.push_back(sum % 10 + '0');
        rem = sum / 10;
    }
    reverse(result.begin(), result.end());
    return {result};
}

Money operator-(const Money& lhs, const Money& rhs) {
    std::string result;

    const unsigned char* lhs_data = lhs.GetMoney();
    const unsigned char* rhs_data = rhs.GetMoney();

    size_t lhs_sz = lhs.GetLength();
    size_t rhs_sz = rhs.GetLength();

    int32_t i = lhs_sz - 1;
    int32_t j = rhs_sz - 1;

    int32_t rem = 0;
    while (i >= 0 || j >= 0) {
        int32_t sum = rem;
        rem = 0;
        if (i >= 0)
            sum += lhs_data[i--] - '0';
        if (j >= 0)
            sum -= rhs_data[j--] - '0';

        if (sum < 0)
            rem = -1;
        result.push_back((sum + 10) % 10 + '0');
    }
    while (result.size() - 1 && result.back() == '0') {
        result.pop_back();
    }
    if (rem < 0) {
        result.push_back('-');
    }
    reverse(result.begin(), result.end());
    return {result};
}


std::ostream& operator<<(std::ostream& os, const Money& m) {
    size_t sz = m.GetLength();
    const unsigned char* data = m.GetMoney();
    for (size_t i = 0; i < sz; ++i) {
        os << data[i];
    }
    return os;
}

int32_t stoi(const unsigned char* data, size_t size) {
    int32_t result = 0;
    for (size_t i = 0; i < size; ++i) {
        result = result * 10 + data[i] - '0';
    }
    return result;
}
