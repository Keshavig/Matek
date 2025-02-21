#include "operators.hpp"

#include <cstddef>
#include <iostream>

#define LOG(X) std::cout << X << std::endl;

/* CONSTRUCTOR */

BasicOperators::BasicOperators(const operatorList& basicOperators): m_OperatorList(basicOperators) {}

void BasicOperators::adlist(const operatorList& operatorList) {
    size_t opplistLen = operatorList.size();

    for (size_t idx = 0; idx < opplistLen; idx++) {
        m_OperatorList.emplace_back(operatorList[idx]);
    }
}

void BasicOperators::adone(const singleOperator& singleOperator) {
    m_OperatorList.push_back(singleOperator);
}

singleOperator BasicOperators::get(const size_t position) const {
    /* If pos > m_OperatorList.size() then just return a struct with no operator and nullptr*/
    if (position > m_OperatorList.size())
        return {' ', nullptr};

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
    LOG(length);

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

