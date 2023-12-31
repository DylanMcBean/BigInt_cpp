#pragma once

#include <cstdint>
#include <string>
#include <vector>

/// @brief Class to represent arbitrarily large integers
///        This class is stored in little-endian format, meaning the most
///        significant digit is at the end of the vector

class BigInt {
  // Variables
 public:
 private:
  std::vector<uint32_t> numbers;  // each uint holds 1 million, more efficient
                                  // than 1 digit per uint8_t
  bool isNegative = false;        // start as positive

  // Methods
 public:
  // constructors
  BigInt();             // default constructor - value of 0
  BigInt(int64_t num);  // int constructor - value of num
  BigInt(uint64_t num,
         bool isNegative);  // int constructor, with sign - value of num
  BigInt(const std::string& numStr);  // string constructor - value of numStr

  // operators
  BigInt operator+(const BigInt& rhs) const;  // addition
  BigInt operator-(const BigInt& rhs) const;  // subtraction
  BigInt operator*(const BigInt& rhs) const;  // multiplication
  BigInt operator/(const BigInt& rhs) const;  // division
  BigInt operator%(const BigInt& rhs) const;  // modulus
  BigInt operator-() const;                   // unary negation
  BigInt operator+() const;                   // unary plus
  BigInt& operator++();                       // prefix increment
  BigInt operator++(int);                     // postfix increment
  BigInt& operator--();                       // prefix decrement
  BigInt operator--(int);                     // postfix decrement
  BigInt& operator+=(const BigInt& rhs);      // addition assignment
  BigInt& operator-=(const BigInt& rhs);      // subtraction assignment
  BigInt& operator*=(const BigInt& rhs);      // multiplication assignment
  BigInt& operator/=(const BigInt& rhs);      // division assignment
  BigInt& operator%=(const BigInt& rhs);      // modulus assignment

  // comparison operators
  bool operator==(const BigInt& rhs) const;  // equality
  bool operator!=(const BigInt& rhs) const;  // inequality
  bool operator<(const BigInt& rhs) const;   // less than
  bool operator>(const BigInt& rhs) const;   // greater than
  bool operator<=(const BigInt& rhs) const;  // less than or equal to
  bool operator>=(const BigInt& rhs) const;  // greater than or equal to

  // friend functions
  friend std::ostream& operator<<(std::ostream& os,
                                  const BigInt& num);  // output stream

  // helper functions
  std::string format() const;  // returns the number as a string
  int numDigits() const;       // returns the number of digits

 private:
  const std::vector<uint32_t>& getNumbers()
      const;          // returns the vector of numbers
  void fixInvalid();  // removes leading zeros and fixes sign
};
