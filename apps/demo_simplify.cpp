#include <ratexpr/expression.hpp>

#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace ratexpr;

namespace {

using Clock = std::chrono::steady_clock;
using Ms = std::chrono::duration<double, std::milli>;

struct Row {
    int level = 0;
    std::size_t ops_before = 0;
    std::size_t ops_after = 0;
    double eval_original = 0.0;
    double simplify_time = 0.0;
    double eval_simplified = 0.0;
    Fraction value;
};

std::unique_ptr<Expression> num(std::int64_t n, std::int64_t d = 1) {
    return std::make_unique<Const>(Fraction(n, d));
}

void print_value(const char* label, const Expression& e) {
    std::cout << label << ": ";
    try {
        std::cout << e.evaluate() << '\n';
    } catch (const std::invalid_argument& ex) {
        std::cout << "std::invalid_argument (" << ex.what() << ")\n";
    }
}

void show(const char* title, const Expression& e) {
    std::unique_ptr<Expression> s = simplify(e);
    std::cout << title << '\n';
    std::cout << "  before: " << e << "  [operations: " << count_operations(e) << "]\n";
    std::cout << "  after:  " << *s << "  [operations: " << count_operations(*s) << "]\n";
    std::cout << "  ";
    print_value("value before", e);
    std::cout << "  ";
    print_value("value after ", *s);
    std::cout << '\n';
}

void small_examples() {
    std::unique_ptr<Expression> first = std::make_unique<Div>(
        std::make_unique<Mul>(
            std::make_unique<Add>(num(1, 2), num(1, 3)),
            std::make_unique<Sub>(num(3), num(1, 4))),
        std::make_unique<Negative>(num(5, 2)));
    show("Example 1: the whole tree folds into one constant", *first);

    std::unique_ptr<Expression> second = std::make_unique<Add>(
        std::make_unique<Mul>(num(2), num(3)),
        std::make_unique<Div>(
            num(1),
            std::make_unique<Add>(num(1), num(-1))));
    show("Example 2: division by zero stays in the tree", *second);
}

std::unique_ptr<Expression> build(int level, std::uint32_t& counter) {
    if (level == 0) {
        return std::make_unique<Const>(Fraction(counter++ % 5 + 1, 3));
    }
    std::unique_ptr<Expression> left = build(level - 1, counter);
    std::unique_ptr<Expression> right = build(level - 1, counter);
    return std::make_unique<Div>(
        std::make_unique<Add>(std::move(left), std::move(right)),
        std::make_unique<Const>(Fraction(2)));
}

bool measure(int level, int repeats, Row& row) {
    std::uint32_t counter = 0;
    std::unique_ptr<Expression> tree = build(level, counter);

    row.level = level;
    row.ops_before = count_operations(*tree);

    double sum_eval = 0.0;
    double sum_simplify = 0.0;
    double sum_eval_simplified = 0.0;

    for (int i = 0; i < repeats; ++i) {
        const auto t0 = Clock::now();
        const Fraction original = tree->evaluate();
        const auto t1 = Clock::now();
        std::unique_ptr<Expression> simplified = simplify(*tree);
        const auto t2 = Clock::now();
        const Fraction folded = simplified->evaluate();
        const auto t3 = Clock::now();

        if (original != folded) {
            std::cerr << "MISMATCH on level " << level << ": " << original
                      << " != " << folded << '\n';
            return false;
        }

        if (i == 0) {
            row.ops_after = count_operations(*simplified);
            row.value = original;
        }

        sum_eval += Ms(t1 - t0).count();
        sum_simplify += Ms(t2 - t1).count();
        sum_eval_simplified += Ms(t3 - t2).count();
    }

    row.eval_original = sum_eval / repeats;
    row.simplify_time = sum_simplify / repeats;
    row.eval_simplified = sum_eval_simplified / repeats;
    return true;
}

void print_table(const std::vector<Row>& rows) {
    std::cout << std::fixed << std::setprecision(3) << std::left
              << std::setw(7) << "level"
              << std::setw(12) << "ops before"
              << std::setw(11) << "ops after"
              << std::setw(16) << "eval orig, ms"
              << std::setw(16) << "simplify, ms"
              << std::setw(17) << "eval simpl, ms"
              << std::setw(17) << "simpl+eval, ms"
              << std::setw(12) << "break-even"
              << "value\n";

    for (const Row& r : rows) {
        const double total = r.simplify_time + r.eval_simplified;
        const double gain = r.eval_original - r.eval_simplified;
        const std::string break_even =
            gain > 0.0
                ? std::to_string(static_cast<long long>(std::ceil(r.simplify_time / gain)))
                : std::string("never");

        std::cout << std::setw(7) << r.level
                  << std::setw(12) << r.ops_before
                  << std::setw(11) << r.ops_after
                  << std::setw(16) << r.eval_original
                  << std::setw(16) << r.simplify_time
                  << std::setw(17) << r.eval_simplified
                  << std::setw(17) << total
                  << std::setw(12) << break_even
                  << r.value << '\n';
    }
}

}

int main(int argc, char** argv) {
    small_examples();

    std::vector<int> levels = {10, 14, 18, 21};
    if (argc > 1) {
        levels.clear();
        for (int i = 1; i < argc; ++i) {
            levels.push_back(std::atoi(argv[i]));
        }
    }

    constexpr int repeats = 5;
    std::vector<Row> rows;

    for (int level : levels) {
        if (level < 1 || level > 24) {
            std::cerr << "skipping level " << level << " (allowed: 1..24)\n";
            continue;
        }
        std::cout << "measuring level " << level << "..." << std::endl;
        Row row;
        if (!measure(level, repeats, row)) {
            return 1;
        }
        rows.push_back(row);
    }

    std::cout << "\nAverages over " << repeats << " runs\n";
    print_table(rows);

    return 0;
}
