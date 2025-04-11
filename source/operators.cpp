#include <cstdlib>
#include <unordered_set>
#include <string_view>
#include <initializer_list>

#include "operators.h"

constexpr const char* NIL = "NIL";
std::unordered_set<std::string_view> definedOperators = {};

namespace Matek {
    std::unordered_set<std::string_view> singleOperator::getSymbols(void) const {
        return m_operatorNames;
    }

    bool singleOperator::isvalidSymbol(const std::string_view operatorSymbol) const {
        /* function std::unordered_set::count just count's the number of occurences of operatorSymbol in the set, and since
         * unordered_set does not allow for duplicates, this means it returns either 1 or 0
         * depending on if operatorSymbol exists in m_operatorNames or not */

        return m_operatorNames.count(operatorSymbol);
    }

    BinaryOperators::BinaryOperators(std::initializer_list<singleOperator> initList) : m_OperatorList(initList) {
        definedOperators.reserve(m_OperatorList.size() * m_OperatorList[0].getSymbols().size());
    }

    /* INFO: This just inserts the operator symbols into the definedOperators set */
    void BinaryOperators::updateSymbols(const symbolsList& symbols) {
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
            const std::unordered_set<std::string_view> symbols = boperators[i].getSymbols();
            updateSymbols(symbols);
        }
    }

    void BinaryOperators::add(const std::vector<singleOperator>& operatorList) {
        const size_t length = operatorList.size();

        for (size_t i = 0; i < length; ++i) {
            singleOperator current = operatorList[i];
            m_OperatorList.emplace_back(current);

            const symbolsList& symbols = current.getSymbols();
            updateSymbols(symbols);
        }
    }

    void BinaryOperators::add(const singleOperator& singleOperator) {
        const symbolsList& operatorSymbols = singleOperator.getSymbols();
        updateSymbols(operatorSymbols);
        m_OperatorList.push_back(singleOperator);
    }

    singleOperator BinaryOperators::get(const char* Operator) const {
        size_t pos = ERRCODE, length = m_OperatorList.size();

        for (size_t idx = 0; idx < length; idx++) {
            if (m_OperatorList[idx].isvalidSymbol(Operator)) {
                pos = idx;
                break;
            }
        }

        /* NOTE: We return NIL just so we dont get the warning by the singleOperators constructor for having an empty argument pack */
        if (pos == ERRCODE) return {nullptr, Precedence::INVALID, NIL};
        return get(pos);
    }

    singleOperator BinaryOperators::get(const size_t position) const {
        if (position > m_OperatorList.size())
            return {nullptr, Precedence::INVALID, NIL};

        return m_OperatorList[position];
    }

    Precedence BinaryOperators::getPrecedence(const char* Operator) const {
        singleOperator sopr = get(Operator);
        return sopr.m_precedence;
    }

    size_t BinaryOperators::length(void) const {
        return m_OperatorList.size();
    }
};
