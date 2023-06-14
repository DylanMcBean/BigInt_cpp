#include "BigInt.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

/// @brief Default constructor
BigInt::BigInt()
    : numbers(1, 0), isNegative(false) {}  // initialize to 0 and positive

/// @brief Int constructor
/// @param num The int to initialize to
BigInt::BigInt(int64_t num) {
  if (num < 0) {
    isNegative = true;
    num = -num;
  }
  while (num > 0) {
    numbers.push_back(num % 1000000);
    num /= 1000000;
  }
  fixInvalid();
}

/// @brief Int constructur with sign
/// @param num The int to initialize to
/// @param isNegative Whether the number is negative
BigInt::BigInt(uint64_t num, bool isNegative) : isNegative(isNegative) {
  while (num > 0) {
    numbers.push_back(num % 1000000);
    num /= 1000000;
  }
  fixInvalid();
}

/// @brief String constructor
/// @param numStr The string to initialize to
BigInt::BigInt(const std::string& numStr) {
  if (numStr[0] == '-') {
    isNegative = true;
  }
  int position = 0;
  bool started = false;
  while (numStr[position] != '\0') {
    if (numStr[position] == '-') {
      position++;
      continue;
    } else if (numStr[position] == '0' && !started) {
      position++;
      continue;
    } else {
      started = true;
    }

    if (started) {
      int num = 0;
      for (int i = 0; i < 6; i++) {
        if (numStr[position] == '\0') {
          break;
        } else {
          if (numStr[position] >= '0' && numStr[position] <= '9') {
            num *= 10;
            num += numStr[position] - '0';
            position++;
          } else {
            throw std::invalid_argument(
                "Invalid character in number string at position " +
                std::to_string(position));
            break;
          }
        }
      }
      numbers.push_back(num);
    }
  }
  fixInvalid();
}

/// @note Operator overloads

/// @brief Addition operator
/// @param rhs The number to add to this number
/// @return The sum of this number and rhs
BigInt BigInt::operator+(const BigInt& rhs) const {
  // make sure to take into accoun these cases:
  // 1. positive, positive
  // 2. positive, negative
  // 3. negative, positive
  // 4. negative, negative

  // case 1
  if (!isNegative && !rhs.isNegative) {
    BigInt result;
    result.numbers.pop_back();
    int carry = 0;
    for (int i = 0; i < std::max(numbers.size(), rhs.numbers.size()); i++) {
      int sum = carry;
      if (i < numbers.size()) {
        sum += numbers[i];
      }
      if (i < rhs.numbers.size()) {
        sum += rhs.numbers[i];
      }
      result.numbers.push_back(sum % 1000000);
      carry = sum / 1000000;
    }
    if (carry > 0) {
      result.numbers.push_back(carry);
    }
    result.fixInvalid();
    return result;
  }

  // case 2
  if (!isNegative && rhs.isNegative) {
    return *this - (-rhs);
  }

  // case 3
  if (isNegative && !rhs.isNegative) {
    return rhs - -(*this);
  }

  // case 4
  if (isNegative && rhs.isNegative) {
    return -(-*this + -rhs);
  }

  // should never get here
  throw std::runtime_error("Addition operator reached end of function");
}

/// @brief Subtraction operator
/// @param rhs The number to subtract from this number
/// @return The difference of this number and rhs
BigInt BigInt::operator-(const BigInt& rhs) const {
  // make sure to take into accoun these cases:
  // 1. positive, positive
  // 2. positive, negative
  // 3. negative, positive
  // 4. negative, negative

  // case 1
  if (!isNegative && !rhs.isNegative) {
    if (*this < rhs) {
      return -(rhs - *this);
    }
    BigInt result;
    result.numbers.pop_back();
    int carry = 0;
    for (int i = 0; i < std::max(numbers.size(), rhs.numbers.size()); i++) {
      int diff = carry;
      if (i < numbers.size()) {
        diff += numbers[i];
      }
      if (i < rhs.numbers.size()) {
        diff -= rhs.numbers[i];
        if (diff < 0) {
          diff += 1000000;
          carry = -1;
        } else {
          carry = 0;
        }
      }
      result.numbers.push_back(diff);
    }
    result.fixInvalid();
    return result;
  }

  // case 2
  if (!isNegative && rhs.isNegative) {
    return *this + (-rhs);
  }

  // case 3
  if (isNegative && !rhs.isNegative) {
    return -(-*this + rhs);
  }

  // case 4
  if (isNegative && rhs.isNegative) {
    return -rhs - (-*this);
  }

  // should never get here
  throw std::runtime_error("Subtraction operator reached end of function");
}

