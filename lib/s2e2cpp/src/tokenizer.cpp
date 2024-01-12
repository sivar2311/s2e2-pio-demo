#include "tokenizer.hpp"

#include <s2e2/error.hpp>

#include <algorithm> 
#include <cctype>


namespace // anonymous
{
    // Some special symbols.
    constexpr char COMMA = ',';
    constexpr char LEFT_BRACKET = '(';
    constexpr char RIGHT_BRACKET = ')';
    constexpr char QUOTE = '"';
    constexpr char BACKSLASH = '\\';

    /**
     * @brief Remove all leading whitespace symbols from the string.
     * @param[in, out] str - String to remove white spaces from.
     */
    void leftTrim(std::string& str)
    {
        str.erase(
            str.begin(),
            std::find_if(str.begin(), str.end(), [](int c) { return !std::isblank(c); })
        );
    }

    /**
     * @brief Remove all trailing whitespace symbols from the string.
     * @param[in, out] str - String to remove white spaces from.
     */
    void rightTrim(std::string& str)
    {
        str.erase(
            std::find_if(str.rbegin(), str.rend(), [](int c) { return !std::isblank(c); }).base(),
            str.end()
        );
    }

    /**
     * @brief Remove all leading and trailing whitespace symbols from the string.
     * @param[in, out] str - String to remove white spaces from.
     */
    void trim(std::string& str)
    {
        leftTrim(str);
        rightTrim(str);
    }

    /**
     * @brief Replace all EXPRESSION tokens with ATOM ones.
     * @param[in, out] tokens - List of tokens.
     */
    void convertExpressionsIntoAtoms(std::list<s2e2::Token>& tokens)
    {
        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            if (it->type != s2e2::TokenType::EXPRESSION)
            {
                continue;
            }

            auto itCopy = it;
            auto tokenValue = it->value;
            it = tokens.emplace(it, s2e2::TokenType::ATOM, tokenValue);

            tokens.erase(itCopy);
        }
    }

    /**
     * @class ExpressionSplitter
     * @brief Class splits expression into raw tokens.
     */
    class ExpressionSplitter
    {
    public:
        /**
         * @brief Construct the splitter object.
         * @param[in] typeByValue - External function to get token's type by its value.
         */
        ExpressionSplitter(std::function<s2e2::TokenType(const std::string&)> typeByValue)
            : typeByValue_{typeByValue}
        {
        }

        /**
         * @brief Split expression into tokens by spaces and brackets.
         * @param[in] expression - Input expression.
         * @throws Error if expression contains unknown symbol.
         */
        const std::list<s2e2::Token>& splitIntoTokens(const std::string& expression)
        {
            for (char symbol : expression)
            {
                processSymbol(symbol);
            }
            flushToken();
            return tokens_;
        }

    private:
        /**
         * @brief Process one symbol of the input expression.
         * @param[in] symbol - Symbol of the expression.
         * @throws Error if expression contains unknown symbol.
         */
        void processSymbol(const char symbol)
        {
            switch (symbol)
            {
            case COMMA:
            case LEFT_BRACKET:
            case RIGHT_BRACKET:
                processSpecialSymbol(symbol);
                break;

            case QUOTE:
                processQuoteSymbol(symbol);
                break;

            default:
                processCommonSymbol(symbol);
                break;
            }
        }

        /**
         * @brief Process one special symbol of the input expression.
         * @param[in] symbol - Special symbol of expression.
         * @throws Error if expression contains unknown symbol.
         */
        void processSpecialSymbol(const char symbol)
        {
            if (insideQuotes_)
            {
                addSymbolToToken(symbol);
                return;
            }
            
            flushToken();

            switch (symbol)
            {
            case COMMA:
                addFoundToken(s2e2::TokenType::COMMA, COMMA);
                break;

            case LEFT_BRACKET:
                addFoundToken(s2e2::TokenType::LEFT_BRACKET, LEFT_BRACKET);
                break;

            case RIGHT_BRACKET:
                addFoundToken(s2e2::TokenType::RIGHT_BRACKET, RIGHT_BRACKET);
                break;

            default:
                throw s2e2::Error(std::string{"Tokenizer: unexpected special symbol "} + symbol);
            }
        }

        /**
         * @brief Process one quote symbol of the input expression.
         * @param[in] symbol - Quote symbol of expression.
         */
        void processQuoteSymbol(const char symbol)
        {
            if (insideQuotes_ && isEscaped())
            {
                addSymbolToToken(symbol);
                return;
            }
            
            flushToken();
            insideQuotes_ = !insideQuotes_;
        }

        /**
         * @brief Process one common symbol of the input expression.
         * @param[in] symbol - Common symbol of expression.
         */
        void processCommonSymbol(const char symbol)
        {
            if (insideQuotes_ || std::isblank(symbol) == 0)
            {
                addSymbolToToken(symbol);
                return;
            }
            flushToken();
        }

        /**
         * @brief Add symbol to currently parsed token.
         * @param[in] symbol - Symbol to add.
         */
        void addSymbolToToken(const char symbol)
        {
            if (symbol == QUOTE)
            {
                currentToken_.back() = symbol;
            }
            else
            {
                currentToken_ += symbol;
            }
        }

        /**
         * @brief Add current token if there is such to the list of found tokens.
         */
        void flushToken()
        {
            if (!insideQuotes_)
            {
                trim(currentToken_);
            }

            if (!currentToken_.empty() || insideQuotes_)
            {
                addFoundToken(tokenTypeByValue(currentToken_), currentToken_);
            }
            currentToken_.clear();
        }

        /**
         * @brief Add token to the list of found tokens.
         * @tparam T - Type of token's value.
         * @param[in] type - Token's type.
         * @param[in] value - Token's value.
         */
        template <class T>
        void addFoundToken(s2e2::TokenType type, const T& value)
        {
            std::string valueCopy;
            valueCopy += value;

            tokens_.emplace_back(type, std::move(valueCopy));
        }

        /**
         * @brief Check if current symbol is escaped, i.e. preceded by a backslash.
         * @returns true is symbol is escaped, false otherwise.
         */
        bool isEscaped() const
        {
            return !currentToken_.empty() &&
                   currentToken_.back() == BACKSLASH;
        }

        /**
         * @brief Get token type by its value and current state of the splitter.
         * @param[in] value - Token's value.
         * @returns Token's type.
         */
        s2e2::TokenType tokenTypeByValue(const std::string& value)
        {
            if (insideQuotes_)
            {
                return s2e2::TokenType::ATOM;
            }
            return typeByValue_(value);
        }

    private:
        /// @brief Flag of "inside quotes" state. If set it means that current symbol belongs to an ATOM.
        bool insideQuotes_ = false;

        /// @brief Currently parsed token value.
        std::string currentToken_;

        /// @brief List of found tokens.
        std::list<s2e2::Token> tokens_;

        /// @brief External function to get token's type by its value.
        std::function<s2e2::TokenType(const std::string&)> typeByValue_;
    };

} // namespace anonymous 

