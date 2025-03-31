#include <optional>
#include "operators.h"

constexpr size_t ERRCODE = -1;

/* NOTE: STRUCT singleOperator STUFF */

real_t singleOperator::eval(real_t x, real_t y) const {
    return m_operatorFunction(x, y);
}

OperatorPrecedence singleOperator::getPrecedence(void) const {
    return m_Precedence;
}

/*  TODO: Optimize this stuff with hashTable or something */
bool singleOperator::isvalidOperatorSymbol(const std::string& operatorSymbol) const {
    const size_t length = m_operatorNames.size();

    for (size_t i = 0; i < length; ++i) {
        if (m_operatorNames[i] == operatorSymbol) {
            return true;
        }
    }

    return false;
}

/* NOTE: CLASS BasicOperators STUFF */

BasicOperators::BasicOperators(const std::vector<singleOperator>& basicOperators): m_OperatorList(basicOperators) {}

void BasicOperators::addlist(const std::vector<singleOperator>& operatorList) {
    const size_t length = operatorList.size();

    for (size_t i = 0; i < length; ++i) {
        m_OperatorList.emplace_back(operatorList[i]);
    }
}

void BasicOperators::add(const singleOperator& singleOperator) {
    m_OperatorList.push_back(singleOperator);
}

singleOperator BasicOperators::get(const size_t position) const {
    if (position > m_OperatorList.size())
        return { nullptr, OperatorPrecedence::None, "NIL" }; /* just does not matter */

    return m_OperatorList[position];
}


/* TODO: We should remove this, way too slow */
std::optional<real_t> BasicOperators::runfunc(const std::string& Operator, real_t val1, real_t val2) {
    size_t len = m_OperatorList.size();

    for (size_t i = 0; i < len; ++i) {
        if (m_OperatorList[i].isvalidOperatorSymbol(Operator)) {
            return std::make_optional(m_OperatorList[i].eval(val1, val2));
        }
    }

    return std::nullopt;
}

/* NOTE: what is this */
singleOperator BasicOperators::get(const std::string& Operator) const {
    size_t pos = ERRCODE, length = m_OperatorList.size();

    for (size_t idx = 0; idx < length; idx++) {
        if (m_OperatorList[idx].isvalidOperatorSymbol(Operator)) {
            pos = idx;
            break;
        }
    }

    /* NOTE: Does not matter and NIL is just so we dont get the warning by the singleOperators constructor for empty argument pack */
    if (pos == ERRCODE) return {nullptr, OperatorPrecedence::None, "NIL"};
    return get(pos);
}

size_t BasicOperators::size(void) const {
    return m_OperatorList.size();
}

/* NOTE: `Remove` functions */
// void BasicOperators::overwrite(const std::vector<singleOperator>& newOperatorList) {
//     size_t size = newOperatorList.size();
//
//     m_OperatorList.clear();
//     m_OperatorList.reserve(size);
//
//     for (size_t idx = 0; idx < size; ++idx) {
//         m_OperatorList.emplace_back(newOperatorList[idx]);
//     }
// }

// void BasicOperators::removeAt(const size_t position) {
//     if (position > m_OperatorList.size()) {
//         return;
//     }
//
//     m_OperatorList.erase(m_OperatorList.begin() + position);
// }


// void BasicOperators::remove(const std::string_view osymbol) {
//     size_t pos = ERRCODE, length = m_OperatorList.size();
//
//     for (size_t idx = 0; idx < length; ++idx) {
//         if (m_OperatorList[idx].isvalidOperatorSymbol(osymbol)) {
//             pos = idx;
//         }
//     }
//
//     if (pos != ERRCODE) {
//         m_OperatorList.erase(m_OperatorList.begin() + pos);
//         return;
//     }
// }
