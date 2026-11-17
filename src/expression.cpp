// #include <ratexpr/expression.hpp>
//
// #include <ostream>
// #include <stdexcept>
// #include <utility>
//
// namespace ratexpr {
//
//     std::ostream& operator<<(std::ostream& os, const Expression& e) {
//         e.print(os);
//         return os;
//     }
//
//     Const::Const(const Fraction& value) : value_(value) {}
//
//     Fraction Const::evaluate() const {
//         return value_;
//     }
//
//     void Const::print(std::ostream& os) const {
//         os << value_;
//     }
//
//     Negative::Negative(std::unique_ptr<Expression> operand)
//         : operand_(std::move(operand)) {
//         if (!operand_) {
//             throw std::invalid_argument("Negative: null operand");
//         }
//     }
//
//     Fraction Negative::evaluate() const {
//         return -operand_->evaluate();
//     }
//
//     void Negative::print(std::ostream& os) const {
//         os << "(-" << *operand_ << ')';
//     }
//
//     BinaryExpression::BinaryExpression(std::unique_ptr<Expression> lhs,
//                                        std::unique_ptr<Expression> rhs)
//         : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {
//         if (!lhs_ || !rhs_) {
//             throw std::invalid_argument("BinaryExpression: null operand");
//         }
//     }
//
//     void BinaryExpression::print(std::ostream& os) const {
//         os << '(' << *lhs_ << ' ' << symbol() << ' ' << *rhs_ << ')';
//     }
//
//     Fraction Add::evaluate() const { return lhs_->evaluate() + rhs_->evaluate(); }
//     Fraction Sub::evaluate() const { return lhs_->evaluate() - rhs_->evaluate(); }
//     Fraction Mul::evaluate() const { return lhs_->evaluate() * rhs_->evaluate(); }
//     Fraction Div::evaluate() const { return lhs_->evaluate() / rhs_->evaluate(); }
//
// }
#include <ratexpr/expression.hpp>

#include <ostream>
#include <stdexcept>
#include <utility>

namespace ratexpr {

std::ostream& operator<<(std::ostream& os, const Expression& e) {
    e.print(os);
    return os;
}

Const::Const(const Fraction& value) : value_(value) {}

Fraction Const::evaluate() const {
    return value_;
}

void Const::print(std::ostream& os) const {
    os << value_;
}

Negative::Negative(std::unique_ptr<Expression> operand)
    : operand_(std::move(operand)) {
    if (!operand_) {
        throw std::invalid_argument("Negative: null operand");
    }
}

Fraction Negative::evaluate() const {
    return -operand_->evaluate();
}

void Negative::print(std::ostream& os) const {
    os << "(-" << *operand_ << ')';
}

BinaryExpression::BinaryExpression(std::unique_ptr<Expression> lhs,
                                   std::unique_ptr<Expression> rhs)
    : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {
    if (!lhs_ || !rhs_) {
        throw std::invalid_argument("BinaryExpression: null operand");
    }
}

void BinaryExpression::print(std::ostream& os) const {
    os << '(' << *lhs_ << ' ' << symbol() << ' ' << *rhs_ << ')';
}

Fraction Add::evaluate() const { return lhs_->evaluate() + rhs_->evaluate(); }
Fraction Sub::evaluate() const { return lhs_->evaluate() - rhs_->evaluate(); }
Fraction Mul::evaluate() const { return lhs_->evaluate() * rhs_->evaluate(); }
Fraction Div::evaluate() const { return lhs_->evaluate() / rhs_->evaluate(); }

std::unique_ptr<Expression> Add::with_operands(
    std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) const {
    return std::make_unique<Add>(std::move(left), std::move(right));
}

std::unique_ptr<Expression> Sub::with_operands(
    std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) const {
    return std::make_unique<Sub>(std::move(left), std::move(right));
}

std::unique_ptr<Expression> Mul::with_operands(
    std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) const {
    return std::make_unique<Mul>(std::move(left), std::move(right));
}

std::unique_ptr<Expression> Div::with_operands(
    std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) const {
    return std::make_unique<Div>(std::move(left), std::move(right));
}

std::size_t count_operations(const Expression& e) {
    if (const auto* n = dynamic_cast<const Negative*>(&e)) {
        return 1 + count_operations(n->operand());
    }
    if (const auto* b = dynamic_cast<const BinaryExpression*>(&e)) {
        return 1 + count_operations(b->lhs()) + count_operations(b->rhs());
    }
    return 0;
}

std::unique_ptr<Expression> simplify(const Expression& e) {
    if (const auto* c = dynamic_cast<const Const*>(&e)) {
        return std::make_unique<Const>(c->value());
    }

    if (const auto* n = dynamic_cast<const Negative*>(&e)) {
        std::unique_ptr<Expression> inner = simplify(n->operand());
        if (const auto* c = dynamic_cast<const Const*>(inner.get())) {
            return std::make_unique<Const>(-c->value());
        }
        return std::make_unique<Negative>(std::move(inner));
    }

    if (const auto* b = dynamic_cast<const BinaryExpression*>(&e)) {
        std::unique_ptr<Expression> left = simplify(b->lhs());
        std::unique_ptr<Expression> right = simplify(b->rhs());
        const bool foldable = dynamic_cast<const Const*>(left.get()) != nullptr &&
                              dynamic_cast<const Const*>(right.get()) != nullptr;
        std::unique_ptr<Expression> node =
            b->with_operands(std::move(left), std::move(right));
        if (foldable) {
            try {
                return std::make_unique<Const>(node->evaluate());
            } catch (const std::invalid_argument&) {
            }
        }
        return node;
    }

    throw std::invalid_argument("simplify: unknown expression type");
}

}
