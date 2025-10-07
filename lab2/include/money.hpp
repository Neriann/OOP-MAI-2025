#pragma once

#include <string>


class Money {
public:
    Money();

    Money(size_t n, unsigned char t = 0);
    Money(const Money& other);

    Money(const std::initializer_list<unsigned char>& t);

    Money(const std::string& other);

    Money& operator=(const Money& other);

    Money& operator=(Money&& other) noexcept;

    Money(Money&& other) noexcept;

    bool operator==(const Money& other) const;
    bool operator!=(const Money& other) const;
    bool operator>(const Money& other) const;
    bool operator>=(const Money& other) const;
    bool operator<(const Money& other) const;
    bool operator<=(const Money& other) const;

    const unsigned char* GetMoney() const;
    size_t GetLength() const;

    virtual ~Money() noexcept;

private:
    unsigned char* data;
    size_t size;

    void swap(Money& other) noexcept;
};

Money operator+(const Money& lhs, const Money& rhs);

Money operator-(const Money& lhs, const Money& rhs);

std::ostream& operator<<(std::ostream& os, const Money& money);

int32_t stoi(const unsigned char* data, size_t size);
