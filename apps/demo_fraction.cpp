#include <ratexpr/fraction.hpp>

#include <iostream>
#include <stdexcept>

using ratexpr::Fraction;

int main() {
    const Fraction half(1, 2);
    const Fraction third(1, 3);

    std::cout << "Fraction(6, -8) = " << Fraction(6, -8) << '\n';
    std::cout << "Fraction(10, 4) = " << Fraction(10, 4) << '\n';
    std::cout << "Fraction(0, 5) = " << Fraction(0, 5) << '\n';
    std::cout << "-(3/4) = " << -Fraction(3, 4) << '\n';

    std::cout << "1/2 + 1/3 = " << half + third << '\n';
    std::cout << "1/2 - 1/3 = " << half - third << '\n';
    std::cout << "1/2 * 1/3 = " << half * third << '\n';
    std::cout << "1/2 / 1/3 = " << half / third << '\n';

    std::cout << "1/2 + 2u = " << half + 2u << '\n';
    std::cout << "2u + 1/2 = " << 2u + half << '\n';
    std::cout << "1/2 - 2u = " << half - 2u << '\n';
    std::cout << "2u - 1/2 = " << 2u - half << '\n';
    std::cout << "1/2 * 2u = " << half * 2u << '\n';
    std::cout << "2u * 1/2 = " << 2u * half << '\n';
    std::cout << "1/2 / 2u = " << half / 2u << '\n';
    std::cout << "2u / 1/2 = " << 2u / half << '\n';

    Fraction acc;
    acc += half;
    acc += third;
    acc *= 6u;
    std::cout << "acc = " << acc << '\n';

    try {
        std::cout << Fraction(1, 0) << '\n';
    } catch (const std::invalid_argument& e) {
        std::cout << "Fraction(1, 0) -> std::invalid_argument: " << e.what() << '\n';
    }

    try {
        std::cout << half / 0u << '\n';
    } catch (const std::invalid_argument& e) {
        std::cout << "1/2 / 0u -> std::invalid_argument: " << e.what() << '\n';
    }

    std::cout << "Fraction(1, 2) * -1 = " << Fraction(1, 2) * -1 << '\n';

    return 0;
}
