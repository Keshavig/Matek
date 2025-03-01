#include "operators.h"

#include <optional>

/* STRUCT singleOperator STUFF */

inline real_t singleOperator::eval(real_t x, real_t y) const {
    return m_operatorFunction(x, y);
}

inline OperatorPrecedence singleOperator::getPrecedence(void) const {
    return m_Precedence;
}

/* CLASS BasicOperators STUFF*/

BasicOperators::BasicOperators(const operatorList& basicOperators): m_OperatorList(basicOperators) {}

void BasicOperators::addlist(const operatorList& operatorList) {
    size_t opplistLen = operatorList.size();

    for (size_t idx = 0; idx < opplistLen; idx++) {
        m_OperatorList.emplace_back(operatorList[idx]);
    }
}

void BasicOperators::add(const singleOperator& singleOperator) {
    m_OperatorList.push_back(singleOperator);
}

singleOperator BasicOperators::get(const size_t position) const {
    /* If pos > m_OperatorList.size() then just return a struct with no operator and nullptr*/
    if (position > m_OperatorList.size())
        return {' ', OperatorPrecedence::LOW, nullptr}; /* just does not matter */

    return m_OperatorList.at(position);
}

void BasicOperators::overwrite(const operatorList& newOperatorList) {
    size_t size = newOperatorList.size();

    m_OperatorList.clear();
    m_OperatorList.reserve(size);

    for (size_t idx = 0; idx < size; idx++) {
        m_OperatorList.emplace_back(newOperatorList[idx]);
    }
}

size_t BasicOperators::size(void) const {
    return m_OperatorList.size();
}

void BasicOperators::removeAt(const size_t position) {
    if (position > m_OperatorList.size()) {
        return;
    }

    m_OperatorList.erase(m_OperatorList.begin() + position);
}


void BasicOperators::remove(const char character) {
    size_t pos = std::string::npos, length = m_OperatorList.size();

    for (size_t idx = 0; idx < length; idx++) {
        if (m_OperatorList[idx].operatorSymbol == character) {
            pos = idx;
        }
    }

    if (pos != std::string::npos) {
        m_OperatorList.erase(m_OperatorList.begin() + pos);
        return;
    }
}

std::optional<real_t> BasicOperators::runfunc(const char _operator, real_t val1, real_t val2) {
    size_t len = m_OperatorList.size();

    for (size_t i = 0; i < len; i++) {
        if (_operator == m_OperatorList[i].operatorSymbol) {
            return std::make_optional(m_OperatorList[i].eval(val1, val2));
        }
    }

    return std::nullopt;
}
