#include <cstdlib>
#include <unordered_set>
#include "operators.h"

constexpr size_t ERRCODE = -1;
constexpr const char* NIL = "NIL";

static std::unordered_set<std::string> definedOperators;

/* ----------------------------- */
/*  STRUCT SINGLEOPERATOR STUFF  */
/* ----------------------------- */

std::unordered_set<std::string> singleOperator::getSymbols(void) const {
    return m_operatorNames;
}

/* Info: Just checks if `operatorSymbol` exists in m_operatorsSet */
bool singleOperator::isvalidSymbol(const std::string& operatorSymbol) const {
    if (m_operatorNames.find(operatorSymbol) == m_operatorNames.end())
        return false;

    return true;
}

/* ---------------------------- */
/*  CLASS BASICOPERATORS STUFF  */
/* ---------------------------- */

/* INFO: This just inserts the operator symbols into the definedOperators set */
void BinaryOperators::updateSymbols(const std::unordered_set<std::string>& symbols) {
    for (const auto& sym : symbols)
    {
        auto [_, uniq] = definedOperators.insert(sym);
        if (!uniq) {
            std::cerr << "ERROR: You have conflicting operator names in your binaryOperators list\n";
            exit(EXIT_FAILURE);
        }
    }
}

BinaryOperators::BinaryOperators(const std::vector<singleOperator>& boperators): m_OperatorList(boperators) {
    const size_t length = boperators.size();

    for (size_t i = 0; i < length; ++i) {
        const std::unordered_set<std::string> symbols = boperators[i].getSymbols();
        updateSymbols(symbols);
    }
}

void BinaryOperators::add(const std::vector<singleOperator>& operatorList) {
    const size_t length = operatorList.size();

    for (size_t i = 0; i < length; ++i) {
        singleOperator current = operatorList[i];
        m_OperatorList.emplace_back(current);

        const std::unordered_set<std::string> symbols = current.getSymbols();
        updateSymbols(symbols);
    }
}

void BinaryOperators::add(const singleOperator& singleOperator) {
    const std::unordered_set<std::string> operatorSymbols = singleOperator.getSymbols();
    updateSymbols(operatorSymbols);
    m_OperatorList.push_back(singleOperator);
}

singleOperator BinaryOperators::get(const size_t position) const {
    if (position > m_OperatorList.size())
        return { nullptr, OperatorPrecedence::None, "NIL" }; /* just does not matter */

    return m_OperatorList[position];
}


/* NOTE: what is this */
singleOperator BinaryOperators::get(const std::string& Operator) const {
    size_t pos = ERRCODE, length = m_OperatorList.size();

    for (size_t idx = 0; idx < length; idx++) {
        if (m_OperatorList[idx].isvalidSymbol(Operator)) {
            pos = idx;
            break;
        }
    }

    /* Info: We return NIL just so we dont get the warning by the singleOperators constructor for having an empty argument pack */
    if (pos == ERRCODE) return {nullptr, OperatorPrecedence::None, "NIL"};
    return get(pos);
}

OperatorPrecedence BinaryOperators::getPrecedence(const std::string& Operator) const {
    singleOperator sopr = get(Operator);
    return sopr.m_precedence;
}

size_t BinaryOperators::length(void) const {
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