/// @brief Multiplication operator
/// @param rhs The number to multiply this number by
/// @return The product of this number and rhs
BigInt BigInt::operator*(const BigInt& rhs) const {
  // case of multiplication by zero
  if (*this == BigInt(0) || rhs == BigInt(0)) {
    return BigInt(0);
  }

  // determine the sign of the result
  bool resultNegative = (this->isNegative != rhs.isNegative);

  BigInt result;
  result.numbers =
      std::vector<uint32_t>(this->numbers.size() + rhs.numbers.size(), 0);
  for (size_t i = 0; i < this->numbers.size(); i++) {
    uint64_t carry = 0;
    for (size_t j = 0; j < rhs.numbers.size(); j++) {
      uint64_t product = uint64_t(this->numbers[i]) * uint64_t(rhs.numbers[j]) +
                         carry + result.numbers[i + j];
      result.numbers[i + j] = product % 1000000;
      carry = product / 1000000;
    }
    result.numbers[i + rhs.numbers.size()] += carry;
  }

  // remove leading zeroes
  while (result.numbers.size() > 1 && result.numbers.back() == 0) {
    result.numbers.pop_back();
  }

  result.isNegative = resultNegative;

  result.fixInvalid();
  return result;
}

/// @brief Division operator
/// @param rhs The number to divide this number by
/// @return The quotient of this number divided by rhs
BigInt BigInt::operator/(const BigInt& rhs) const {
  // division by zero
  if (rhs == BigInt(0)) {
    throw std::invalid_argument("Division by zero");
  }

  // determine the sign of the result
  bool resultNegative = (this->isNegative != rhs.isNegative);

  // make both numbers positive for easier calculation
  BigInt dividend = this->isNegative ? -*this : *this;
  BigInt divisor = rhs.isNegative ? -rhs : rhs;

  // if the dividend is less than the divisor, the quotient is 0
  if (dividend < divisor) {
    return BigInt(0);
  }

  // make result BigInt
  BigInt quotient;
  quotient.numbers.clear();  // clear out the 0

  BigInt current = BigInt(0);  // current number in process of division
  int i = numbers.size() - 1;
  while (i >= 0) {
    current = current * BigInt(1000000) + BigInt(numbers[i]);
    --i;
    uint32_t start = 0, end = 1000000;
    while (start <= end) {
      uint32_t mid = (start + end) / 2;
      if (BigInt(mid) * divisor <= current) {
        start = mid + 1;
      } else {
        end = mid - 1;
      }
    }
    quotient.numbers.insert(quotient.numbers.begin(), end);
    current = current - BigInt(end) * divisor;
  }

  quotient.isNegative = resultNegative;
  quotient.fixInvalid();
  return quotient;
}

/// @brief Modulo operator
/// @param rhs The number to mod this number by
/// @return The remainder of this number divided by rhs
BigInt BigInt::operator%(const BigInt& rhs) const {
  // modulo by zero
  if (rhs == BigInt(0)) {
    throw std::invalid_argument("Modulo by zero");
  }

  BigInt result = *this - ((*this / rhs) * rhs);

  result.fixInvalid();
  return result;
}

/// @brief negation operator
/// @return The negation of this number
BigInt BigInt::operator-() const {
  BigInt result = *this;
  result.isNegative = !result.isNegative;
  return result;
}

/// @brief unary plus operator
/// @return The copy of this number
BigInt BigInt::operator+() const {
  BigInt result = *this;
  return result;
}

/// @brief Increment operator (prefix)
/// @return The reference to incremented number
BigInt& BigInt::operator++() {
  *this = *this + 1;
  return *this;
}

/// @brief Increment operator (postfix)
/// @return The copy of original number before increment
BigInt BigInt::operator++(int) {
  BigInt result = *this;
  *this = *this + 1;
  return result;
}

/// @brief Decrement operator (prefix)
/// @return The reference to decremented number
BigInt& BigInt::operator--() {
  *this = *this - 1;
  return *this;
}

/// @brief Decrement operator (postfix)
/// @return The copy of original number before decrement
BigInt BigInt::operator--(int) {
  BigInt result = *this;
  *this = *this - 1;
  return result;
}

/// @brief plus equals operator
/// @param rhs The number to add to this number
/// @return The reference to this number after addition
BigInt& BigInt::operator+=(const BigInt& rhs) {
  *this = *this + rhs;
  return *this;
}

/// @brief minus equals operator
/// @param rhs The number to subtract from this number
/// @return The reference to this number after subtraction
BigInt& BigInt::operator-=(const BigInt& rhs) {
  *this = *this - rhs;
  return *this;
}

/// @brief multiplication equals operator
/// @param rhs The number to multiply this number by
/// @return The reference to this number after multiplication
BigInt& BigInt::operator*=(const BigInt& rhs) {
  *this = *this * rhs;
  return *this;
}

/// @brief Division assignment operator
/// @param rhs The number to divide this number by
/// @return The reference to this number after division
BigInt& BigInt::operator/=(const BigInt& rhs) {
  *this = *this / rhs;
  return *this;
}

/// @brief Modulo equals operator
/// @param rhs The number to mod this number by
/// @return The reference to this number after modulo
BigInt& BigInt::operator%=(const BigInt& rhs) {
  *this = *this % rhs;
  return *this;
}

