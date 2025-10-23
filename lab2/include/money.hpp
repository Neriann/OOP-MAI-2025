#pragma once

#include <string>


class Money {
    friend Money operator+(const Money& lhs, const Money& rhs);

    friend Money operator-(const Money& lhs, const Money& rhs);

    friend std::ostream& operator<<(std::ostream& os, const Money& m);

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

    size_t GetLength() const;

    virtual ~Money() noexcept;

private:
    unsigned char* data;
    size_t size;

    void swap(Money& other) noexcept;
};

int32_t stoi(const unsigned char* data, size_t size);
