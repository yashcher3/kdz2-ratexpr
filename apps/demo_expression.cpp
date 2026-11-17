#include <ratexpr/expression.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>

using namespace ratexpr;

int main() {
    auto e = std::make_unique<Mul>(
        std::make_unique<Add>(std::make_unique<Const>(Fraction(1, 2)),
                              std::make_unique<Const>(Fraction(1, 3))),
        std::make_unique<Sub>(std::make_unique<Const>(Fraction(3)),
                              std::make_unique<Const>(Fraction(1, 4))));
    std::cout << *e << " = " << e->evaluate() << '\n';

    auto n = std::make_unique<Negative>(
        std::make_unique<Div>(std::make_unique<Const>(Fraction(2)),
                              std::make_unique<Const>(Fraction(3))));
    std::cout << *n << " = " << n->evaluate() << '\n';

    std::unique_ptr<Expression> bad = std::make_unique<Div>(
        std::make_unique<Const>(Fraction(1)),
        std::make_unique<Const>(Fraction(0)));
    std::cout << *bad << " = ";
    try {
        std::cout << bad->evaluate() << '\n';
    } catch (const std::invalid_argument& ex) {
        std::cout << "std::invalid_argument: " << ex.what() << '\n';
    }

    return 0;
}
