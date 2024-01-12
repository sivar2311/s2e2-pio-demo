#include "token.hpp"


s2e2::Token::Token(const TokenType tokenType, std::string tokenValue)
    : type{tokenType}
    , value{std::move(tokenValue)}
{
}

bool s2e2::Token::operator==(const Token& another) const
{
    return type == another.type &&
           value == another.value;
}