/// @brief comparison operator
/// @param rhs The number to compare to this number
/// @return True if this number is equal to rhs, false otherwise
bool BigInt::operator==(const BigInt& rhs) const {
  return isNegative == rhs.isNegative && numbers == rhs.numbers;
}

/// @brief not equal to operator
/// @param rhs The number to compare to this number
/// @return True if this number is not equal to rhs, false otherwise
bool BigInt::operator!=(const BigInt& rhs) const { return !(*this == rhs); }

/// @brief less than operator
/// @param rhs The number to compare to this number
/// @return True if this number is less than rhs, false otherwise
bool BigInt::operator<(const BigInt& rhs) const {
  if (isNegative != rhs.isNegative) {
    // One number is negative, the other is positive
    return isNegative;
  }

  // Compare digits from the most significant to the least significant
  const std::vector<uint32_t>& lhsNumbers = numbers;
  const std::vector<uint32_t>& rhsNumbers = rhs.numbers;

  if (lhsNumbers.size() != rhsNumbers.size()) {
    // Different number of digits
    return (lhsNumbers.size() < rhsNumbers.size()) != isNegative;
  }

  // Compare digits starting from the most significant
  for (int i = lhsNumbers.size() - 1; i >= 0; i--) {
    if (lhsNumbers[i] != rhsNumbers[i]) {
      // Found a difference in digits
      return (lhsNumbers[i] < rhsNumbers[i]) != isNegative;
    }
  }

  // The numbers are equal
  return false;
}

/// @brief greater than operator
/// @param rhs The number to compare to this number
/// @return True if this number is greater than rhs, false otherwise
bool BigInt::operator>(const BigInt& rhs) const { return rhs < *this; }

/// @brief less than or equal to operator
/// @param rhs The number to compare to this number
/// @return True if this number is less than or equal to rhs, false otherwise
bool BigInt::operator<=(const BigInt& rhs) const { return !(*this > rhs); }

/// @brief greater than or equal to operator
/// @param rhs The number to compare to this number
/// @return True if this number is greater than or equal to rhs, false otherwise
bool BigInt::operator>=(const BigInt& rhs) const { return !(*this < rhs); }

/// @note Helper Functions
std::ostream& operator<<(std::ostream& os, const BigInt& num) {
  // Print the sign if the number is negative
  if (num.isNegative) {
    os << "-";
  }

  // Print the digits in reverse order
  const std::vector<uint32_t>& numbers = num.getNumbers();
  for (int i = numbers.size() - 1; i >= 0; i--) {
    // Print each digit with leading zeros if necessary
    if (i != numbers.size() - 1) {
      os << std::setw(6) << std::setfill('0');
    }
    os << numbers[i];
  }

  return os;
}

const std::vector<uint32_t>& BigInt::getNumbers() const { return numbers; }

std::string BigInt::format() const {
  std::string formattedNum;

  // Convert the BigInt to a string representation
  std::ostringstream oss;
  oss << *this;
  std::string numStr = oss.str();

  // Handle the sign
  if (numStr[0] == '-') {
    formattedNum.push_back('-');
    numStr = numStr.substr(1);
  }

  // Calculate the number of digits
  int digits = numStr.size();

  // Insert commas
  int commaCount = (digits - 1) / 3;
  int firstGroupDigits = digits % 3 == 0 ? 3 : digits % 3;

  // Add the first group of digits
  formattedNum += numStr.substr(0, firstGroupDigits);

  // Add commas and remaining groups of digits
  for (int i = 0; i < commaCount; ++i) {
    formattedNum += ',';
    formattedNum += numStr.substr(firstGroupDigits + i * 3, 3);
  }

  return formattedNum;
}

int BigInt::numDigits() const {
  if (numbers.empty()) return 0;  // If the numbers vector is empty, return 0

  int numDigits = (numbers.size() - 1) *
                  6;  // Each uint32_t except the last one has up to 6 digits
  int lastNumberDigits = 1;  // Handle case when last number is 0
  if (numbers.back() != 0) {
    lastNumberDigits = static_cast<int>(std::log10(numbers.back())) + 1;
  }

  return numDigits + lastNumberDigits;
}

/// @brief Function to fix invalid numbers made by constructors
void BigInt::fixInvalid() {
  // fixing when -0, or when size of numbers is 0
  // in both cases, set to 0 and positive

  // start with -0
  if (numbers.size() == 1 && numbers[0] == 0 && isNegative) {
    isNegative = false;
  }

  // now check if size is 0
  if (numbers.size() == 0) {
    numbers.push_back(0);
    isNegative = false;
  }

  // remove leading zeros
  while (numbers.size() > 1 && numbers.back() == 0) {
    numbers.pop_back();
  }

  // handle case where all digits were zeros
  if (numbers.empty()) {
    numbers.push_back(0);
    isNegative = false;
  }
}
