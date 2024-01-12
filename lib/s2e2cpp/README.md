# s2e2.cpp

This library provides C++ implementation of Simple String Expression Evaluator a.k.a. `s2e2`. The Evaluator returns value of an input expression. Unlike commonly known mathematical expression evaluators this one treats all parts of the expression as strings and its output value is also a string.

For example:
* the value of the expression `A + B` is `AB`
* the value of `REPLACE("The cat is black", cat, dog)` is `The dog is black`

This is how one can use Evaluator to get value of some expression:
```cpp
#include <s2e2/evaluator.hpp>

s2e2::Evaluator evaluator;

evaluator.addStandardFunctions();
evaluator.addStandardOperators();

const std::string expression = "A + B";
const auto result = evaluator.evaluate(expression);
```

## Supported expressions

Supported expressions consist of the following tokens: string literals, operators (unary and binary), functions, predefined constants, round brackets for function's arguments denoting, commas for function's arguments separation and double quotes for characters escaping. 

The difference between a function and an operator is that a function is always followed by a pair of round brackets with a list of function's arguments (probably empty) in between, while an operator does not use brackets and, if it is a binary operator, sticks between its operands. Also operators can have different priorities a.k.a. precedence.

For example:
* this is a function of 2 arguments: `FUNC(Arg1, Arg2)`
* and this is a binary operator: `Arg1 OP Arg2`


## Constants

There is only one predefined constant - `NULL` - which corresponds to an absent value. It can be used to check if some sub-expression is evaluated into some result: `IF(SUBEXPR(Arg1, Arg2) == NULL, NULL, Value)`


## Functions

`s2e2` provides a small set of predefined functions. They are:

* Function `IF(Condition, Value1, Value2)`
  
  Returns `Value1` if `Condition` is true, and `Value2` otherwise. `Condition` must be a boolean value.

* Function `REPLACE(Source, Regex, Replacement)`

  Returns copy of `Source` with all matches of `Regex` replaced by `Replacement`. All three arguments are strings, `Regex` cannot be `NULL` or an empty string, `Replacement` cannot be `NULL`.

* Function `NOW()`

  Returns current UTC datetime. The result is of `std::tm` type.

* Function `ADD_DAYS(Datetime, NumberOfDays)`

  Adds days to the provided datetime. `Datetime` must be of `std::tm` type and not `NULL`. `NumberOfDays` is a string parsable into an any integer. The result is of `std::tm` type.

* Function `FORMAT_DATE(Datetime, Format)`

  Converts `Datetime` into a string according to `Format`. `Datetime` must be of `std::tm` type and not `NULL`. `Format` should be a string.
  

### Custom functions

It is possible to create and use any custom function. Here is a simple example:
```cpp
#include <s2e2/evaluator.hpp>
#include <s2e2/function.hpp>

#include <memory>
#include <unordered_set>


class CustomFunction final : public s2e2::Function
{
public:
    CustomFunction(const std::unordered_set<std::string>& set)
        : s2e2::Function("CONTAINS", 1)
        , set_(set)
    {
    }

    bool checkArguments() const override
    {
        return arguments_[0].has_value() &&
               arguments_[0].type() == typeid(std::string);
    }

    std::any result() const override
    {
        const auto* arg = std::any_cast<std::string>(&arguments_[0]);
        return {set_.count(*arg) > 0};
    }

private:
    const std::unordered_set<std::string> set_;
};

void customFunctionExample() {
    s2e2::Evaluator evaluator;

    evaluator.addStandardFunctions();
    evaluator.addStandardOperators();

    std::unordered_set<std::string> someSet = {"key1", "key2"};
    auto funcPtr = std::make_unique<CustomFunction>(someSet);

    evaluator.addFunction(std::move(funcPtr));

    const std::string expression = "IF(CONTAINS(key1), YES, NO)";
    const auto result = evaluator.evaluate(expression);
}
```

## Operators

As it was mentioned before, every operator has a priority. Within `s2e2` the range of priorities is from 1 to 999. A set of predefined operators is provided. They are:

* Binary operator `+`, priority `500`

  Concatenates two strings. Every operand can be either a `NULL` or a string. The result is a string.

* Binary operator `==`, priority `300`

  Compares two strings, including `NULL`. If both operands are `NULL` the result is `true`. The type of the result is boolean.

* Binary operator `!=`, priority `300`

  The same as `==`, but checks strings for inequality. 

* Binary operator `>`, priority `400`

  Compares two strings lexicographically. None of the operands can be `NULL`. The result is a boolean.

* Binary operator `>=`, priority `400`

  Compares two string lexicographically as well. Both operands must be not `NULL` or both must be `NULL`. In the latter case the result is `true`.

* Binary operator `<`, priority `400`

  Same as `>`, but checks if first operand is less that the second one.

* Binary operator `<=`, priority `400`

  Same as `>=`, but checks if first operand is less or equal that the second one.

* Binary operator `&&`, priority `200`

  Computes logical conjunction of two boolean values. Both arguments are boolean, not `NULL` value. The result is a boolean.

* Binary operator `||`, priority `100`

  Computes logical disjunction of two boolean values. Both arguments are boolean, not `NULL` value. The result is a boolean.

* Unary operator `!`, priority `600`

  Negates boolean value. Operand cannot be `NULL`. The result is a boolean.


### Custom operators

It is possible to create and use any custom operator. Here is a simple example:
```cpp
#include <s2e2/evaluator.hpp>
#include <s2e2/operator.hpp>

#include <algorithm>
#include <memory>
#include <unordered_set>


class CustomOperator final : public s2e2::Operator
{
public:
    CustomOperator()
        : s2e2::Operator("~", 600, 1)
    {
    }

    bool checkArguments() const override
    {
        return arguments_[0].has_value() &&
               arguments_[0].type() == typeid(std::string);
    }

    std::any result() const override
    {
        auto result = std::any_cast<std::string>(arguments_[0]);
        std::reverse(result.begin(), result.end());

        return {std::move(result)};
    }
};

void customOperatorExample() {
    s2e2::Evaluator evaluator;

    evaluator.addStandardFunctions();
    evaluator.addStandardOperators();

    evaluator.addOperator(std::make_unique<CustomOperator>());

    const std::string expression = "~Foo";
    const auto result = evaluator.evaluate(expression);
}
```

## Getting Started

### Prerequisites

To compile this project one would need:
* C++ compiler with C++17 support (tested ones are GCC, Clang and MSVC)
* CMake >= 3.5.0

`gtest` and `gmock` libraries are used for unit testing and are included as git submodules.


### Compile library

On Linux this will compile the static library, the unit tests executable and install everything into `./build/output/<Build Type>` directory:
```
./build.sh
```
On Windows do the following:
```
cd ./build
cmake ..
```
and open 's2e2.sln' with Visual Studio.


### Run tests

On Linux:
```
./run_tests.sh
```
On Windows:
```
./build/output/<Build Type>/test/s2e2_tests.exe
```


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
