#pragma once

#include <interface_converter.hpp>
#include <token.hpp>

#include <gmock/gmock.h>

#include <cstdint>
#include <list>
#include <string>


class ConverterMock : public s2e2::IConverter
{
public:
    MOCK_METHOD2(addOperator, void(const std::string&, const uint_fast16_t));
    MOCK_CONST_METHOD1(convert, std::list<s2e2::Token>(const std::list<s2e2::Token>&));
};

using ConverterNiceMock = testing::NiceMock<ConverterMock>;
