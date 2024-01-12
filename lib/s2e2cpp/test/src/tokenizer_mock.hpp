#pragma once

#include <interface_tokenizer.hpp>
#include <token.hpp>

#include <gmock/gmock.h>

#include <list>
#include <string>


class TokenizerMock : public s2e2::ITokenizer
{
public:
    MOCK_METHOD1(addFunction, void(const std::string&));
    MOCK_METHOD1(addOperator, void(const std::string&));
    MOCK_CONST_METHOD1(tokenize, std::list<s2e2::Token>(const std::string&));
};

using TokenizerNiceMock = testing::NiceMock<TokenizerMock>;