void s2e2::Tokenizer::addFunction(const std::string& functionName)
{
    checkUniqueness(functionName);

    functions_.insert(functionName);
}

void s2e2::Tokenizer::addOperator(const std::string& operatorName)
{
    checkUniqueness(operatorName);

    operators_.insert(operatorName);

    const auto emplaceResult = operatorsByLength_.try_emplace(operatorName.size(), std::unordered_set<std::string>{});
    emplaceResult.first->second.insert(operatorName);
}

std::list<s2e2::Token> s2e2::Tokenizer::tokenize(const std::string& expression) const
{
    ExpressionSplitter splitter(std::bind(&Tokenizer::tokenTypeByValue, this, std::placeholders::_1));
    const auto& rawTokens = splitter.splitIntoTokens(expression);

    auto refinedTokens = splitTokensByOperators(rawTokens);
    convertExpressionsIntoAtoms(refinedTokens);
    return refinedTokens;
}

void s2e2::Tokenizer::checkUniqueness(const std::string& entityName) const
{
    if (functions_.count(entityName) != 0)
    {
        throw Error("Tokenizer: function " + entityName + " is already added");
    }
    if (operators_.count(entityName) != 0)
    {
        throw Error("Tokenizer: operator " + entityName + " is already added");
    }
}

std::list<s2e2::Token> s2e2::Tokenizer::splitTokensByOperators(const std::list<Token>& tokens) const
{
    auto result = tokens;

    for (const auto& pair : operatorsByLength_)
    {
        const auto& operatorsOfTheSameLength = pair.second;
        for (const auto& operatorName : operatorsOfTheSameLength)
        {
            splitTokensBySingleOperator(result, operatorName);
        }
    }

    return result;
}

void s2e2::Tokenizer::splitTokensBySingleOperator(std::list<Token>& tokens, const std::string& operatorName) const
{
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (it->type != TokenType::EXPRESSION)
        {
            continue;
        }
        splitSingleTokenBySingleOperator(tokens, it, operatorName);
    }
}

void s2e2::Tokenizer::splitSingleTokenBySingleOperator(std::list<Token>& tokens,
                                                       std::list<Token>::iterator& tokenIterator,
                                                       const std::string& operatorName) const
{
    std::list<Token> newTokens;
    const auto& token = tokenIterator->value;
    size_t start = 0;

    while (start < token.size())
    {
        const size_t end = token.find(operatorName, start);
        if (end == std::string::npos)
        {
            if (!newTokens.empty())
            {
                auto newTokenValue = token.substr(start);
                newTokens.emplace_back(tokenTypeByValue(newTokenValue), std::move(newTokenValue));
            }
            break;
        }

        if (end != start)
        {
            auto newTokenValue = token.substr(start, end - start);
            newTokens.emplace_back(tokenTypeByValue(newTokenValue), std::move(newTokenValue));
        }

        auto newTokenValue = operatorName;
        newTokens.emplace_back(TokenType::OPERATOR, std::move(newTokenValue));
        start = end + operatorName.size();
    }

    if (!newTokens.empty())
    {
        tokenIterator = tokens.erase(tokenIterator);
        tokens.splice(tokenIterator, newTokens);
        --tokenIterator;
    }
}

s2e2::TokenType s2e2::Tokenizer::tokenTypeByValue(const std::string& value) const
{
    if (operators_.count(value))
    {
        return TokenType::OPERATOR;
    }
    if (functions_.count(value))
    {
        return TokenType::FUNCTION;
    }
    return TokenType::EXPRESSION;
}
