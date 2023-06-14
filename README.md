# BigInt_cpp

BigInt_cpp is a powerful Big Integer library written in C++. This library provides a convenient and efficient way to handle arbitrarily large integers, surpassing the limitations of standard built-in integer types. Whether you need to perform complex calculations or deal with extremely large numbers, BigInt_cpp has got you covered.

## Features

- Support for arbitrarily large integers: BigInt_cpp allows you to work with integers of any size, limited only by available memory.
- Efficient storage: The library stores integers in a little-endian format, optimizing memory usage by utilizing uint32_t variables to represent chunks of one million digits.
- Comprehensive operator overloading: BigInt_cpp provides a rich set of arithmetic and comparison operators, enabling intuitive and straightforward manipulation of large integers.
- Mathematical operations: You can perform various mathematical operations with BigInt_cpp, including addition, subtraction, multiplication, division, and modulus calculations.
- Unary operations: BigInt_cpp supports unary negation and unary plus, allowing you to change the sign of integers or retrieve the absolute value.
- Increment and decrement operators: The library includes both prefix and postfix increment and decrement operators, simplifying the process of modifying integer values.
- Assignment operators: BigInt_cpp offers compound assignment operators for addition, subtraction, multiplication, division, and modulus, allowing concise and efficient code.
- Comparison operators: You can compare BigInt_cpp objects using standard comparison operators, such as equality, inequality, less than, greater than, less than or equal to, and greater than or equal to.
- Stream output: BigInt_cpp supports streaming of integers to output streams, making it easy to display the values in a desired format.

## Usage

To use BigInt_cpp in your project, follow these steps:

1. Download the BigInt_cpp library files.
2. Include the `BigInt.h` header file in your C++ program.
3. Instantiate BigInt objects and perform operations as needed.

Here's a brief example showcasing the usage of BigInt_cpp:

```cpp
#include "BigInt.h"
#include <iostream>

int main() {
    BigInt a("12345678901234567890");
    BigInt b(9876543210987654321);
    
    BigInt sum = a + b;
    BigInt product = a * b;
    
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Product: " << product << std::endl;
    
    return 0;
}
```

## Contributions and Feedback

BigInt_cpp is an open-source project, and contributions are highly appreciated. If you have any suggestions for improvements or new features, please feel free to submit a pull request. Additionally, if you encounter any bugs or issues while using BigInt_cpp, please report them on the project's GitHub issue tracker.

Your feedback and suggestions are crucial for the continued development and enhancement of BigInt_cpp. Together, we can make BigInt_cpp an even better library for handling large integers.

Enjoy using BigInt_cpp and happy coding!

## License

BigInt_cpp is released under the [MIT License](LICENSE).
